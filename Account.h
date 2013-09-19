//
//  Account.h
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/26.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#ifndef Delerrok_Account_h
#define Delerrok_Account_h

#include "structures.h"
#include "defines.h"

int checkAccount(uint8_t* agencyGroupCode, account_t* account, farePolicy_t* farePolicy, uint8_t* timestamp, uint8_t* expireDate);
int isCardStatusValid(account_t* account, farePolicy_t* policy);
int isAccountValid(account_t* account, farePolicy_t* policy);

#endif
