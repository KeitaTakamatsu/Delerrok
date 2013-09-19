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
    uint8_t agencyID[SIZE_OF_AGENCY_ID];
    uint8_t routeID[SIZE_OF_ROUTE_ID];
    uint8_t timestamp[SIZE_OF_YYMMDDHHMMSS]; // YYMMDDHHmmSS
    uint8_t cardID[SIZE_OF_CARD_ID];
    uint8_t cardType;
    uint8_t expireDate[SIZE_OF_YYYYMMDD]; 
    short northA;     // 整数部
    uint32_t northB;   // 小数点以下
    short eastA;      // 整数部
    uint32_t eastB;    // 小数点以下
    uint8_t from[SIZE_OF_STATION_ID];
    uint8_t to[SIZE_OF_STATION_ID];
} __attribute__ ((packed)) txn_t;


typedef struct
{
    uint8_t open;
    int fare;
    uint8_t messageCode;
    uint8_t errorCode;
    uint8_t sfpCode;
    uint8_t passExpireDate[SIZE_OF_YYMMDDHHMMSS]; // yyyymmdd
    int balance;
    uint8_t numOfTripBasedPass;
}__attribute__ ((packed)) response_t;


typedef struct
{
    uint8_t historyType;
    uint8_t stationID[SIZE_OF_STATION_ID];
    uint8_t agencyID[SIZE_OF_AGENCY_ID];
    uint8_t routeID[SIZE_OF_ROUTE_ID];
    uint8_t zoneID[SIZE_OF_ZONE_ID];
    uint8_t timestamp[SIZE_OF_YYMMDDHHMMSS];
    uint8_t routeType;
    uint8_t direction;
    uint8_t peakType;
    uint8_t modeType;
    uint8_t paymentType;
    uint8_t passbackCount;
} __attribute__ ((packed)) history_t;



typedef struct
{
    uint8_t passType;
    uint8_t passStartDate[SIZE_OF_YYMMDDHHMMSS];
    uint8_t passExpireDate[SIZE_OF_YYMMDDHHMMSS];
    uint8_t timeBasedPassRenewalUnits;
    uint16_t timeBasedPassNumberOfRenewalUnits;
    uint8_t numOfTripBasedPass;
    uint8_t numOfValidAgencies;
    uint8_t validAgencyIDList[SIZE_OF_AGENCY_ID*SIZE_OF_VALID_AGENCY_LIST_FOR_PASS];
    uint8_t numOfValidZoneID;
    uint8_t validZoneIDList[SIZE_OF_VALID_ZONE_LIST_FOR_PASS*SIZE_OF_ZONE_ID];
    uint8_t passDiscountType;
}__attribute__ ((packed)) pass_t;


typedef struct
{
    uint8_t startRouteID[SIZE_OF_ROUTE_ID];
    uint8_t timestamp[SIZE_OF_YYMMDDHHMMSS];
    uint8_t startDirection;
    uint8_t originalRouteType;
    uint8_t transferCount;
} __attribute__ ((packed)) transferData_t;


typedef struct
{
    uint8_t cardID[SIZE_OF_CARD_ID];
    uint8_t agencyGroupCode[SIZE_OF_AGENCY_GROUP_CODE];
    uint8_t accountStatus;
    uint8_t cardStatus;
    uint8_t specialFareProgram;
    int balance;
    history_t lastHistory;
    transferData_t transferData;
    pass_t passList[SIZE_OF_PASS_LIST];
} __attribute__ ((packed)) account_t;



typedef struct
{
    uint8_t zoneID[SIZE_OF_ZONE_ID];
    uint8_t prevZoneID[SIZE_OF_ZONE_ID];
    uint8_t nextZoneID[SIZE_OF_ZONE_ID];
} __attribute__ ((packed)) zone_t;


typedef struct
{
    uint8_t allowedTransfer;
    uint8_t sameRouteOnly;
    uint8_t maxTimePeriod;
    uint16_t maxMinutes;
    uint8_t designatedStopsOnly;
    uint8_t numOfDesignatedStops;
    uint8_t designatedStops[SIZE_OF_STATION_ID * SIZE_OF_DESIGNATED_STOPS];
    uint8_t sameRouteType;
    uint8_t specifiedRouteCombinations;
    uint8_t numOfRouteCombination;
    uint8_t routeComtinationFrom[SIZE_OF_ROUTE_ID * SIZE_OF_ROUTE_COMBINATION];
    uint8_t routeComtinationTo[SIZE_OF_ROUTE_ID * SIZE_OF_ROUTE_COMBINATION];
    uint8_t numOfValidCardType;
    uint8_t validCardTypes[SIZE_OF_VALID_PAYMENT_TYPE_FOR_TRANSFER];
    uint8_t connectingRouteOnly;
    uint8_t directionRestriction;
    uint8_t numberOfValidDirectionCombination;
    uint8_t originalDirectionOfTravel[SIZE_OF_DIRECTION_LIST];
    uint8_t transferDirectionOfTravel[SIZE_OF_DIRECTION_LIST];
    uint8_t freeFareTransfer;
    uint8_t freeFareTransferCount;
    uint8_t transferLimitCount;
} __attribute__ ((packed)) transfer_t;


