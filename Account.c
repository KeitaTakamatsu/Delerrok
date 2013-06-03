//
//  Account.c
//  Delerrok
//
//  Created by Keita Takamatsu on 2013/02/26.
//  Copyright (c) 2013年 Keita Takamatsu. All rights reserved.
//

#include "Account.h"
#include "Util.h"


int checkAccount(u_int8* agencyGroupCode, account_t* account, farePolicy_t* farePolicy, u_int8* timestamp, u_int8* expireDate)
{
    if(!isCardStatusValid(account, farePolicy))
    {
#ifdef CONSOLE
        printf("[CheckAccount] HIT--001\n");
#endif
        return 0;
    }
    if(!isCardStatusValid(account, farePolicy))
    {
#ifdef CONSOLE
        printf("[CheckAccount] HIT--002\n");
        return 0;
#endif
    }
    if(!compare(account->agencyGroupCode, 0, agencyGroupCode, 0, SIZE_OF_AGENCY_GROUP_CODE))
    {
#ifdef CONSOLE
        printf("[CheckAccount] HIT--003\n");
#endif
        return 0;
    }
    
    // DateCompare
    struct tm now = getNow();
    struct tm expire = makeTimeYYYYMMDD(expireDate);
    if(datetimeCompareShort(now, expire) > 0)
    {
#ifdef CONSOLE
        printf("[CheckAccount] HIT--004\n");
#endif
        return 0;
    }
    return 1;
}


int isCardStatusValid(account_t* account, farePolicy_t* policy)
{
    int i;
    //for(i = 0; i < NUM_OF_VALID_CARD_STATUS; i++)
    for(i = 0; i < SIZE_OF_VALID_CARD_STATUS_LIST; i++)
    {
        if( policy->validCardTypeList[i] == 0)
            return 0;
        if(policy->validCardStatusList[i] == account->cardStatus)
            return 1;
    }
    
    return 0;
}

int isAccountValid(account_t* account, farePolicy_t* policy)
{
    int i;
    //for(i = 0; i < NUM_OF_VALID_CARD_STATUS; i++)
    for(i = 0; i < SIZE_OF_VALID_ACCOUNT_STATUS_LIST; i++)
    {
        if(policy->validAccountStatusList[i] == 0)
            return 0;
        if(policy->validAccountStatusList[i] == account->accountStatus)
            return 1;
    }
    return 0;
}
