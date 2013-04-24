//
//  ResponseData.c
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/27.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#include "ResponseData.h"

response_t res;
response_t makeResponse(int open, int fare, int messageCode, int errorCode, int sfp, u_int8* passExpireDate, int numOfTripBasedPass, int balance)
{    
    const u_int8 INVALID_DATE[] = {0x01, 0x01,0x00,0x01};
    res.open = open;
    res.fare = fare;
    res.messageCode = messageCode;
    res.errorCode = errorCode;
    res.sfpCode = sfp;
    if(passExpireDate != NULL)
        memcpy(res.passExpireDate, passExpireDate, 4);
    res.numOfTripBasedPass = numOfTripBasedPass;
    res.balance = balance;
    
    if(passExpireDate == NULL)
        memcpy(res.passExpireDate, INVALID_DATE, 4);
    else
        memcpy(res.passExpireDate, passExpireDate, 4);
    return res;
}
