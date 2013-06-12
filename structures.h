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
    u_int8 agencyID[SIZE_OF_AGENCY_ID];
    u_int8 routeID[SIZE_OF_ROUTE_ID];
    u_int8 timestamp[SIZE_OF_YYMMDDHHMMSS]; // YYMMDDHHmmSS
    u_int8 cardID[SIZE_OF_CARD_ID];
    u_int8 cardType;
    u_int8 expireDate[SIZE_OF_YYYYMMDD]; 
    short northA;     // 整数部
    u_int32 northB;   // 小数点以下
    short eastA;      // 整数部
    u_int32 eastB;    // 小数点以下
    u_int8 from[SIZE_OF_STATION_ID];
    u_int8 to[SIZE_OF_STATION_ID];
} __attribute__ ((packed)) txn_t;


typedef struct
{
    u_int8 open;
    int fare;
    u_int8 messageCode;
    u_int8 errorCode;
    u_int8 sfpCode;
    u_int8 passExpireDate[SIZE_OF_YYYYMMDD]; // yyyymmdd
    int balance;
    u_int8 numOfTripBasedPass;
}__attribute__ ((packed)) response_t;


typedef struct
{
    u_int8 historyType;
    u_int8 stationID[SIZE_OF_STATION_ID];
    u_int8 agencyID[SIZE_OF_AGENCY_ID];
    u_int8 routeID[SIZE_OF_ROUTE_ID];
    u_int8 zoneID[SIZE_OF_ZONE_ID];
    u_int8 timestamp[SIZE_OF_YYMMDDHHMMSS];
    u_int8 routeType;
    u_int8 direction;
    u_int8 peakType;
    u_int8 modeType;
    u_int8 paymentType;
    u_int8 passbackCount;
} __attribute__ ((packed)) history_t;




typedef struct
{
    u_int8 passType;
    u_int8 passStartDate[SIZE_OF_YYYYMMDD];
    u_int8 passExpireDate[SIZE_OF_YYYYMMDD];
    u_int8 timeBasedPassAddTimeType;
    u_int16 timeBasedPassAddTime;
    u_int8 numOfTripBasedPass;
    u_int8 confirmType;
    u_int8 validAgencyID[SIZE_OF_AGENCY_ID];
    u_int8 validReligionCode[SIZE_OF_RELIGION_CODE];
    u_int8 numOfValidZoneID;
    u_int8 validZoneIDList[SIZE_OF_VALID_ZONE_LIST_FOR_PASS*SIZE_OF_ZONE_ID];
}__attribute__ ((packed)) pass_t;


typedef struct
{
    u_int8 startRouteID[SIZE_OF_ROUTE_ID];
    u_int8 timestamp[SIZE_OF_YYMMDDHHMMSS];
    u_int8 startDirection;
    u_int8 originalRouteType;
    u_int8 transferCount;
} __attribute__ ((packed)) transferData_t;


typedef struct
{
    u_int8 cardID[SIZE_OF_CARD_ID];
    u_int8 agencyGroupCode[SIZE_OF_AGENCY_GROUP_CODE];
    u_int8 accountStatus;
    u_int8 cardStatus;
    pass_t passList[SIZE_OF_PASS_LIST];
    u_int8 specialFareProgram;
    int balance;
    history_t lastHistory;
    transferData_t transferData;
} __attribute__ ((packed)) account_t;



typedef struct
{
    u_int8 zoneID[SIZE_OF_ZONE_ID];
    u_int8 prevZoneID[SIZE_OF_ZONE_ID];
    u_int8 nextZoneID[SIZE_OF_ZONE_ID];
} __attribute__ ((packed)) zone_t;


