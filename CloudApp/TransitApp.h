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
BOOL checkTxnData(txn_t* txn, farePolicy_t* policy);
void init();
u_int64 makeFlatFareID(u_int8* agencyID ,u_int8* zoneID, u_int8 sfp, u_int8 transferType, u_int8 peaktimeCode, u_int8 );
u_int64 makeStationIDFromGPS2(txn_t* txn, route_t* r);
u_int64 gpsCompValue(txn_t* txn, station_t* st);
u_int64 gpsCompValue2(txn_t* txn, route_t* r, int index);

#endif
