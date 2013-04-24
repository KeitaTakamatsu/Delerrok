//
//  Station.c
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/03/07.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//
/*
#include <stdio.h>
#include "Station.h"
#include "Util.h"


station_t st;
station_t getStationFromGPS(txn_t* txn, route_t* route)
{
    int result = SDBAssign(SDB_INDEX_STATION, makeStationIDFromGPS(txn, route));
    SDBRead(SDB_INDEX_STATION, 0, &st, sizeof(station_t));
    SDBRelease(SDB_INDEX_STATION);
    return st;
}

station_t getStation(u_int8* routeID, u_int8* stationID)
{
    int result = SDBAssign(SDB_INDEX_STATION, makeStationID(routeID, stationID));
    SDBRead(SDB_INDEX_STATION, 0, &st, sizeof(station_t));
    SDBRelease(SDB_INDEX_STATION);
    return st;
}


u_int8 tmpStationID[21];
/*
long long makeStationIDFromGPS(txn_t* txn, route_t* route)
{
    int idx = 0;
    tmpStationID[idx] = STATION_ID_TYPE_GPS; idx++;
    blockcopy(route->routeID, 0, tmpStationID, idx, SIZE_OF_ROUTE_ID); idx += SIZE_OF_ROUTE_ID;
    blockcopy(&txn->northA, 0, tmpStationID, idx, 2); idx += 2;
    blockcopy(&txn->northB, 0, tmpStationID, idx, 4); idx += 4;
    blockcopy(&txn->eastA, 0, tmpStationID, idx, 2);  idx += 2;
    blockcopy(&txn->eastB, 0, tmpStationID, idx, 4);  idx += 4;
    
    long long l = md5(tmpStationID, 21);
    dump_arr("ID=", &l, 0, 8);
    
    return l;
}
*/
/*
long long makeStationIDFromStation(station_t* station)
{
    int idx = 0;
    tmpStationID[idx] = STATION_ID_TYPE_GPS; idx++;    
    blockcopy(station->routeID, 0, tmpStationID, idx, SIZE_OF_ROUTE_ID); idx += SIZE_OF_ROUTE_ID;
    blockcopy(&station->northA, 0, tmpStationID, idx, 2); idx += 2;
    blockcopy(&station->northB, 0, tmpStationID, idx, 4); idx += 4;
    blockcopy(&station->eastA, 0, tmpStationID, idx, 2);  idx += 2;
    blockcopy(&station->eastB, 0, tmpStationID, idx, 4);  idx += 4;
    dump_arr("StationID=", tmpStationID, 0, 21);
    return md5(tmpStationID, 21);
}


long long makeStationID(u_int8* routeID, u_int8* stationID)
{
    int idx = 0;
    tmpStationID[idx] = STATION_ID_TYPE_DIRECT; idx++;
    blockcopy(routeID, 0, tmpStationID, idx, SIZE_OF_ROUTE_ID); idx += SIZE_OF_ROUTE_ID;
    blockcopy(stationID, 0, tmpStationID, idx, SIZE_OF_STATION_ID); idx += SIZE_OF_STATION_ID;
    return md5(tmpStationID, 21);
}


*/






















