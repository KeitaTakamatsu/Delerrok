//
//  StoredValue.c
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/28.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#include "StoredValue.h"
#include "CloudApp/CORE2.h"
#include "CloudApp/COREUtils.h"
#include "CloudApp/CloudApp.h"
#include "CloudApp/FeliCa.h"
#include "CloudApp/TransitApp.h"
#include "Transfer.h"
#include "Account.h"
#include "ResponseData.h"
#include "HistoryData.h"
#include "Pass.h"
#include "Util.h"


/* Process Stored Value for Flat Fare and return Response Data. */
response_t storedValueFlat(txn_t* txn, account_t* account, agency_t* agency, route_t* route, station_t* station, u_int8 spDiscount)
{
    transferData_t newTransferData;
    farePolicy_t* policy = &agency->policy;
    fare_t fare;
    
    if(!policy->storedValueEnabled)
        return makeResponse(0, 0, 0, 0, account->specialFareProgram, NULL, 0, account->balance);
    
    memcpy(&newTransferData, &account->transferData, sizeof(transferData_t));
    
    /* Get Transfer Result */
    int transferResult = transferFlat(txn, agency, route, account, station, txn->timestamp, &newTransferData);
    long long flatFareID;
    
    if(transferResult == TRANSFER_TYPE_FREE)
    {
        fare.fareValue = 0;
    }
    else
    {
        dump_station(station);
        
        /* Make Fare ID for Flat Fare */
        flatFareID = makeFlatFareID(station->agencyID, station->zone.zoneID, account->specialFareProgram, transferResult, 0,               spDiscount);
        dump_arr("AgencyID = ", station->agencyID, 0, 8);
        dump_arr("ZoneID = ", station->zone.zoneID, 0, 8);
        SDBAssign(SDB_INDEX_FARE, flatFareID);
        SDBRead(SDB_INDEX_FARE, 0, &fare, sizeof(fare_t));
        SDBRelease(SDB_INDEX_FARE);
    }
    
    /* Update Account */
    if(account->balance >= fare.fareValue)
    {
        account->balance -= fare.fareValue;
        /* Update Transfer Data */
        account->transferData = newTransferData;
        /* Update Last History */
        account->lastHistory = makeHistoryData(agency->agencyID, 1, route, station, txn->timestamp, 1);
        dump_history(&account->lastHistory);
        
        /* Write Account Data to SDB */
        SDBWrite(SDB_INDEX_ACCOUNT, account, 0, sizeof(account_t));
        return makeResponse(OPEN, fare.fareValue, 0, 0, account->specialFareProgram, NULL, 0, account->balance);
    }
    else
    {
        /* Minus Balance */
        if(!policy->minusBalanceEnabled)
            return makeResponse(0, fare.fareValue, 0, 0, account->specialFareProgram, NULL, 0, account->balance);
        /* Update Transfer Data */
        if(transferResult)
            account->transferData = newTransferData;
        /* Update LastHistory */
        account->lastHistory = makeHistoryData(agency->agencyID, 1, route, station, txn->timestamp, 1);
        
        /* Write Account Data to SDB */
        SDBWrite(SDB_INDEX_ACCOUNT, account, 0, sizeof(account_t));
        return makeResponse(OPEN, fare.fareValue, 0, 0, account->specialFareProgram, NULL, 0, account->balance);
    }
}


u_int8 flatFareID[SIZE_OF_AGENCY_ID+SIZE_OF_ZONE_ID+4];
u_int64 makeFlatFareID(u_int8* agencyID, u_int8* zoneID, u_int8 sfp, u_int8 transferType, u_int8 peaktimeCode, u_int8 passDiscount)
{
    int idx = 0;
    blockcopy(agencyID, 0, flatFareID, idx, SIZE_OF_AGENCY_ID); idx += SIZE_OF_AGENCY_ID;
    blockcopy(zoneID, 0, flatFareID, idx, SIZE_OF_ZONE_ID); idx += SIZE_OF_ZONE_ID;
    flatFareID[idx] = sfp; idx++;
    flatFareID[idx] = transferType; idx++;
    flatFareID[idx] = peaktimeCode; idx++;
    flatFareID[idx] = passDiscount; idx++;
    return md5(flatFareID, SIZE_OF_AGENCY_ID+SIZE_OF_ZONE_ID+4);
}


u_int64 makeFlatFareIDFromFare(fare_t* fare)
{
    int idx = 0;
    blockcopy(fare->agencyID, 0, flatFareID, idx, SIZE_OF_AGENCY_ID); idx += SIZE_OF_AGENCY_ID;
    blockcopy(fare->zoneID, 0, flatFareID, idx, SIZE_OF_ZONE_ID); idx += SIZE_OF_ZONE_ID;
    flatFareID[idx] = fare->sfp; idx++;
    flatFareID[idx] = fare->transferType; idx++;
    flatFareID[idx] = fare->peaktimeCode; idx++;
    flatFareID[idx] = fare->passDiscount; idx++;
    return md5(flatFareID, SIZE_OF_AGENCY_ID+SIZE_OF_ZONE_ID+4);
}

