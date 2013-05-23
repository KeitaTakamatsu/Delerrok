//
//  StoredValue.h
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/28.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#ifndef Delerrok_StoredValue_h
#define Delerrok_StoredValue_h

#include <stdio.h>
#include "defines.h"
#include "structures.h"



long long makeFlatFareID(u_int8* zoneID, u_int8 sfp, u_int8 transferType, u_int8 peaktimeCode);
long long makeFlatFareIDFromFare(fare_t* fare);
response_t storedValueFlat(txn_t* txn, account_t* account, agency_t* agency, route_t* route, station_t* station);


#endif
