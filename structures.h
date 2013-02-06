//
//  structures.h
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/05.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#ifndef Delerrok_structures_h
#define Delerrok_structures_h

#include "defines.h"

typedef struct
{
    u_int8 appCode;
    u_int8 agencyID[SIZE_OF_AGENCY_ID];
    u_int8 routeID[SIZE_OF_ROUTE_ID];
    u_int32 timestamp;
    u_int64 cardID;
    u_int8 cardType;
    u_int16 expireDate;
    int north;
    int east;
    u_int8 from[SIZE_OF_STATION_ID];
    u_int8 to[SIZE_OF_STATION_ID];
} __attribute__ ((packed)) txn_t;


typedef struct
{
    u_int8 appCode;
    u_int8 open;
    short fare;
    u_int8 messageCode;
    u_int8 errorCode;
    u_int8 sfpCode;
    u_int16 passExpireDate;
    short balance;
}__attribute__ ((packed)) response_t;



typedef struct
{
    u_int8 passType;
    u_int32 passStartDate;
    u_int32 passExpireDate;
    u_int8 timeBasedPassAddTimeType;
    u_int16 timeBasedPassAddTime;
    u_int8 numOfTripBasedPass;
    u_int8 validAgencyID[SIZE_OF_AGENCY_ID];
    u_int8 validRouteID[SIZE_OF_ROUTE_ID];
    u_int8 validZoneIDList[SIZE_OF_VALID_ZONE_LIST][SIZE_OF_ZONE_ID];
}__attribute__ ((packed)) pass_t;


typedef struct{
    u_int8 zoneID[SIZE_OF_ZONE_ID];
    u_int8 prevZoneID[SIZE_OF_ZONE_ID];
    u_int8 nextZoneID[SIZE_OF_ZONE_ID];
} __attribute__ ((packed)) zone_t;






























#endif
