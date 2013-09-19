//
//  ResponseData.h
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/27.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#ifndef Delerrok_ResponseData_h
#define Delerrok_ResponseData_h

#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "structures.h"
response_t makeResponse(int open, int fare, int messageCode, int errorCode, int sfp, uint8_t* passExpireDate, int numOfTripBasedPass, int balance);


#endif
