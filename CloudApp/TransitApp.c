//
//  TransitApp.c
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/26.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CORE2.h"
#include "COREUtils.h"
#include "CloudApp.h"
#include "FeliCa.h"
#include "TransitApp.h"
#include "../Account.h"
#include "../ResponseData.h"
#include "../HistoryData.h"
#include "../StoredValue.h"
#include "../Pass.h"
#include "../Util.h"

farePolicy_t* policy;

void init()
{
    
}


response_t res;
pass_t* currentPass;
station_t from;
station_t to;
history_t newhist;
u_int64 fareID;
u_int64 stationIDTo;
u_int64 stationIDFrom;



response_t app(txn_t* txn, account_t* account, agency_t* agency, route_t* route)
{
    policy = &agency->policy;
    
    dump_agency(agency);
    dump_policy(policy);
    dump_account(account);
    
    if(!checkTxnData(txn, policy))
    {
        // Txn data is invalid
        // Return response and exit.
        return makeResponse(0, 0, MESSAGE_CODE_NONE, MESSAGE_CODE_NONE, account->specialFareProgram, NULL, 0, account->balance);
    }
    if(!checkAccount(agency->agencyGroupCode, account, policy, txn->timestamp, txn->expireDate))
    {
        // Account is invalid.
        // Return response and exit.
        return makeResponse(0, 0, MESSAGE_CODE_NONE, MESSAGE_CODE_NONE, account->specialFareProgram, NULL, 0, account->balance);
    }
    
    switch (policy->fareType)
    {
        case FARE_TYPE_FLAT:
            return flatfare(txn, account, agency, route);
        case FARE_TYPE_ZONE:
            break;
        case FARE_TYPE_DISTANCE:
            break;
    }
    return res;
}




response_t flatfare(txn_t* txn, account_t* account, agency_t* agency, route_t* route)
{
#ifdef DEBUG_MODE
    /* Cloud App Debug Mode send Account Data that is not influenced by current transaction to Client. */
    memcpy(&beforeAccount, account, sizeof(account_t));
#endif
    
    response_t response;
    u_int64 stationID = makeStationIDFromGPS2(txn, route);
    dump_txn(txn);
    
    /* Get Station Data from SDB */
    SDBAssign(SDB_INDEX_STATION, stationID);
    SDBRead(SDB_INDEX_STATION, 0, &from, sizeof(station_t));
    dump_station(&from);
    
#ifdef DEBUG_MODE
    /* Cloud App Debug Mode send Station Data to Client. */
    memcpy(&stationForDebug, &from, sizeof(station_t));
#endif
    
    int hasPass, passNumber;
    int passResult = getPass(txn, agency, account, &from, NULL, &hasPass, &passNumber, route->specialDiscount);
    currentPass = &account->passList[passNumber];
    if(passResult && passResult != PASS_TYPE_DISCOUNT)
    {
        response = passProcessFlat(agency->agencyID, passResult, txn, account, currentPass, route, &from, NO_TRANSFER, 0);
        SDBRelease(SDB_INDEX_STATION);
        
#ifdef DEBUG_MODE
        /* Cloud App Debug Mode send Account Data that is influenced by current transaction to Client. */
        memcpy(&afterAccount, account, sizeof(account_t));
#endif
        
        return response;
    }
    
    // Stored Value
    response = storedValueFlat(txn, account, agency, route, &from, passResult == PASS_TYPE_DISCOUNT ? 1 : 0);
    
#ifdef DEBUG_MODE
    /* Cloud App Debug Mode send Account Data that is influenced by current transaction to Client. */
    memcpy(&afterAccount, account, sizeof(account_t));
#endif    
    
    return response;
}



BOOL checkTxnData(txn_t* txn, farePolicy_t* policy)
{
    int i;
    /* Check Card Type */
    for(i = 0; i < policy->numOfValidCardType; i++)
    {
        if(txn->cardType == policy->validCardTypeList[i])
            break;
        if(i == SIZE_OF_VALID_CARD_LIST-1)
        {
            #ifdef DEBUG_CONSOLE
                printf("[checkTnxData] ****HIT--A--\n");
            #endif
            return 0;
        }
    }
    
    /* Add more check as necessary */
    
    return 1;
}




station_t st;
/* I will change this method soon. */
u_int64 makeStationIDFromGPS2(txn_t* txn, route_t* r)
{
    int i;
    u_int64 sdbid, var;
    double comp, min = 9999999;
    
    for(i = 0; i < r->numOfStation; i++)
    {
        if(min > (comp = gpsCompValue2(txn, r, i*12)))
        {
            min = comp;
            //var = *((long long*) (r->stationIDList+i*8));
            var = toUInt64(r->stationIDList, i*8);
            dump_arr("",r->stationIDList, i*8, 8);
        }
    }
    return var;
}

u_int64 gpsCompValue2(txn_t* txn, route_t* r, int index)
{
    int64 txn_north = (int64)txn->northA*1000000 + txn->northB;
    int64 txn_east = (int64)txn->eastA*1000000 + txn->eastB;
    int64 st_north = (int64)toInt16(r->gpsLocationList, index)*1000000+toInt32(r->gpsLocationList, index+2);
    int64 st_east = (int64)toInt16(r->gpsLocationList, index+6)*1000000+toInt32(r->gpsLocationList, index+8);
    int64 a = labs(st_north - txn_north);
    int64 b = labs(st_east - txn_east);
    
    return a+b;
}


/*
u_int64 gpsCompValue2(txn_t* txn, route_t* r, int index)
{
    long long txn_north = ((long long) txn->northA)*1000000 + ((long long) txn->northB);
    long long txn_east = ((long long) txn->eastA)*1000000 + ((long long) txn->eastB);
    long long st_north = ((long long)*((short*) r->gpsLocationList))*1000000+((long long)*((int*)(r->gpsLocationList+index+2)));
    long long st_east = ((long long)*((short*) (r->gpsLocationList+6)))*1000000+((long long)*((int*) (r->gpsLocationList+8)));
    
    long long a = labs(st_north - txn_north);
    long long b = labs(st_east - txn_east);
    return a+b;
}
*/

u_int64 gpsCompValue(txn_t* txn, station_t* st)
{
    double txn_north = ((double)txn->northA)+((double)txn->northB)/1000000;
    double txn_east = ((double)txn->eastA)+((double)txn->eastB)/1000000;
    double st_north = ((double)st->northA)+((double)st->northB)/1000000;
    double st_east  = ((double)st->eastA)+((double)st->eastB)/1000000;
    
    double a = labs(st_north - txn_north);
    double b = labs(st_east - txn_east);
    return a+b;
}























