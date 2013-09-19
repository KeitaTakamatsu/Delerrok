//
//  Transfer.c
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/03/07.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#include <time.h>
#include "Transfer.h"
#include "HistoryData.h"
#include "StoredValue.h"
#include "Account.h"
#include "Util.h"
#include "CloudApp/CloudApp.h"
#include "CloudApp/CORE2.h"



uint8_t checkSameRoute(transfer_t* t, route_t* route, transferData_t* td);
uint8_t checkMaxTimePeriod(transfer_t* t, uint8_t* timestamp, transferData_t* td);
uint8_t checkNotOnSameRoute(transfer_t* t, route_t* route, transferData_t* td);
uint8_t checkDesignateStopsOnly(transfer_t* t, station_t* station);
uint8_t checkValidPaymentType(transfer_t* t, txn_t* txn);
uint8_t checkConnectingRouteOnly(transfer_t* t, route_t* route, station_t* station, transferData_t* td);
uint8_t checkSpecifiedRouteCombinations(transfer_t* t, route_t* route, history_t* hist);
uint8_t checkDirection(transfer_t* t, route_t* route, transferData_t* td);
uint8_t checkTransferLimit(transfer_t* t, transferData_t* td);


/*
    This method return Transfer Type.
    Transfer is used to Fare ID.
*/
uint8_t makeTransferType(transfer_t* t, route_t* route, transferData_t* td)
{
    if(t->freeFareTransfer)
    {
        if(td->transferCount >= t->freeFareTransferCount)
            return TRANSFER_TYPE_FREE;
    }
    
    if(route->routeType == td->originalRouteType)
        return TRANSFER_TYPE_NORMAL;
    if(route->routeType == ROUTE_TYPE_REGULAR && td->originalRouteType == ROUTE_TYPE_EXPRESS)
        return TRANSFER_TYPE_EXPRESS_TO_REGULER;
    if(route->routeType == ROUTE_TYPE_EXPRESS && td->originalRouteType == ROUTE_TYPE_REGULAR)
        return TRANSFER_TYPE_REGULAR_TO_EXPRESS;
        
    return TRANSFER_TYPE_NORMAL;
}

/* This method return Transfer Type.*/
uint8_t transferFlat(txn_t* txn, agency_t* agency, route_t* route, account_t* account, station_t* station, uint8_t* timestamp, transferData_t* newTransferData)
{
    transfer_t* t = &agency->policy.transfer;
    transferData_t* td = &account->transferData;
    dump_station(station);
    dump_route(route);
    dump_transferData(&account->transferData);    
    
    if(!t->allowedTransfer)
        return NO_TRANSFER;
    if(!checkMaxTimePeriod(t, timestamp, td))
        goto NO_TRANSFER_EXIT;
    if(!checkValidPaymentType(t, txn))
        goto NO_TRANSFER_EXIT;
    if(!checkDirection(t, route, td))
        goto NO_TRANSFER_EXIT;
    if(!checkDesignateStopsOnly(t, station))
        goto NO_TRANSFER_EXIT;
    if(!checkConnectingRouteOnly(t, route,station, td))
        goto NO_TRANSFER_EXIT;
    
TRANSFER_SUCCESS_EXIT:
    newTransferData->transferCount++;
    return makeTransferType(t, route, td);
    
NO_TRANSFER_EXIT:
    newTransferData->originalRouteType = route->routeType;
    newTransferData->startDirection = route->direction;
    blockcopy(route->routeID, 0, newTransferData->startRouteID, 0, SIZE_OF_ROUTE_ID);
    blockcopy(txn->timestamp, 0, newTransferData->timestamp, 0, SIZE_OF_YYMMDDHHMMSS);
    newTransferData->transferCount = 0;
    return NO_TRANSFER;
}

uint8_t checkTransferLimit(transfer_t* t, transferData_t* td)
{
    if(t->transferLimitCount == 0)
        return 1;
    
    if(td->transferCount >= t->transferLimitCount)
        return 0;
    
    return 1;
}




uint8_t checkDirection(transfer_t* t, route_t* route, transferData_t* td)
{
    if(!t->directionRestriction)
        return 1;
    
    int i;
    for(i = 0; i < t->numberOfValidDirectionCombination; i++)
    {
        if(td->startDirection != t->originalDirectionOfTravel[i])
            continue;
        if(route->direction != t->transferDirectionOfTravel[i])
            continue;
        return 1;
    }    
    return 0;
}

uint8_t checkSameRoute(transfer_t* t, route_t* route, transferData_t* td)
{
    if(!t->sameRouteOnly)
        return 0;
    
    if (compare(route->routeID, 0, td->startRouteID, 0, SIZE_OF_ROUTE_ID))
        return 1;
    
    return 0;
}

