//
//  Util.c
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/28.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#include "Util.h"

void blockcopy(u_int8* src, int srcidx, u_int8* dst, int dstidx, int len)
{
    int i;
    for(i = 0; i < len; i++)
    {
        dst[i+dstidx] = src[i+srcidx];
    }
}


md5_state_t state;
md5_byte_t digest[16];
long long md5(char* data, int len)
{
    md5_init(&state);
    md5_append(&state, data, len);
    md5_finish(&state, digest);
    return *((long long*) digest);
}

int compare(u_int8* a, int idx_a, u_int8* b, int idx_b, int len)
{
    int i;
    for(i = 0; i < len; i++)
    {
        if(a[i+idx_a] != b[i+idx_b])
            return 0;
    }
    return 1;
}

struct tm getNow(){
    time_t now = time(NULL);
    struct tm t = *localtime(&now);
    t.tm_year += 1900;
    t.tm_mon += 1;
    return t;
}

struct tm addTime(struct tm src, int addYear, int addMonth, int addDays, int addHour){
    struct tm _src = src;
    dump_tm(src);
    _src.tm_year += addYear;
    _src.tm_mon += addMonth-1;
    _src.tm_mday += addDays;
    _src.tm_hour += addHour;
    time_t t1 = mktime(&_src);
    
    struct tm val = *localtime(&t1);
    val.tm_mon +=1;
    return val;
}

struct tm addMin(struct tm src, int min){
    struct tm _src = src;
    _src.tm_min = min;
    time_t t1 = mktime(&_src);
    
    struct tm val = *localtime(&t1);
    return val;
}

void makeYYMMDDHHmmSSFromTime(struct tm timestamp, u_int8* time)
{
    time[0] = (u_int8)timestamp.tm_year-100;
    time[1] = (u_int8)timestamp.tm_mon+1;
    time[2] = (u_int8)timestamp.tm_mday;
    time[3] = (u_int8)timestamp.tm_hour;
    time[4] = (u_int8)timestamp.tm_min;
    time[5] = (u_int8)timestamp.tm_sec;
}

void makeYYYYMMDD(struct tm timestamp, u_int8* time)
{
    time[0] = timestamp.tm_year / 100;
    time[1] = timestamp.tm_year % 100;
    time[2] = timestamp.tm_mon;
    time[3] = timestamp.tm_mday;
}


struct tm makeTimeYYMMDDHHmmSS(u_int8* timestamp)
{
    struct tm time;
    int YY = timestamp[0];
    int MM = timestamp[1];
    int DD = timestamp[2];
    int HH = timestamp[3];
    int mm = timestamp[4];
    int SS = timestamp[5];
    
    time.tm_year = YY+100;
    time.tm_mon = MM-1;
    time.tm_mday = DD;
    time.tm_hour = HH;
    time.tm_min = mm;
    time.tm_sec = SS;
    
    return time;
}

struct tm makeTimeYYYYMMDD(u_int8* timestamp)
{
    struct tm time;
    int YYYY = timestamp[0]*100+timestamp[1];
    int MM = timestamp[2];
    int DD = timestamp[3];
    
    time.tm_year = YYYY+100;
    time.tm_mon = MM-1;
    time.tm_mday = DD;
    
    return time;
}


double datetimeCompare(struct tm time1, struct tm time2)
{
    time_t t1  = mktime(&time1);
    time_t t2  = mktime(&time2);
    
#ifdef DEBUG
    printf("[DateTimeCompare]\n");
    printf("|*** %ld \n", t1);
    printf("|*** %ld \n", t2);
    printf("|*** %f \n", difftime(t1, t2));
    printf("|*** %ld\n", t2 - t1);
#endif
    
    return difftime(t1, t2);
}

double datetimeCompareShort(struct tm time1, struct tm time2)
{
    time_t t1 = mktime(&time1) / (60*60*24);
    time_t t2 = mktime(&time2) / (60*60*24);
    
#ifdef DEBUG
    printf("[DateTimeCompareShort]\n");
    printf("|*** %ld \n", t1);
    printf("|*** %ld \n", t2);
    printf("|*** %f \n", difftime(t1, t2));
    printf("|*** %ld\n", t2 - t1);
#endif
    return difftime(t1, t2);
}


void dump_txn(txn_t* txn)
{
    int i;
    printf("AgencyID=");
    for(i = 0; i < 8; i++)
        printf("%02d", txn->agencyID[i]);
    printf("\n");
    
    printf("RouteID=");
    for(i = 0; i < 8; i++)
        printf("%02d", txn->routeID[i]);
    printf("\n");
    
    printf("Timestamp=");
    for(i = 0; i < 8; i++)
        printf("%02d", txn->timestamp[i]);
    printf("\n");
    
    printf("CardID=");
    for(i = 0; i < 8; i++)
        printf("%2d ", txn->cardID[i]);
    printf("\n");
    
    printf("CardType=%d\n", txn->cardType);
    
    printf("Expire=");
    for(i = 0; i < 4; i++)
        printf("%02d", txn->expireDate[i]);
    printf("\n");
    
    printf("North=%d.%d\n", txn->northA, txn->northB);
    printf("East =%d.%d\n", txn->eastA, txn->eastB);
}


