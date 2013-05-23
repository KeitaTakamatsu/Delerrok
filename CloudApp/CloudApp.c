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
#include "TransitApp.h"
#include "StoredValue.h"
#include "Util.h"
#include "defines.h"
#include "udp.h"

#define TYPE_CARD 0x0000
#define TYPE_APP  0x0001

CATaskData *taskEchoHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo);
CATaskData *taskEchoWithTimeoutHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo);
CATaskData *taskEchoWithTimeoutTimeoutHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo);
CATaskData *taskIncrementHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo);
CATaskData *taskTransit(CATaskData* taskData, SDBAssignedInfo *sdbInfo);
CATaskData *taskSDBEditor(CATaskData* taskData, SDBAssignedInfo* sdbInfo);
CATaskData *taskTest(CATaskData* taskData, SDBAssignedInfo* sdbInfo);
CATaskData *taskIncliment(CATaskData* taskData, SDBAssignedInfo* sdbInfo);


txn_t* txn_data;
account_t* account;
agency_t* agency;
route_t* route;
response_t res_data;

void CloudAppInit(void)
{
    fprintf(stdout, "CloudAppInit called\n");
    char tmp[16];
    md5(tmp, 16);
    
    account = malloc(sizeof(account_t));
    agency = malloc(sizeof(agency_t));
    route = malloc(sizeof(route_t));
    
    
    // Transit
    SDBAssignInfo taskTransitHandlerAssignInfo;
    taskTransitHandlerAssignInfo.numID = 2;
    // Account
    taskTransitHandlerAssignInfo.SDBIndex[0] = SDB_INDEX_ACCOUNT;
    taskTransitHandlerAssignInfo.IDOffset[0] = CARD_ID_OFFSET;
    taskTransitHandlerAssignInfo.IDLength[0] = SIZE_OF_CARD_ID;
    // Agency
    taskTransitHandlerAssignInfo.SDBIndex[1] = SDB_INDEX_AGENCY;
    taskTransitHandlerAssignInfo.IDOffset[1] = AGENCY_ID_OFFSET;
    taskTransitHandlerAssignInfo.IDLength[1] = SIZE_OF_AGENCY_ID;
    // Route
    // taskTransitHandlerAssignInfo.SDBIndex[2] = SDB_INDEX_ROUTE;
    // taskTransitHandlerAssignInfo.IDOffset[2] = ROUTE_ID_OFFSET;
    // taskTransitHandlerAssignInfo.IDLength[2] = SIZE_OF_ROUTE_ID;
    
    // Incliment
    SDBAssignInfo taskInclimentAssignInfo;
    taskInclimentAssignInfo.numID = 1;
    taskInclimentAssignInfo.SDBIndex[0] = 0;
    taskInclimentAssignInfo.IDOffset[0] = AGENCY_ID_OFFSET;
    taskInclimentAssignInfo.IDLength[0] = SIZE_OF_AGENCY_ID;
    
    
    
    ///Register handlers
    //Echo
    CARegisterTaskHandler(Echo, taskEchoHandler, NULL);
    // Transit
    CARegisterTaskHandler(Transit, taskTransit, &taskTransitHandlerAssignInfo);
    // SDBEditor
    CARegisterTaskHandler(SDBEditor, taskSDBEditor, NULL);
    // TestTask
    CARegisterTaskHandler(TaskTest, taskTest, NULL);
    
    CARegisterTaskHandler(Increment, taskIncliment, &taskInclimentAssignInfo);
    
    //Echo with timeout
    // CARegisterTaskHandler(EchoWithTimeout, taskEchoWithTimeoutHandler,NULL);
    /*
    CARegisterTimeoutHandler(EchoWithTimeout, taskEchoWithTimeoutTimeoutHandler,NULL);
    //Increment
    SDBAssignInfo taskIncrementHandlerAssignInfo;
    taskIncrementHandlerAssignInfo.numID = 1;
    taskIncrementHandlerAssignInfo.SDBIndex[0] = 0;
    taskIncrementHandlerAssignInfo.IDOffset[0] = SDBASSIGNINFO_IDOFFSET_CLIENTID;
    taskIncrementHandlerAssignInfo.IDLength[0] = sizeof(uint64_t);
    CARegisterTaskHandler(Increment, taskIncrementHandler,&taskIncrementHandlerAssignInfo);
    */
}

void CloudAppTerminate(void)
{
  fprintf(stdout, "CloudAppTerminate called\n");
}

#pragma mark UDP handlers
///CATaskData *handler_routine(CATaskData *taskData, SDBAssignedInfo *sdbInfo)

CATaskData *taskIncliment(CATaskData* taskData, SDBAssignedInfo* sdbInfo)
{
    int num = 0;
    if(sdbInfo->count[0] > 0)
    {
        SDBRead(0, 0, &num, 4);
        num++;
        SDBWrite(0, &num, 0, 4);
    }
    else if(sdbInfo->count == 0)
    {
        SDBWrite(0, &num, 0, 4);
    }
    printf("NUM=%d\n", num);
    SDBRelease(0);
    return taskData;
}

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
  *(int *)((taskData->data[0]+4)) = 0; // Clear timeout on message
  taskData->timeout = 0;
  return taskData;
}