typedef struct
{
    u_int8 allowedTransfer;
    u_int8 sameRouteOnly;
    u_int8 notOnSameRoute;
    u_int8 maxTimePeriod;
    u_int16 maxMinutes;
    u_int8 oppositDirectionRestriction;
    u_int8 sameDirectionRestriction;
    u_int8 designatedStopsOnly;
    u_int8 numOfDesignatedStops;
    u_int8 designatedStops[SIZE_OF_STATION_ID * SIZE_OF_DESIGNATED_STOPS];
    u_int8 sameRouteType;
    u_int8 specifiedRouteCombinations;
    u_int8 numOfRouteCombination;
    u_int8 routeComtinationFrom[SIZE_OF_ROUTE_ID * SIZE_OF_ROUTE_COMBINATION];
    u_int8 routeComtinationTo[SIZE_OF_ROUTE_ID * SIZE_OF_ROUTE_COMBINATION];
    u_int8 numOfValidPaymentType;
    u_int8 validPaymentType[SIZE_OF_VALID_PAYMENT_TYPE_FOR_TRANSFER];
    u_int8 connectingRouteOnly;
    u_int8 directionRestriction;
    u_int8 freeFareTransfer;
    u_int8 freeFareTransferCount;
} __attribute__ ((packed)) transfer_t;


typedef struct
{
    u_int8 peakTimeCode;
	u_int8 week;
	u_int8 startHour;	
	u_int8 startMinutes;
	u_int8 endHour;     
	u_int8 endMinutes;	
}__attribute__ ((packed)) peaktime_t;


typedef struct
{
    u_int8 enabled;     
    u_int8 limitCount;	
    u_int16 limitSecond;
}__attribute__ ((packed)) passback_t;


typedef struct
{
    u_int8 fareType;
    u_int8 passEnabled;
    u_int8 storedValueEnabled;
    u_int8 bankcardEnabled;
    u_int8 sfpEnabled;
    u_int8 zoneDiscountEnabled;
    u_int8 minusBalanceEnabled;
    u_int16 minusLimit;
    u_int8 numOfValidCardType;
    u_int8 validCardTypeList[SIZE_OF_VALID_CARD_TYPE_LIST];
    u_int8 numOfValidCardStatus;
    u_int8 validCardStatusList[SIZE_OF_VALID_CARD_STATUS_LIST];
    u_int8 numOfAccountStatus;
    u_int8 validAccountStatusList[SIZE_OF_VALID_ACCOUNT_STATUS_LIST];
    transfer_t transfer;
    passback_t passback;
    u_int8 numOfPeakTime;
    peaktime_t peaktimeList[SIZE_OF_PEAKTIME_LIST];
} __attribute__ ((packed)) farePolicy_t;


typedef struct
{
    u_int8 stationID[SIZE_OF_STATION_ID];
    u_int8 routeID[SIZE_OF_ROUTE_ID];
    u_int8 agencyID[SIZE_OF_AGENCY_ID];
    zone_t zone;
    u_int8 transferPoint;
    u_int8 numOfConnectedRoutes;
    u_int8 connectedRoutesList[SIZE_OF_ROUTE_ID * SIZE_OF_CONNECTED_ROUTE_LIST];
    short northA;
    u_int32 northB;
    short eastA;
    u_int32 eastB;
} __attribute__ ((packed)) station_t;


typedef struct
{
    u_int8 routeID[SIZE_OF_ROUTE_ID];
    u_int8 agencyID[SIZE_OF_AGENCY_ID];
    u_int8 modeType;
    u_int8 routeType;
    u_int8 direction;
    u_int8 validDirection;
    u_int8 numOfConnectedRoute;
    u_int8 connectedRouteList[SIZE_OF_ROUTE_ID*SIZE_OF_CONNECTED_ROUTE_LIST];
    u_int16 numOfStation;
    u_int8 stationIDList[SIZE_OF_STATION_ID*SIZE_OF_STATION_LIST];
} __attribute__ ((packed)) route_t;



typedef struct
{
    u_int8 zoneID[SIZE_OF_ZONE_ID];
    u_int8 sfp;
    u_int8 transferType;
    u_int8 peaktimeCode;
    int fareValue;
}__attribute__ ((packed)) fare_t;



typedef struct
{
    u_int8 agencyID[SIZE_OF_AGENCY_ID];
    u_int8 agencyGroupCode[SIZE_OF_AGENCY_GROUP_CODE];
    u_int16 numberOfRoutes;
    u_int8 routeIDList[SIZE_OF_ROUTE_ID*SIZE_OF_ROUTE_LIST];
    farePolicy_t policy;
} __attribute__ ((packed)) agency_t;



typedef struct
{
    u_int16 formatType;
    u_int16 length;
    u_int8 sdbindex;
    u_int8 sdbID[8];
    u_int8 data[10000];
} __attribute__ ((packed)) sdbtxn_t;



















#endif
