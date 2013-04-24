//
//  Pass.h
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/26.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#ifndef Delerrok_Pass_h
#define Delerrok_Pass_h
#include "structures.h"
#include "defines.h"

#define RESULT_INVALID 0
#define RESULT_VALID 1
#define RESULT_UPDATE_TIME 2
#define RESULT_UPDATE_TRIP 3

int getPass(txn_t* txn, agency_t* agency, account_t* account, station_t* from, station_t* to, int* hasPass, int* passNumber);


int passCheckFlat(txn_t* txn, farePolicy_t* policy, station_t* from, account_t* account, pass_t* pass);
int checkPassValid(pass_t* pass, u_int8* agencyID, u_int8* routeID, u_int8* zoneID, u_int8* timestamp);
int checkValidZone(pass_t* pass, u_int8* zoneID);
int passCheck(txn_t* txn, farePolicy_t* policy, station_t* from, station_t* to, account_t* account, pass_t* pass);
void passProcess(int passCheckResult, txn_t* txn, account_t* account, pass_t* pass, route_t* route,
                 station_t* currentStation, u_int8 transfer, u_int8 transferCount, response_t* response);
response_t passProcessFlat(int passCheckResult, txn_t* txn, account_t* account, pass_t* pass, route_t* route ,station_t* station, u_int8 transfer, u_int8 transferCount);
struct tm* getActivatedTimeBasedPass(pass_t* pass, u_int8* timestamp);
void timeBasedPassActivate(pass_t* pass, struct tm now, struct tm newTime);


#endif


