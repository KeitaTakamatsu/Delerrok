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


void blockcopy(uint8_t* src, int srcidx, uint8_t* dst, int dstidx, int len);
long long md5(char* data, int len);
int compare(uint8_t* a, int idx_a, uint8_t* b, int idx_b, int len);
struct tm makeTimeYYYYMMDD(uint8_t* timestmap);
struct tm makeTimeYYMMDDHHmmSS(uint8_t* timestmap);
void makeYYYYMMDD(struct tm timestamp, uint8_t* time);
void makeYYMMDDHHmmSSFromTime(struct tm timestamp, uint8_t* time);
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
void dump_arr(char* title, uint8_t* buf, int index, int length);
uint16_t toUInt16(uint8_t* buf, int index);
uint32_t toUInt32(uint8_t* buf, int index);
uint64_t toUInt64(uint8_t* buf, int index);
int16_t toInt16(uint8_t* buf, int index);
int32_t toInt32(uint8_t* buf, int index);
int64_t toInt64(uint8_t* buf, int index);


#endif
