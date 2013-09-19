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



uint64_t makeFlatFareID(uint8_t* agencyID, uint8_t* zoneID, uint8_t sfp, uint8_t transferType, uint8_t peaktimeCode, uint8_t passDiscount);
uint64_t makeFlatFareIDFromFare(fare_t* fare);
response_t storedValueFlat(txn_t* txn, account_t* account, agency_t* agency, route_t* route, station_t* station, uint8_t spDiscount);


#endif


