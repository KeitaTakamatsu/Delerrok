//
//  TransitApp.h
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/26.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#ifndef Delerrok_TransitApp_h
#define Delerrok_TransitApp_h


#include "../structures.h"
#include "../defines.h"


account_t beforeAccount, afterAccount;
history_t newhistoryForDebug;
station_t stationForDebug;

response_t app(txn_t* txn, account_t* account, agency_t* agency, route_t* route);
response_t flatfare(txn_t* txn, account_t* account, agency_t* agency, route_t* route);
uint8_t checkTxnData(txn_t* txn, farePolicy_t* policy);
void init();
uint64_t makeFlatFareID(uint8_t* agencyID ,uint8_t* zoneID, uint8_t sfp, uint8_t transferType, uint8_t peaktimeCode, uint8_t );
uint64_t makeStationIDFromGPS2(txn_t* txn, route_t* r);
uint64_t gpsCompValue(txn_t* txn, station_t* st);
uint64_t gpsCompValue2(txn_t* txn, route_t* r, int index);

#endif