typedef struct
{
    uint8_t peakTimeCode;
	uint8_t week;
	uint8_t startHour;	
	uint8_t startMinutes;
	uint8_t endHour;     
	uint8_t endMinutes;	
}__attribute__ ((packed)) peaktime_t;


typedef struct
{
    uint8_t enabled;     
    uint8_t limitCount;	
    uint16_t limitSecond;
}__attribute__ ((packed)) passback_t;


typedef struct
{
    uint8_t fareType;
    uint8_t passEnabled;
    uint8_t storedValueEnabled;
    uint8_t bankcardEnabled;
    uint8_t sfpEnabled;
    uint8_t zoneDiscountEnabled;
    uint8_t minusBalanceEnabled;
    uint16_t minusLimit;
    uint8_t numOfValidCardType;
    uint8_t validCardTypeList[SIZE_OF_VALID_CARD_TYPE_LIST];
    uint8_t numOfValidCardStatus;
    uint8_t validCardStatusList[SIZE_OF_VALID_CARD_STATUS_LIST];
    uint8_t numOfAccountStatus;
    uint8_t validAccountStatusList[SIZE_OF_VALID_ACCOUNT_STATUS_LIST];
    transfer_t transfer;
} __attribute__ ((packed)) farePolicy_t;


typedef struct
{
    uint8_t stationID[SIZE_OF_STATION_ID];
    uint8_t routeID[SIZE_OF_ROUTE_ID];
    uint8_t agencyID[SIZE_OF_AGENCY_ID];
    zone_t zone;
    uint8_t transferPoint;
    uint8_t numOfConnectedRoutes;
    uint8_t connectedRoutesList[SIZE_OF_ROUTE_ID * SIZE_OF_CONNECTED_ROUTE_LIST];
    short northA;
    uint32_t northB;
    short eastA;
    uint32_t eastB;
} __attribute__ ((packed)) station_t;


typedef struct
{
    uint8_t routeID[SIZE_OF_ROUTE_ID];
    uint8_t agencyID[SIZE_OF_AGENCY_ID];
    uint8_t modeType;
    uint8_t routeType;
    uint8_t direction;
    uint8_t specialDiscount;
    uint8_t numOfConnectedRoute;
    uint8_t connectedRouteList[SIZE_OF_ROUTE_ID*SIZE_OF_CONNECTED_ROUTE_LIST];
    uint8_t numOfStation;
    uint8_t stationIDList[SIZE_OF_STATION_ID*SIZE_OF_STATION_LIST];
    uint8_t gpsLocationList[SIZE_OF_STATION_LIST*12];
} __attribute__ ((packed)) route_t;



typedef struct
{
    uint8_t agencyID[SIZE_OF_AGENCY_ID];
    uint8_t zoneID[SIZE_OF_ZONE_ID];
    uint8_t sfp;
    uint8_t transferType;
    uint8_t peaktimeCode;
    uint8_t passDiscount;
    int fareValue;
}__attribute__ ((packed)) fare_t;



typedef struct
{
    uint8_t agencyID[SIZE_OF_AGENCY_ID];
    uint8_t agencyGroupCode[SIZE_OF_AGENCY_GROUP_CODE];
    uint16_t numberOfRoutes;
    uint8_t routeIDList[SIZE_OF_ROUTE_ID*SIZE_OF_ROUTE_LIST];
    farePolicy_t policy;
} __attribute__ ((packed)) agency_t;



typedef struct
{
    uint16_t formatType;
    uint16_t length;
    uint8_t sdbindex;
    uint8_t sdbID[8];
    uint32_t dataIndex;
    uint8_t data[10000];
} __attribute__ ((packed)) sdbtxn_t;








/* For Task Transfer */
typedef struct
{
    uint16_t formatType;
    uint16_t length;
    uint8_t data[100];
} __attribute__ ((packed)) ttfer_t;










#endif