void dump_route(route_t* route)
{
    printf("[Route]\n");
    dump_arr("RouteID=", route->routeID, 0, SIZE_OF_ROUTE_ID);
    dump_arr("AgencyID=", route->agencyID, 0, SIZE_OF_AGENCY_ID);
    printf("ModeType=%d\n", route->modeType);
    printf("RouteType=%d\n", route->routeType);
    printf("Direction=%d\n", route->direction);
    printf("NumOfConnectedRoute=%d\n", route->numOfConnectedRoute);
    printf("NumOfStation=%d\n", route->numOfStation);
}

void dump_tm(struct tm t)
{
    printf("%d/%d/%d %d:%d:%d\n", t.tm_mon, t.tm_mday, t.tm_year, t.tm_hour, t.tm_min, t.tm_sec );
}

void dump_tm_short(struct tm t)
{
    printf("%d/%d/%d\n", t.tm_mon, t.tm_mday, t.tm_year);
}




void dump_agency(agency_t* ag)
{
    int i, j;
    printf("[Agency]\n");
    
    printf("AgencyID=");
    for(i = 0; i < SIZE_OF_AGENCY_ID; i++)
        printf("%02d", ag->agencyID[i]);
    printf("\n");
    
    printf("AgencyGroupCode=");
    for(i = 0; i < SIZE_OF_AGENCY_GROUP_CODE; i++)
        printf("%02d", ag->agencyGroupCode[i]);
    printf("\n");
    
    printf("NumberOfRoutes=%d\n", ag->numberOfRoutes);
    
    for(i = 0; i < ag->numberOfRoutes; i++)
    {
        printf("RouteID ");
        for(j = 0; j < SIZE_OF_ROUTE_ID; j++)
            printf("%02d", ag->routeIDList[i*SIZE_OF_ROUTE_ID+j]);
        printf("\n");
    }
}

void dump_policy(farePolicy_t* policy)
{
    printf("[FarePlicy]\n");
    printf("FareType=%d\n", policy->fareType);
    printf("PassEnabled=%d\n", policy->passEnabled);
    printf("StoredValueEnabled=%d\n", policy->storedValueEnabled);
    printf("BankcardEnabled=%d\n", policy->bankcardEnabled);
    printf("SFPEnabled=%d\n", policy->sfpEnabled);
    printf("ZoneDiscountEnabled=%d\n", policy->zoneDiscountEnabled);
    printf("MinusBalanceEnabled=%d\n", policy->minusBalanceEnabled);
    printf("MinusBalanceLimit=%d\n", policy->minusLimit);

    printf("NumOfValidCardType=%d\n", policy->numOfValidCardType);
    dump_arr("ValidCardTypeList=", policy->validCardTypeList, 0, policy->numOfValidCardType);
    printf("NumOfValidCardStatus=%d\n", policy->numOfValidCardStatus);
    dump_arr("ValidCardStatus=", policy->validCardTypeList, 0, policy->numOfValidCardType);
    printf("NumOfValidAccountStatus=%d\n", policy->numOfAccountStatus);
    dump_arr("ValidAccountStatus=", policy->validAccountStatusList, 0, policy->numOfAccountStatus);
}

void dump_account(account_t* ac)
{
    printf("[Account]\n");
    dump_arr("CardID=", ac->cardID, 0, SIZE_OF_CARD_ID);
    dump_arr("AgencyGroupCode=", ac->agencyGroupCode, 0, SIZE_OF_AGENCY_GROUP_CODE);
    printf("AccountStatus=%d\n", ac->accountStatus);
    printf("CardStatus=%d\n", ac->cardStatus);
    printf("SFP=%d\n", ac->specialFareProgram);
    printf("Balance=%d\n", ac->balance);
}

void dump_pass(pass_t* pass)
{
    int i;
    printf("[Pass]\n");
    printf("PassType=%d\n", pass->passType);
    printf("StartDate=%d%d/%d/%d\n", pass->passStartDate[0],pass->passStartDate[1],pass->passStartDate[2],pass->passStartDate[3]);
    printf("ExpireDate=%d%d/%d/%d\n", pass->passExpireDate[0],pass->passExpireDate[1],pass->passExpireDate[2],pass->passExpireDate[3]);
    printf("TimeBasedPassAddTimeType=%d\n", pass->timeBasedPassAddTimeType);
    printf("TimeBasedPassAddTime=%d\n", pass->timeBasedPassAddTime);
    printf("NumOfTripBasedPass=%d\n", pass->numOfTripBasedPass);
    printf("ConfirmType=%d\n", pass->confirmType);
    dump_arr("ValidAgencyID=", pass->validAgencyID, 0, SIZE_OF_AGENCY_ID);
    dump_arr("ReligionCode=", pass->validReligionCode, 0, SIZE_OF_RELIGION_CODE);
    printf("NumOfValidZoneID=%d\n", pass->numOfValidZoneID);
    printf("ValidZoneIDList\n");
    for(i = 0; i < pass->numOfValidZoneID; i++)
    {
        dump_arr("     ", pass->validZoneIDList, i*SIZE_OF_ZONE_ID, SIZE_OF_ZONE_ID);
    }    
}

