//
//  StoredValue.c
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/28.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#include "StoredValue.h"
#include "CORE2.h"
#include "COREUtils.h"
#include "CloudApp.h"
#include "FeliCa.h"
#include "TransitApp.h"
#include "Transfer.h"
#include "Account.h"
#include "ResponseData.h"
#include "HistoryData.h"
#include "Pass.h"



response_t storedValueFlat(txn_t* txn, account_t* account, agency_t* agency, route_t* route, station_t* station)
{
    transferData_t newTransferData;
    farePolicy_t* policy = &agency->policy;
    
    if(!policy->storedValueEnabled)
        return makeResponse(0, 0, 0, 0, account->specialFareProgram, NULL, 0, account->balance);
    
    memcpy(&newTransferData, &account->transferData, sizeof(transferData_t));
    
    int transferResult = checkTransferFlat(txn, agency, route, account, station, txn->timestamp, &newTransferData);
    long long flatFareID = makeFlatFareID(station->zone.zoneID, account->specialFareProgram, transferResult, 0);
    fare_t fare;
    
    SDBAssign(SDB_INDEX_FARE, flatFareID);
    SDBRead(SDB_INDEX_FARE, 0, &fare, sizeof(fare_t));
    SDBRelease(SDB_INDEX_FARE);
    
    if(account->balance >= fare.fareValue)
    {
        account->balance -= fare.fareValue;
        account->transferData = newTransferData;
        account->lastHistory = makeHistoryData(1, route, station, txn->timestamp, 1);
        dump_history(&account->lastHistory);
        SDBWrite(SDB_INDEX_ACCOUNT, account, 0, sizeof(account_t));
        return makeResponse(OPEN, fare.fareValue, 0, 0, account->specialFareProgram, NULL, 0, account->balance);
    }
    else
    {
        if(!policy->minusBalanceEnabled)
            return makeResponse(0, fare.fareValue, 0, 0, account->specialFareProgram, NULL, 0, account->balance);
        
        if(transferResult)
            account->transferData = newTransferData;
        account->lastHistory = makeHistoryData(1, route, station, txn->timestamp, 1);
        SDBWrite(SDB_INDEX_ACCOUNT, account, 0, sizeof(account_t));
        return makeResponse(OPEN, fare.fareValue, 0, 0, account->specialFareProgram, NULL, 0, account->balance);        
    }
}


u_int8 flatFareID[SIZE_OF_ZONE_ID+3];
long long makeFlatFareID(u_int8* zoneID, u_int8 sfp, u_int8 transferType, u_int8 peaktimeCode)
{
    int idx = 0;
    blockcopy(zoneID, idx, flatFareID, 0, SIZE_OF_ZONE_ID); idx += SIZE_OF_ZONE_ID;
    flatFareID[idx] = sfp; idx++;
    flatFareID[idx] = transferType; idx++;
    flatFareID[idx] = peaktimeCode; idx++;
    return md5(flatFareID, SIZE_OF_ZONE_ID+3);
}


long long makeFlatFareIDFromFare(fare_t* fare)
{
    int idx = 0;
    blockcopy(fare->zoneID, idx, flatFareID, 0, SIZE_OF_ZONE_ID); idx += SIZE_OF_ZONE_ID;
    flatFareID[idx] = fare->sfp; idx++;
    flatFareID[idx] = fare->transferType; idx++;
    flatFareID[idx] = fare->peaktimeCode; idx++;
    return md5(flatFareID, SIZE_OF_ZONE_ID+3);    
}

