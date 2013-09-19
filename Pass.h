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

int getPass(txn_t* txn, agency_t* agency, account_t* account, station_t* from, station_t* to, int* hasPass, int* passNumber, uint8_t spDiscount);
int passCheckFlat(txn_t* txn, farePolicy_t* policy, station_t* from, account_t* account, pass_t* pass, uint8_t spDiscount);
int checkPassValid(pass_t* pass, uint8_t* agencyID, uint8_t* routeID, uint8_t* zoneID, uint8_t* timestamp, uint8_t validPassDiscount);
int checkValidZone(pass_t* pass, uint8_t* zoneID);
int checkAgencyID(pass_t* pass, uint8_t* agencyID);
int passCheck(txn_t* txn, farePolicy_t* policy, station_t* from, station_t* to, account_t* account, pass_t* pass, uint8_t spDiscount);
void passProcess(int passCheckResult, txn_t* txn, account_t* account, pass_t* pass, route_t* route,
                 station_t* currentStation, uint8_t transfer, uint8_t transferCount, response_t* response);
response_t passProcessFlat(uint8_t* agencyID, int passCheckResult, txn_t* txn, account_t* account, pass_t* pass, route_t* route ,station_t* station, uint8_t transfer, uint8_t transferCount);
struct tm* getActivatedTimeBasedPassTime(pass_t* pass, uint8_t* timestamp);
void timeBasedPassActivate(pass_t* pass, struct tm now, struct tm newTime);

#endif