void dump_history(history_t* hist)
{
    printf("[History]\n");
    printf("HistoryType=%d\n", hist->historyType);
    dump_arr("StationID=", hist->stationID, 0, SIZE_OF_STATION_ID);
    dump_arr("AgencyID=", hist->agencyID, 0, SIZE_OF_AGENCY_ID);
    dump_arr("RouteID=", hist->routeID, 0, SIZE_OF_ROUTE_ID);
    dump_arr("ZoneID=", hist->zoneID, 0, SIZE_OF_ZONE_ID);
    printf("Timestamp=%02d/%02d/%02d %02d:%02d:%02d\n", hist->timestamp[0],hist->timestamp[1],hist->timestamp[2],hist->timestamp[3],hist->timestamp[4],hist->timestamp[5]);
    printf("RuteType=%d\n", hist->routeType);
    printf("Direction=%d\n", hist->direction);
    printf("PeakType=%d\n", hist->peakType);
    printf("ModeType=%d\n", hist->modeType);
    printf("PaymentType=%d\n", hist->paymentType);
    printf("PassbackCount=%d\n", hist->passbackCount);
}


void dump_transferData(transferData_t* td)
{
    printf("[Transfer Data]\n");
    dump_arr("StationRouteID=", td->startRouteID, 0, SIZE_OF_ROUTE_ID);
    printf("Timestamp=%02d/%02d/%02d %02d:%02d:%02d\n", td->timestamp[0], td->timestamp[1], td->timestamp[2], td->timestamp[3], td->timestamp[4], td->timestamp[5]);
    printf("StartDirection=%d\n", td->startDirection);
    printf("OriginalRouteType=%d\n", td->originalRouteType);
    printf("TransferCount=%d\n", td->transferCount);
}

void dump_responseData(response_t* res)
{
    printf("[Response]\n");
    printf("Open=%02d\n", res->open);
    printf("Fare=%d\n", res->fare);
    printf("MessageCode=%02d\n", res->messageCode);
    printf("ErrorCode=%02d\n", res->errorCode);
    printf("SFPCode=%02d\n", res->sfpCode);
    printf("PassExpireDate=%02d%02d/%02d/%02d\n", res->passExpireDate[0],res->passExpireDate[1], res->passExpireDate[2], res->passExpireDate[3]);
    printf("Balance=%d\n", res->balance);
    printf("NumOfTripBasedPass=%02d\n", res->numOfTripBasedPass);
}

void dump_fare(fare_t* fare)
{
    printf("[Fare]\n");
    dump_arr("ZoneID=", fare->zoneID, 0, SIZE_OF_ZONE_ID);
    printf("SFP=%d\n", fare->sfp);
    printf("TransferType=%d\n", fare->transferType);
    printf("PeaktimeCode=%d\n", fare->peaktimeCode);
    printf("FareValue=%d\n", fare->fareValue);
}


void dump_station(station_t* station)
{
    int i;
    printf("[Station]\n");
    dump_arr("StationID=", station->stationID, 0, SIZE_OF_STATION_ID);
    dump_arr("RouteID=", station->routeID, 0, SIZE_OF_ROUTE_ID);;
    dump_arr("AgencyID=", station->agencyID, 0, SIZE_OF_AGENCY_ID);
    dump_arr("ZoneID=", station->zone.zoneID, 0, SIZE_OF_ZONE_ID);
    dump_arr("PrevZoneID=", station->zone.prevZoneID, 0, SIZE_OF_ZONE_ID);
    dump_arr("NextZoneID=", station->zone.nextZoneID, 0, SIZE_OF_ZONE_ID);
    printf("TransferPoint=%02d\n", station->transferPoint);
    printf("NumOfConnectedRoute=%d\n", station->numOfConnectedRoutes);
    
    printf("ConnectedRouteIDList\n");
    for(i = 0; i < station->numOfConnectedRoutes; i++)
        dump_arr("     ", station->connectedRoutesList, i*SIZE_OF_ROUTE_ID, SIZE_OF_ROUTE_ID);
    printf("North=%d.%d\n", station->northA, station->northB);
    printf("East =%d.%d\n", station->eastA, station->eastB);
}


void dump_arr(char* title, u_int8* buf, int index, int length)
{
    int i;
    printf("%s", title);
    for(i = 0; i < length; i++)
        printf("%02d,", buf[i+index]);
    printf("\n");
}












