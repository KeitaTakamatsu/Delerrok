//
//  Util.h
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/28.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#ifndef Delerrok_Util_h
#define Delerrok_Util_h
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "structures.h"
#include "defines.h"
#include "md5.h"


void blockcopy(u_int8* src, int srcidx, u_int8* dst, int dstidx, int len);
long long md5(char* data, int len);
int compare(u_int8* a, int idx_a, u_int8* b, int idx_b, int len);
struct tm makeTimeYYYYMMDD(u_int8* timestmap);
struct tm makeTimeYYMMDDHHmmSS(u_int8* timestmap);
void makeYYYYMMDD(struct tm timestamp, u_int8* time);
void makeYYMMDDHHmmSSFromTime(struct tm timestamp, u_int8* time);
double datetimeCompare(struct tm t1, struct tm t2);
double datetimeCompareShort(struct tm t1, struct tm t2);
struct tm getNow();
struct tm addTime(struct tm src, int addYear, int addMonth, int addDays, int addHour);
struct tm addMin(struct tm src, int min);
void dump_tm_short(struct tm t);
void dump_tm(struct tm t);;
void dump_txn(txn_t* txn);
void dump_transferData(transferData_t* td);
void dump_account(account_t* account);
void dump_pass(pass_t* pass);
void dump_route(route_t* route);
void dump_history(history_t* hist);
void dump_transferData(transferData_t* transerData);
void dump_agency(agency_t* ag);
void dump_responseData(response_t* res);
void dump_policy(farePolicy_t* policy);
void dump_station(station_t* station);
void dump_transfer(transfer_t* tr);
void dump_arr(char* title, u_int8* buf, int index, int length);
u_int16 toUInt16(u_int8* buf, int index);
u_int32 toUInt32(u_int8* buf, int index);
u_int64 toUInt64(u_int8* buf, int index);
int16 toInt16(u_int8* buf, int index);
int32 toInt32(u_int8* buf, int index);
int64 toInt64(u_int8* buf, int index);


#endif
