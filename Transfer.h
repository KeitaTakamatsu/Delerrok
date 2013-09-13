//
//  Transfer.h
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/03/07.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#ifndef Delerrok_Transfer_h
#define Delerrok_Transfer_h

#include "structures.h"
#include "defines.h"

u_int8 transferFlat(txn_t* txn, agency_t* agency, route_t* route, account_t* account, station_t* station, u_int8* timestamp, transferData_t* newTransferData);
double timeLimitCheck(history_t* hist, transfer_t* transfer, u_int8* timestamp);
u_int8 checkConnectingRouteOnly(transfer_t* t, route_t* route, station_t* station, transferData_t* td);



#endif
