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

int checkAccount(u_int8* agencyGroupCode, account_t* account, farePolicy_t* farePolicy, u_int8* timestamp, u_int8* expireDate);
int isCardStatusValid(account_t* account, farePolicy_t* policy);
int isAccountValid(account_t* account, farePolicy_t* policy);

#endif
