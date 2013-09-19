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

uint8_t transferFlat(txn_t* txn, agency_t* agency, route_t* route, account_t* account, station_t* station, uint8_t* timestamp, transferData_t* newTransferData);
double timeLimitCheck(history_t* hist, transfer_t* transfer, uint8_t* timestamp);
uint8_t checkConnectingRouteOnly(transfer_t* t, route_t* route, station_t* station, transferData_t* td);



#endif