uint8_t checkNotOnSameRoute(transfer_t* t, route_t* route, transferData_t* td)
{
    if(!t->sameRouteOnly)
        return 1;
    
    if(compare(route->routeID, 0, td->startRouteID, 0, SIZE_OF_ROUTE_ID))
        return 0;
    
    return 1;
}

uint8_t checkMaxTimePeriod(transfer_t* t, uint8_t* timestamp, transferData_t* td)
{
    if(!t->maxTimePeriod)
        return 1;
    
    struct tm now = makeTimeYYMMDDHHmmSS(timestamp);
    struct tm transferDataTimestamp = makeTimeYYMMDDHHmmSS(td->timestamp);
    struct tm period = addMin(transferDataTimestamp, t->maxMinutes);
    
    time_t now_t = mktime(&now);
    time_t period_t = mktime(&period);
    
    double diff = difftime(period_t, now_t);
    
    if(diff < 0)
        return 0;
    
    return 1;
}

uint8_t checkDesignateStopsOnly(transfer_t* t, station_t* station)
{
    if(!t->designatedStopsOnly)
        return 1;
    
    int i;
    for(i = 0; i < t->numOfDesignatedStops; i++)
    {
        if(compare(t->designatedStops, i*SIZE_OF_STATION_ID, station->stationID, 0, SIZE_OF_STATION_ID))
            return 1;
    }
    return 0;
}

uint8_t checkSameRouteType(transfer_t* t, route_t* route, transferData_t* td)
{
    if(!t->sameRouteType)
        return 1;
    
    if(route->routeType != td->originalRouteType)
        return 0;
    
    
    return 1;
}

uint8_t checkSpecifiedRouteCombinations(transfer_t* t, route_t* route, history_t* hist)
{
    if(!t->specifiedRouteCombinations)
        return 1;
    
    int i;
    for(i = 0; i < t->numOfRouteCombination; i++)
    {
        if(!compare(hist->routeID, 0, t->routeComtinationFrom, i*SIZE_OF_ROUTE_ID, SIZE_OF_ROUTE_ID))
            continue;
        if(!compare(route->routeID, 0, t->routeComtinationTo, i*SIZE_OF_ROUTE_ID, SIZE_OF_ROUTE_ID))
            continue;
        return 1;
    }
    return 0;
}

/* putting on hold */
uint8_t checkValidPaymentType(transfer_t* t, txn_t* txn)
{
    // 0 = All Card Type is OK.
    if(!t->numOfValidCardType == 0)
        return 1;
    
    int i;
    for(i = 0; i < t->numOfValidCardType; i++)
    {
        if(txn->cardType == t->numOfValidCardType)
            return 1;
    }
    
    return 0;
}

uint8_t checkConnectingRouteOnly(transfer_t* t, route_t* route, station_t* station, transferData_t* td)
{
    if(!t->connectingRouteOnly)
        return 1;
    
    int i;
    dump_arr("HistoryRoute=", td->startRouteID, 0, SIZE_OF_ROUTE_ID);
    switch(t->connectingRouteOnly)
    {
        case CHECK_TYPE_CONNECTED_ROUTE:
            for(i = 0; i < route->numOfConnectedRoute; i++)
            {
                if(compare(route->connectedRouteList, i*SIZE_OF_ROUTE_ID, td->startRouteID, 0, SIZE_OF_ROUTE_ID))
                    return 1;
            }
            break;
        case CHECK_TYPE_CONNECTED_STATION:
            for(i = 0; i < station->numOfConnectedRoutes; i++)
            {
                dump_arr("ConnectedRoute=", station->connectedRoutesList, i*SIZE_OF_ROUTE_ID, SIZE_OF_ROUTE_ID);
                if(compare(station->connectedRoutesList, i*SIZE_OF_ROUTE_ID, td->startRouteID, 0, SIZE_OF_ROUTE_ID))
                    return 1;
            }
            break;
    }
    return 0;
}



double timeLimitCheck(history_t* hist, transfer_t* transfer, uint8_t* timestamp)
{
    
    struct tm now = makeTimeYYMMDDHHmmSS(timestamp);
    struct tm histTime = makeTimeYYMMDDHHmmSS(hist->timestamp);
    struct tm limit = addMin(histTime, transfer->maxMinutes);
    
    double d = datetimeCompare(now, limit);
    
#ifdef CONSOLE
    printf("now=%02d:%02d   hist=%02d:%02d   lim=%02d:%02d\n", now.tm_hour, now.tm_min, histTime.tm_hour, histTime.tm_min, limit.tm_hour, limit.tm_min);
    printf("%f\n", d);
#endif
    return datetimeCompare(now, limit);
     
    return 0;
}