CATaskData *taskTest(CATaskData* taskData, SDBAssignedInfo* sdbInfo)
{
    route_t* route = NULL;
    sdbtxn_t* data = (sdbtxn_t*)(taskData->data[0]);
    
    int num = sizeof(route_t);
    printf("num=%d\n", num);
    
    long long l =  *((long long*)data->sdbID);
    SDBAssign(data->sdbindex, l);
    SDBRead(data->sdbindex, 0, route, sizeof(route_t));
    SDBRelease(data->sdbindex);
    memcpy(taskData->data[0], &route->routeID, 8);
    return taskData;
}

u_int8 c[128];
CATaskData* taskTransit(CATaskData* taskData, SDBAssignedInfo *sdbInfo)
{
    txn_data = (txn_t*)(taskData->data[0]+4);
    dump_txn(txn_data);
    
    SDBRead(SDB_INDEX_AGENCY, 0, agency, sizeof(agency_t));
    dump_agency(agency);
    
    SDBRead(SDB_INDEX_ACCOUNT, 0, account, sizeof(account_t));
    dump_account(account);
    
    
    long long* testlong = malloc(sizeof(long long));
    memcpy(testlong, txn_data->routeID, 8);    
    SDBAssign(SDB_INDEX_ROUTE, *testlong);
    SDBRead(SDB_INDEX_ROUTE,  0, route, sizeof(route_t));
    dump_route(route);    
    
    res_data = app(txn_data, account, agency, route);
    memcpy(taskData->data[0], &res_data, sizeof(response_t));
    
    dump_responseData(taskData->data[0]);
    dump_arr("Response", taskData->data[0], 0, sizeof(response_t));
    printf("sizeof(response_t) = %d\n",sizeof(response_t));
    
    
    blockcopy(taskData, 0, c, 0, 20);
    blockcopy(taskData->data[0], 0, c, 20, sizeof(response_t));
    dump_arr("C=", c, 0, 20 + sizeof(response_t));
    udp_send_original(c, 20 + sizeof(response_t), "10.0.1.8", 12345);
    // udp_send_original(taskData->data[0], sizeof(response_t), "10.0.1.8", 12345);
    return taskData;
}



u_int8* sdbid_tmp;
CATaskData *taskSDBEditor(CATaskData* taskData, SDBAssignedInfo* sdbInfo)
{
    sdbtxn_t* data = (sdbtxn_t*)(taskData->data[0]);
    
    switch(data->sdbindex)
    {
        case SDB_INDEX_ACCOUNT:
        {
            account_t ac = *((account_t*) data->data);
            dump_account(&ac);
            SDBAssign(data->sdbindex, *((long long*)data->sdbID));
            SDBWrite(data->sdbindex, &ac, 0, sizeof(account_t));
            SDBRelease(data->sdbindex);
            break;
        }
        case SDB_INDEX_AGENCY:
        {
            agency_t ag = *((agency_t*) data->data);
            dump_agency(&ag);
            
            SDBAssign(data->sdbindex,  *((long long*)data->sdbID));
            SDBWrite(data->sdbindex, &ag, 0, sizeof(agency_t));
            SDBRelease(data->sdbindex);
            break;
        }
        case SDB_INDEX_FARE:
        {            
            fare_t fare = *((fare_t*) data->data);
            long long id = makeFlatFareIDFromFare(&fare);
            SDBAssign(data->sdbindex,  id);
            SDBWrite(data->sdbindex, &fare, 0, sizeof(fare_t));
            SDBRelease(data->sdbindex);
            break;
        }
        case SDB_INDEX_ROUTE:
        {
            route_t route = *((route_t*) data->data);
            SDBAssign(data->sdbindex,  *((long long*)data->sdbID));
            SDBWrite(data->sdbindex, &route, 0, sizeof(route_t));
            SDBRelease(data->sdbindex);
            break;
        }
        case SDB_INDEX_STATION:
        {
            station_t station = *((station_t*) data->data);
            long long id = makeStationIDFromStation(&station);
            SDBAssign(data->sdbindex,  id);
            SDBWrite(data->sdbindex, &station, 0, sizeof(station_t));
            SDBRelease(data->sdbindex);
            break;
        }
        case 33:
        {
            farePolicy_t policy = *((farePolicy_t*) data->data);
            dump_policy(&policy);
            
            agency_t ag;
            SDBAssign(SDB_INDEX_AGENCY, *((long long*) data->sdbID));
            SDBRead(SDB_INDEX_AGENCY, 0, &ag, sizeof(agency_t));
            ag.policy = policy;
            SDBWrite(SDB_INDEX_AGENCY, &ag, 0, sizeof(agency_t));
            SDBRelease(SDB_INDEX_AGENCY);
            break;
        }
    }
    return taskData;
}





















