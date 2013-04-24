//
//  HistoryData.h
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/28.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#ifndef Delerrok_HistoryData_h
#define Delerrok_HistoryData_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "structures.h"

history_t makeHistoryData(u_int8 historyType, route_t* route, station_t* station, u_int8* timestamp,
                          u_int8 paymentType);

#endif
