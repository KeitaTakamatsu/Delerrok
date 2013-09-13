//
//  defines.h
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/05.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#include <stdint.h>

#ifndef Delerrok_defines_h
#define Delerrok_defines_h

#define DEBUG_CONSOLE
#define DEBUG_MODE


#define SIZE_OF_CARD_ID 8
#define SIZE_OF_RELIGION_CODE 8
#define SIZE_OF_AGENCY_ID 8
#define SIZE_OF_ROUTE_ID 8
#define SIZE_OF_STATION_ID 8
#define SIZE_OF_ZONE_ID 8
#define SIZE_OF_AGENCY_GROUP_CODE 2
#define SIZE_OF_VALID_ZONE_LIST_FOR_PASS 10
#define SIZE_OF_VALID_AGENCY_LIST_FOR_PASS 50
#define SIZE_OF_VALID_CARD_TYPE_LIST 10
#define SIZE_OF_VALID_ROUTE_LIST 10
#define SIZE_OF_VALID_CARD_LIST 10
#define SIZE_OF_VALID_MODE_LIST 10
#define SIZE_OF_VALID_ROUTE_LIST_FOR_PASS 10
#define SIZE_OF_PEAKTIME_LIST 5
#define SIZE_OF_VALID_ACCOUNT_STATUS_LIST 10
#define SIZE_OF_VALID_CARD_STATUS_LIST 10
#define SIZE_OF_HISTORY_LIST 10
#define SIZE_OF_STATION_LIST 200
#define SIZE_OF_ROUTE_LIST 500
#define SIZE_OF_CONNECTED_ROUTE_LIST 20
#define SIZE_OF_PASS_LIST 5
#define SIZE_OF_YYMMDDHHMMSS 6
#define SIZE_OF_YYYYMMDD 4
#define SIZE_OF_DESIGNATED_STOPS 20
#define SIZE_OF_ROUTE_COMBINATION 20
#define SIZE_OF_VALID_PAYMENT_TYPE_FOR_TRANSFER 10
#define SIZE_OF_DIRECTION_LIST 12

// SDB
#define SDB_INDEX_ACCOUNT  0
#define SDB_INDEX_AGENCY  1
#define SDB_INDEX_ROUTE   2
#define SDB_INDEX_STATION 3
#define SDB_INDEX_FARE    4



#define AGENCY_ID_OFFSET 4
#define CARD_ID_OFFSET 26
#define ROUTE_ID_OFFSET 12


// WEEK
#define WEEK_SUNDAY 0x01
#define WEEK_MONDAY 0x02
#define WEEK_TUESDAY 0x04
#define WEEK_WEDNESDAY 0x08
#define WEEK_THURSDAY 0x10
#define WEEK_FRIDAY 0x20
#define WEEK_SATURDAY 0x40

// FareType
#define FARE_TYPE_FLAT 1
#define FARE_TYPE_ZONE 2
#define FARE_TYPE_DISTANCE 3

#define TBPASS_ACTIVATE_TYPE_NONE 0
#define TBPASS_ACTIVATE_TYPE_HOUR 1
#define TBPASS_ACTIVATE_TYPE_DAYS 2
#define TBPASS_ACTIVATE_TYPE_WEEKS 3
#define TBPASS_ACTIVATE_TYPE_MONTH 4
#define TBPASS_ACTIVATE_TYPE_YEAR 5

// PASS
#define PASS_TYPE_NONE 0x00
#define PASS_TYPE_TIME 0x01
#define PASS_TYPE_CALENDAR 0x02
#define PASS_TYPE_TRIP 0x03
#define PASS_TYPE_DISCOUNT 0x04

#define PASS_RESULT_INVALID 0
#define PASS_RESULT_VALID 1
#define PASS_RESULT_UPDATE_TIME 2
#define PASS_RESULT_UPDATE_TRIP 3

// Confirm Type
#define CONFIRM_TYPE_NONE 0
#define CONFIRM_TYPE_AGENCY 1
#define CONFIRM_TYPE_RELIGION 2

// MODE
#define MODE_TYPE_BUS 1
#define MODE_TYPE_TRAIN 2
#define MODE_TYPE_TROLLEY 3
#define MDOE_TYPE_SUBWAY 4


// Route Type
#define ROUTE_TYPE_REGULAR 1
#define ROUTE_TYPE_EXPRESS 2


// Message Code
#define MESSAGE_CODE_NONE 0


// Error Code
#define ERROR_CODE_NONE 0


// History Type
#define HISTORY_TYPE_PASS 1
#define HISTORY_TYPE_SV 2
#define HISTORY_TYPE_CREDIT 2


// Payment Type
#define PAYMENT_TYPE_FAILURE 0
#define PAYMENT_TYPE_STORED_VALUE 1
#define PAYMENT_TYPE_PASS 2
#define PAYMENT_TYPE_TRIP_BASED_PASS 3
#define PAYMENT_TYPE_BANK 4

// Transfer Type
#define NO_TRANSFER 0
#define TRANSFER_TYPE_NORMAL 1
#define TRANSFER_TYPE_REGULAR_TO_EXPRESS 2
#define TRANSFER_TYPE_EXPRESS_TO_REGULER 3
#define TRANSFER_TYPE_FREE 0xFF

// Transfer Check Type
#define CHECK_TYPE_CONNECTED_ROUTE 1
#define CHECK_TYPE_CONNECTED_STATION 2

// Account Status
#define ACCOUNT_STATUS_UNABLE 0
#define ACCOUNT_STATUS_ENABLE 1
#define ACCOUNT_STATUS_STOPED 2


// Card Status
#define CARD_STATUS_UNABLE 0
#define CARD_STATUS_ENABLE 1
#define CARD_STATUS_LOST 2
#define CARD_STATUS_STOREN 3


// Card Type
#define CARD_TYPE_NONE 0
#define CARD_TYPE_HOUSE 1
#define CARD_TYPE_BANK 2
#define CARD_TYPE_UNKONW 3

// SFP
#define SFP_REGULAR 0
#define SFP_SENIOR 1
#define SFP_CHILD1 2
#define SFP_CHILD2 3
#define SFP_CHILD3 4
#define SFP_STUDENT1 5
#define SFP_STUDENT2 6
#define SFP_STUDENT3 7
#define SFP_DISABLED 8
#define SFP_DISADVANTAGED 9
#define SFP_EMPLOYEE1 10
#define SFP_EMPLOYEE2 11
#define SFP_BLIND 12
#define SFP_MEDICARE 13
#define SFP_OTHER1 14
#define SFP_OTHER2 15

// Direction
#define NORTH 0x01
#define SOUTH 0x02
#define EAST 0x04
#define WEST 0x08


#define OPEN 1
#define FARE_FREE 0

// Station ID Type
#define STATION_ID_TYPE_DIRECT 1
#define STATION_ID_TYPE_GPS 2



//
#define AGENCY_GROUP_CODE_ANY 0xFF

typedef uint64_t u_int64;
typedef uint32_t u_int32;
typedef uint16_t u_int16;
typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef uint8_t u_int8;
typedef uint8_t BOOL;

#define TRUE 1
#define FALSE 0

#endif

