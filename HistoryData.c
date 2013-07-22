//
//  HistoryData.c
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/28.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#include "HistoryData.h"

history_t hist;
history_t makeHistoryData(u_int8* agencyID, u_int8 historyType, route_t* route, station_t* station, u_int8* timestamp,
                          u_int8 paymentType)
{
    hist.historyType = historyType;
    memcpy(hist.agencyID, agencyID, SIZE_OF_AGENCY_ID);
    memcpy(hist.routeID, route->routeID, SIZE_OF_ROUTE_ID);
    hist.modeType = route->modeType;
    hist.paymentType = paymentType;
    hist.routeType = route->routeType;
    memcpy(hist.timestamp, timestamp, 6);
    memcpy(hist.stationID, station->stationID, SIZE_OF_STATION_ID);
    memcpy(hist.zoneID, station->zone.zoneID, SIZE_OF_ZONE_ID);
    return hist;
}