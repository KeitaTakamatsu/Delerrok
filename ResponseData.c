//
//  ResponseData.c
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/27.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#include "ResponseData.h"

response_t res;
/* Make Response Data. */
response_t makeResponse(int open, int fare, int messageCode, int errorCode, int sfp, u_int8* passExpireDate, int numOfTripBasedPass, int balance)
{    
    const u_int8 INVALID_DATE[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    res.open = open;
    res.fare = fare;
    res.messageCode = messageCode;
    res.errorCode = errorCode;
    res.sfpCode = sfp;
    /*
    if(passExpireDate != '\0')
        memcpy(res.passExpireDate, passExpireDate, 6);
    */
    res.numOfTripBasedPass = numOfTripBasedPass;
    res.balance = balance;
    
    if(passExpireDate == '\0')
        memcpy(res.passExpireDate, INVALID_DATE, 6);
    else
        memcpy(res.passExpireDate, passExpireDate, 6);
    return res;
}
