//
//  CloudApp.h
//  CloudApp
//
//  Created by Haruhiko Soma on 10/3/12.
//  Copyright (c) 2012 QUADRAC Co., Ltd. All rights reserved.
//

#ifndef CloudApp_CloudApp_h
#define CloudApp_CloudApp_h

enum TestCloudAppTaskID {
    Echo            = 0x0000,
    EchoWithTimeout = 0x0001,
    Increment       = 0x0002,
    Transit         = 0x0003,
    SDBEditor       = 0x0004,
    SDBReader       = 0x0005,
    TaskTest        = 0x0006,
    TaskTransfer    = 0x0007
};




#endif
