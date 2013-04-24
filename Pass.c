//
//  Pass.c
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/26.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#include "Pass.h"
#include "ResponseData.h"
#include "HistoryData.h"
#include "Util.h"


int getPass(txn_t* txn, agency_t* agency, account_t* account, station_t* from, station_t* to, int* hasPass, int* passNumber)
{
    int i, result;
    for(i = 0; i < SIZE_OF_PASS_LIST; i++)
    {
        if(account->passList[i].passType != 0)
            *hasPass = 1;
        result = passCheck(txn, &(agency->policy), from, to, account, &(account->passList[i]));
        if(result > 0)
        {
            *passNumber = i;
            return result;
        }
    }
    return 0;
}


history_t tmp_hist;
response_t tmp_res;
response_t passProcessFlat(int passCheckResult, txn_t* txn, account_t* account, pass_t* pass, route_t* route ,station_t* station, u_int8 transfer, u_int8 transferCount)
{
    switch(passCheckResult)
    {
        case RESULT_VALID:
        {
            tmp_res = makeResponse(OPEN, FARE_FREE, MESSAGE_CODE_NONE, ERROR_CODE_NONE, account->specialFareProgram, pass->passExpireDate, pass->numOfTripBasedPass, account->balance);
            account->lastHistory = makeHistoryData(HISTORY_TYPE_FLAT_NORMAL, route, station, txn->timestamp, PAYMENT_TYPE_PASS);
            
            break;
        }
        case RESULT_UPDATE_TIME:
        {
            struct tm* newtime = getActivatedTimeBasedPass(pass, txn->timestamp);
            struct tm now = makeTimeYYMMDDHHmmSS(txn->timestamp);
            timeBasedPassActivate(pass, now, *newtime);
            tmp_res = makeResponse(OPEN, FARE_FREE, MESSAGE_CODE_NONE, ERROR_CODE_NONE, account->specialFareProgram, pass->passExpireDate, pass->numOfTripBasedPass, account->balance);
            account->lastHistory = makeHistoryData(HISTORY_TYPE_FLAT_NORMAL, route, station, txn->timestamp, PAYMENT_TYPE_PASS);
            break;
        }
        case RESULT_UPDATE_TRIP:
        {
            pass->numOfTripBasedPass--;
            tmp_res = makeResponse(OPEN, FARE_FREE, MESSAGE_CODE_NONE, ERROR_CODE_NONE, account->specialFareProgram, pass->passExpireDate, pass->numOfTripBasedPass, account->balance);
            account->lastHistory = makeHistoryData(HISTORY_TYPE_FLAT_NORMAL, route, station, txn->timestamp, PAYMENT_TYPE_PASS);
            
            break;
        }
    }
    return tmp_res;
}

int passCheck(txn_t* txn, farePolicy_t* policy, station_t* from, station_t* to, account_t* account, pass_t* pass)
{
    int result;
    if(pass->passType == 0)
        return 0;
    switch(policy->fareType)
    {
        case FARE_TYPE_FLAT:
            result =passCheckFlat(txn, policy, from, account, pass);
            break;
        case FARE_TYPE_ZONE:
            break;
        case FARE_TYPE_DISTANCE:
            break;
    }
    return result;
}


int passCheckFlat(txn_t* txn, farePolicy_t* policy, station_t* from, account_t* account, pass_t* pass)
{
    return checkPassValid(pass, txn->agencyID, txn->routeID, from->zone.zoneID, txn->timestamp);
}


