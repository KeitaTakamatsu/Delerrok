//
//  TransitApp.c
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/26.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "CORE2.h"
#include "COREUtils.h"
#include "CloudApp.h"
#include "FeliCa.h"
#include "TransitApp.h"
#include "Account.h"
#include "ResponseData.h"
#include "HistoryData.h"
#include "StoredValue.h"
#include "Pass.h"
#include "Util.h"

farePolicy_t* policy;

void init()
{
    
}


response_t res;
pass_t currentPass;
station_t from;
station_t to;
history_t newhist;
long long fareID;
long long stationIDTo;
long long stationIDFrom;
response_t app(txn_t* txn, account_t* account, agency_t* agency, route_t* route)
{
    dump_agency(agency);
    policy = &agency->policy;
    dump_policy(policy);
    dump_account(account);
    
    if(!checkTxnData(txn, policy))
    {
        // Txnデータが有効ではない
        // レスポンスを返して終了
        return makeResponse(0, 0, MESSAGE_CODE_NONE, MESSAGE_CODE_NONE, account->specialFareProgram, NULL, 0, account->balance);
    }
    if(!checkAccount(agency->agencyGroupCode, account, policy, txn->timestamp, txn->expireDate))
    {
        // アカウントが有効ではない
        // レスポンスを返して終了
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



pass_t currentPass;
response_t flatfare(txn_t* txn, account_t* account, agency_t* agency, route_t* route)
{
    response_t response;
    long long a = makeStationIDFromGPS(txn, route);
    
    dump_txn(txn);
    
    
    int sdbresult = SDBAssign(SDB_INDEX_STATION, *((long long*)(&a)));
    SDBRead(SDB_INDEX_STATION, 0, &from, sizeof(station_t));
    dump_station(&from);
    
    int hasPass, passNumber;
    int passResult = getPass(txn, agency, account, &from, NULL, &hasPass, &passNumber);
    currentPass = account->passList[passNumber];
    if(passResult)
    {
        response = passProcessFlat(passResult, txn, account, &currentPass, route, &from, NO_TRANSFER, 0);
        SDBRelease(SDB_INDEX_STATION);
        return response;
    }
    
    // Stored Value
    response = storedValueFlat(txn, account, agency, route, &from);
    return response;
}



BOOL checkTxnData(txn_t* txn, farePolicy_t* policy)
{
    int i;
    for(i = 0; i < policy->numOfValidCardType; i++)
    {
        if(txn->cardType == policy->validCardTypeList[i])
            return 1;
    }
#ifdef CONSOLE
    printf("[checkTnxData] ****HIT--A--\n");
#endif
    return 0;
}



u_int8 tmpStationID[21];
long long makeStationIDFromGPS(txn_t* txn, route_t* route)
{
    int idx = 0;
    tmpStationID[idx] = STATION_ID_TYPE_GPS; idx++;
    blockcopy(route->routeID, 0, tmpStationID, idx, SIZE_OF_ROUTE_ID); idx += SIZE_OF_ROUTE_ID;
    blockcopy(&txn->northA, 0, tmpStationID, idx, 2); idx += 2;
    blockcopy(&txn->northB, 0, tmpStationID, idx, 4); idx += 4;
    blockcopy(&txn->eastA, 0, tmpStationID, idx, 2);  idx += 2;
    blockcopy(&txn->eastB, 0, tmpStationID, idx, 4);  idx += 4;
    
    long long l = md5(tmpStationID, 21);
    dump_arr("ID=", &l, 0, 8);
    
    return l;
}

station_t st;
station_t getStationFromGPS(txn_t* txn, route_t* route)
{
    int result = SDBAssign(SDB_INDEX_STATION, makeStationIDFromGPS(txn, route));
    SDBRead(SDB_INDEX_STATION, 0, &st, sizeof(station_t));
    SDBRelease(SDB_INDEX_STATION);
    return st;
}

station_t getStation(u_int8* routeID, u_int8* stationID)
{
    int result = SDBAssign(SDB_INDEX_STATION, makeStationID(routeID, stationID));
    SDBRead(SDB_INDEX_STATION, 0, &st, sizeof(station_t));
    SDBRelease(SDB_INDEX_STATION);
    return st;
}


long long makeStationIDFromStation(station_t* station)
{
    int idx = 0;
    tmpStationID[idx] = STATION_ID_TYPE_GPS; idx++;
    blockcopy(station->routeID, 0, tmpStationID, idx, SIZE_OF_ROUTE_ID); idx += SIZE_OF_ROUTE_ID;
    blockcopy(&station->northA, 0, tmpStationID, idx, 2); idx += 2;
    blockcopy(&station->northB, 0, tmpStationID, idx, 4); idx += 4;
    blockcopy(&station->eastA, 0, tmpStationID, idx, 2);  idx += 2;
    blockcopy(&station->eastB, 0, tmpStationID, idx, 4);  idx += 4;
    return md5(tmpStationID, 21);
}


long long makeStationID(u_int8* routeID, u_int8* stationID)
{
    int idx = 0;
    tmpStationID[idx] = STATION_ID_TYPE_DIRECT; idx++;
    blockcopy(routeID, 0, tmpStationID, idx, SIZE_OF_ROUTE_ID); idx += SIZE_OF_ROUTE_ID;
    blockcopy(stationID, 0, tmpStationID, idx, SIZE_OF_STATION_ID); idx += SIZE_OF_STATION_ID;
    return md5(tmpStationID, 21);
}

























