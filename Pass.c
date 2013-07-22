#include "Pass.h"
#include "ResponseData.h"
#include "HistoryData.h"
#include "Util.h"


/*
 Get valid Pass from Account.
 This method return Pass Result.
    PASS_RESULT_INVALID 0
        Account does not have a valid Pass.
    PASS_RESULT_VALID 1
        Account have a valid Pass.
    PASS_RESULT_UPDATE_TIME 2
        Account have a pending Time Based Pass.
    PASS_RESULT_UPDATE_TRIP 3
        Account have a Trip Based Pass.
 */
int getPass(txn_t* txn, agency_t* agency, account_t* account, station_t* from, station_t* to, int* hasPass, int* passNumber)
{
    int i, result;
    for(i = 0; i < SIZE_OF_PASS_LIST; i++)
    {        
        dump_pass(&(account->passList[i]));
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


// history_t tmp_hist;
response_t tmp_res;

/*
    This method return Response Data and update Last History.
*/
response_t passProcessFlat(u_int8* agencyID, int passCheckResult, txn_t* txn, account_t* account, pass_t* pass, route_t* route ,station_t* station, u_int8 transfer, u_int8 transferCount)
{
    
    switch(passCheckResult)
    {
        case RESULT_VALID:
        {
            tmp_res = makeResponse(OPEN, FARE_FREE, MESSAGE_CODE_NONE, ERROR_CODE_NONE, account->specialFareProgram, pass->passExpireDate, pass->numOfTripBasedPass, account->balance);
            account->lastHistory = makeHistoryData(agencyID, HISTORY_TYPE_PASS, route, station, txn->timestamp, PAYMENT_TYPE_PASS);
            
            break;
        }
        case RESULT_UPDATE_TIME:
        {
            struct tm* newtime = getActivatedTimeBasedPassTime(pass, txn->timestamp);
            struct tm now = makeTimeYYMMDDHHmmSS(txn->timestamp);
            timeBasedPassActivate(pass, now, *newtime);
            tmp_res = makeResponse(OPEN, FARE_FREE, MESSAGE_CODE_NONE, ERROR_CODE_NONE, account->specialFareProgram, pass->passExpireDate, pass->numOfTripBasedPass, account->balance);
            account->lastHistory = makeHistoryData(agencyID, HISTORY_TYPE_PASS, route, station, txn->timestamp, PAYMENT_TYPE_PASS);
            break;
        }
        case RESULT_UPDATE_TRIP:
        {
            pass->numOfTripBasedPass--;
            tmp_res = makeResponse(OPEN, FARE_FREE, MESSAGE_CODE_NONE, ERROR_CODE_NONE, account->specialFareProgram, pass->passExpireDate, pass->numOfTripBasedPass, account->balance);
            account->lastHistory = makeHistoryData(agencyID, HISTORY_TYPE_PASS, route, station, txn->timestamp, PAYMENT_TYPE_PASS);
            
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
    
    
    /* Update soon. Need Region Code. */
    //if(!compare(agencyID, 0, pass->validAgencyID, 0, 2))
    //    return PASS_RESULT_INVALID;
    /* ------------------------------ */
    
    if(pass->passType == PASS_TYPE_TIME)
    {
        /*
          Update Time Based Pass.
         */
        
        if(pass->timeBasedPassRenewalUnits != 0)
        {
            newtime = getActivatedTimeBasedPassTime(pass, timestamp);
            if(newtime == NULL)
            {
#ifdef CONSOLE
                printf("This Pass doesn't have value for renew.\n");
#endif
                return PASS_RESULT_INVALID;
            }
            else
            {
#ifdef CONSOLE
                printf("RESULT_NEED_UPDATE_TIME\n");
#endif
                expire = *newtime;
                start = now;
                result = PASS_RESULT_UPDATE_TIME;
            }
        }
    }
    else
    {
        expire = makeTimeYYMMDDHHmmSS(pass->passExpireDate);
        start = makeTimeYYMMDDHHmmSS(pass->passStartDate);
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
            result = PASS_RESULT_UPDATE_TRIP;
            break;
    }
    
    /* Check Datetime */
    int _a,_b;
    _a = datetimeCompare(now, start);
    _b = datetimeCompare(now, expire);
    
    
    
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

struct tm activatedPassTime;

/* This method return Time that is activated. */
struct tm* getActivatedTimeBasedPassTime(pass_t* pass, u_int8* timestamp)
{
    if(pass->timeBasedPassNumberOfRenewalUnits <= 0)
        return NULL;
    
    struct tm time = makeTimeYYMMDDHHmmSS(timestamp);
    switch (pass->timeBasedPassRenewalUnits)
    {
        case TBPASS_ACTIVATE_TYPE_HOUR:
            activatedPassTime = addTime(time, 0, 0, 0, pass->timeBasedPassRenewalUnits);
            break;
        case TBPASS_ACTIVATE_TYPE_DAYS:
            activatedPassTime = addTime(time, 0, 0, pass->timeBasedPassRenewalUnits, 0);
            break;
    }
    return &activatedPassTime;
}

/* Activate Time Based Pass */
void timeBasedPassActivate(pass_t* pass, struct tm now, struct tm newTime)
{    
    makeYYYYMMDD(now, pass->passStartDate);
    makeYYYYMMDD(newTime, pass->passExpireDate);
    pass->timeBasedPassRenewalUnits = 0;
    pass->timeBasedPassNumberOfRenewalUnits = 0;
}


int checkValidZone(pass_t* pass, u_int8* zoneID)
{
    const u_int8 VALID[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    const u_int8 INVALID[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    
    int i;
    for(i = 0; i < pass->numOfValidZoneID; i++)
    {
        if(compare(pass->validZoneIDList, 0, INVALID, 0, SIZE_OF_ZONE_ID))
            return 0;
        if(compare(pass->validZoneIDList, 0, VALID, 0, SIZE_OF_ZONE_ID))
            return 1;
        if(compare(pass->validZoneIDList, 0, zoneID, 0, SIZE_OF_ZONE_ID))
            return 1;
    }
    return 0;
}