int checkPassValid(pass_t* pass, u_int8* agencyID, u_int8* routeID, u_int8* zoneID, u_int8* timestamp)
{
    int result = 0;
    const u_int8 INVALID_DATE[] = {0x01, 0x01,0x00,0x01};
    struct tm* newtime;
    struct tm now = makeTimeYYMMDDHHmmSS(timestamp);
    struct tm expire;
    struct tm start;
    
    if(!checkValidZone(pass, zoneID))
        return PASS_RESULT_INVALID;
    
    if(!compare(agencyID, 0, pass->validAgencyID, 0, 2))
        return PASS_RESULT_INVALID;
        
    if(pass->passType == PASS_TYPE_TIME)
    {
        if(pass->timeBasedPassAddTimeType != 0)
        {
            newtime = getActivatedTimeBasedPass(pass, timestamp);
            if(newtime == NULL)
            {
#ifdef DEBUG
                printf("HIT---PASS---D\n");
#endif
                return PASS_RESULT_INVALID;
            }
            else
            {
#ifdef DEBUG
                printf("HIT---PASS---RESULT_NEED_UPDATE_TIME\n");
#endif
                expire = *newtime;
                start = now;
                result = PASS_RESULT_UPDATE_TIME;
            }
        }
    }
    else
    {
        expire = makeTimeYYYYMMDD(pass->passExpireDate);
        start = makeTimeYYYYMMDD(pass->passStartDate);
    }
    
    switch(pass->passType)
    {
        case PASS_TYPE_CALENDAR:
            result = PASS_RESULT_VALID;
            break;
        case PASS_TYPE_TIME:
            result = PASS_RESULT_UPDATE_TIME;
            break;
        case PASS_TYPE_TRIP:
            if(pass->numOfTripBasedPass <= 0)
                return PASS_RESULT_INVALID;
            else
                result = PASS_RESULT_UPDATE_TRIP;
            break;
    }
    
    int _a,_b;
    _a = datetimeCompare(now, start);
    _b = datetimeCompare(now, expire);
    dump_tm_short(start);
    dump_tm_short(expire);
    dump_tm_short(now);
    if(datetimeCompareShort(now, start) < 0)
        return RESULT_INVALID;
    if(datetimeCompareShort(now, expire) > 0)
        return RESULT_INVALID;
    
    
    if(pass->passType == PASS_TYPE_TRIP)
    {
        if(pass->numOfTripBasedPass <= 0)
        {
            pass->passType = 0;
            memcpy(pass->passExpireDate, &INVALID_DATE, 4);
            memcpy(pass->passStartDate, &INVALID_DATE, 4);
            return RESULT_INVALID;
        }
        else
        {
            pass->numOfTripBasedPass--;
        }
    }   
    
    return result;
}

struct tm activatedPass;
struct tm* getActivatedTimeBasedPass(pass_t* pass, u_int8* timestamp)
{
    if(pass->timeBasedPassAddTime <= 0)
        return NULL;
    
    struct tm time = makeTimeYYMMDDHHmmSS(timestamp);
    switch (pass->timeBasedPassAddTimeType)
    {
        case TBPASS_ACTIVATE_TYPE_HOUR:
            activatedPass = addTime(time, 0, 0, 0, pass->timeBasedPassAddTime);
            break;
        case TBPASS_ACTIVATE_TYPE_DAYS:
            activatedPass = addTime(time, 0, 0, pass->timeBasedPassAddTime, 0);
            break;
        case TBPASS_ACTIVATE_TYPE_WEEKS:
            activatedPass = addTime(time, 0, 0, pass->timeBasedPassAddTime*7, 0);
            break;
        case TBPASS_ACTIVATE_TYPE_MONTH:
            activatedPass = addTime(time, 0, pass->timeBasedPassAddTime, 0, 0);
            break;
        case TBPASS_ACTIVATE_TYPE_YEAR:
            activatedPass = addTime(time, pass->timeBasedPassAddTime, 0, 0, 0);
            break;
    }
    return &activatedPass;
}


void timeBasedPassActivate(pass_t* pass, struct tm now, struct tm newTime)
{    
    makeYYYYMMDD(now, pass->passStartDate);
    makeYYYYMMDD(newTime, pass->passExpireDate);
    pass->timeBasedPassAddTime = 0;
    pass->timeBasedPassAddTimeType = 0;
}


int checkValidZone(pass_t* pass, u_int8* zoneID)
{
    const u_int8 VALID[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    const u_int8 INVALID[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    
    int i;
    for(i = 0; i < SIZE_OF_VALID_ZONE_LIST_FOR_PASS; i++)
    {
        if(compare(pass->validZoneIDList[i], 0, INVALID, 0, SIZE_OF_ZONE_ID))
            return 0;
        if(compare(pass->validZoneIDList[i], 0, VALID, 0, SIZE_OF_ZONE_ID))
            return 1;
        if(compare(pass->validZoneIDList[i], 0, zoneID, 0, SIZE_OF_ZONE_ID))
            return 1;
    }
    return 0;
}




