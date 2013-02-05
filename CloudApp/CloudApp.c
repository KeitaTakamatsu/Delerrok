//
//  CloudApp.c
//  CloudApp
//
//  Created by Haruhiko Soma on 8/14/12.
//  Copyright (c) 2012 QUADRAC Co., Ltd. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "CORE2.h"
#include "COREUtils.h"
#include "CloudApp.h"
#include "FeliCa.h"
#include "structures.h"
#include "defines.h"


#define TYPE_CARD 0x0000
#define TYPE_APP  0x0001

CATaskData *taskEchoHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo);
CATaskData *taskEchoWithTimeoutHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo);
CATaskData *taskEchoWithTimeoutTimeoutHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo);
CATaskData *taskIncrementHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo);

void CloudAppInit(void)
{
  fprintf(stdout, "CloudAppInit called\n");
  
  ///Register handlers
  //Echo
  CARegisterTaskHandler(Echo, taskEchoHandler,NULL);
  //Echo with timeout
  CARegisterTaskHandler(EchoWithTimeout, taskEchoWithTimeoutHandler,NULL);
  CARegisterTimeoutHandler(EchoWithTimeout, taskEchoWithTimeoutTimeoutHandler,NULL);
  //Increment
  SDBAssignInfo taskIncrementHandlerAssignInfo;
  taskIncrementHandlerAssignInfo.numID = 1;
  taskIncrementHandlerAssignInfo.SDBIndex[0] = 0;
  taskIncrementHandlerAssignInfo.IDOffset[0] = SDBASSIGNINFO_IDOFFSET_CLIENTID;
  taskIncrementHandlerAssignInfo.IDLength[0] = sizeof(uint64_t);
  CARegisterTaskHandler(Increment, taskIncrementHandler,&taskIncrementHandlerAssignInfo);
}

void CloudAppTerminate(void)
{
  fprintf(stdout, "CloudAppTerminate called\n");
}

#pragma mark UDP handlers
///CATaskData *handler_routine(CATaskData *taskData, SDBAssignedInfo *sdbInfo)

CATaskData *taskEchoHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo)
{
  dprintf(stderr,"taskEcho called: %s\n", taskData->data[0]+4);
  taskData->timeout = 0;
  return taskData;
}

CATaskData *taskEchoWithTimeoutHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo)
{
  dprintf(stderr,"taskEchoWithTimeout called: %s\n", taskData->data[0]+4 + sizeof(int));
  taskData->timeout = *(int*)(taskData->data[0]+4);
  return taskData;
}

CATaskData *taskEchoWithTimeoutTimeoutHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo)
{
  dprintf(stderr,"taskEchoWithTimeout Timeout handler called: %s\n", taskData->data[0] + 4 + sizeof(int));
  *(int *)((taskData->data[0]+4)) = 0; //Clear timeout on message
  taskData->timeout = 0;
  return taskData;
}

CATaskData *taskIncrementHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo)
{
  dprintf(stderr,"taskIncrement called\n");
  CALog("task called");
  //Update access counter
  int accessCounter = 0;
  
  uint8_t *data = taskData->data[0]+4;
  
  int sdbCount = sdbInfo->count[0];
  if (sdbCount < 0) {
    fprintf(stderr, "SDBAssign failed\n");
    exit(EXIT_FAILURE);
  } else if(sdbCount == 0){
    //Initialize
    accessCounter = 0;
    SDBWrite(0, &accessCounter, 0, sizeof(accessCounter));
  } else {
    //Update
    SDBRead(0, 0, &accessCounter, sizeof(accessCounter));
    accessCounter++;
    SDBWrite(0, &accessCounter, 0, sizeof(accessCounter));
  }
  
  //Read and write message
  char previousMessage[100];
  sdbCount = SDBAssign(1, 10);
  if (sdbCount < 0) {
    fprintf(stderr, "SDBAssign failed\n");
    exit(EXIT_FAILURE);
  } else if(sdbCount == 0){
    //Initialize
    sprintf(previousMessage, "initial message");
    SDBWrite(1, data, 0, taskData->length);
  } else {
    //Update
    SDBRead(1, 0, previousMessage, 100);
    SDBWrite(1, data, 0, taskData->length);
  }
  //SDB Release
  SDBRelease(1);
  
  taskData->length = strlen(previousMessage)+ 1 + sizeof(accessCounter) + 4;
  memcpy(data, &accessCounter, sizeof(accessCounter));
  strcpy((char*)(data + sizeof(accessCounter)), previousMessage);
  
  UINT16(data-4) = TYPE_APP;
  UINT16(data-2) =   taskData->length - 4;
  taskData->timeout = 0;
  return taskData;
}
