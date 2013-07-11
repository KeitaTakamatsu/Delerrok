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
void makeCloudAppData(u_int8* dist, u_int8* src, int index, u_int16 datalength, u_int16 formattype);

txn_t* txn_data;
account_t* account;
agency_t* agency;
route_t* route;
response_t res_data;


/* This method called when Cloud App start. */
void CloudAppInit(void)
{
    fprintf(stdout, "CloudAppInit called\n");
    char tmp[16];
    md5(tmp, 16);
    
    
    account = malloc(sizeof(account_t));
    agency = malloc(sizeof(agency_t));
    route = malloc(sizeof(route_t));
    
    /*
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
    taskTransitHandlerAssignInfo.SDBIndex[2] = SDB_INDEX_ROUTE;
    taskTransitHandlerAssignInfo.IDOffset[2] = ROUTE_ID_OFFSET;
    taskTransitHandlerAssignInfo.IDLength[2] = SIZE_OF_ROUTE_ID;
    */
    // Incliment
    /*
    SDBAssignInfo taskInclimentAssignInfo;
    taskInclimentAssignInfo.numID = 1;
    taskInclimentAssignInfo.SDBIndex[0] = 0;
    taskInclimentAssignInfo.IDOffset[0] = AGENCY_ID_OFFSET;
    taskInclimentAssignInfo.IDLength[0] = SIZE_OF_AGENCY_ID;    
    */
    
    ///Register handlers
    // Transit
    // CARegisterTaskHandler(Transit, taskTransit, &taskTransitHandlerAssignInfo);
    
    
    ///Register handlers
    CARegisterTaskHandler(Transit, taskTransit, NULL);
    // SDBEditor
    CARegisterTaskHandler(SDBEditor, taskSDBEditor, NULL);
    
    
    //Echo
    //CARegisterTaskHandler(Echo, taskEchoHandler, NULL);

    // TestTask
    //CARegisterTaskHandler(TaskTest, taskTest, NULL);
    
    //CARegisterTaskHandler(Increment, taskIncliment, &taskInclimentAssignInfo);
    
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

/* It is sample of use CORE Cloud App. */
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

/* do not use */
CATaskData *taskEchoHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo)
{
  dprintf(stderr,"taskEcho called: %s\n", taskData->data[0]+4);
  taskData->timeout = 0;
  return taskData;
}

/* do not use */
CATaskData *taskEchoWithTimeoutHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo)
{
  dprintf(stderr,"taskEchoWithTimeout called: %s\n", taskData->data[0]+4 + sizeof(int));
  taskData->timeout = *(int*)(taskData->data[0]+4);
  return taskData;
}

/* do not use */
CATaskData *taskEchoWithTimeoutTimeoutHandler(CATaskData *taskData, SDBAssignedInfo *sdbInfo)
{
  dprintf(stderr,"taskEchoWithTimeout Timeout handler called: %s\n", taskData->data[0] + 4 + sizeof(int));
  *(int *)((taskData->data[0]+4)) = 0; // Clear timeout on message
  taskData->timeout = 0;
  return taskData;
}

/* do not use */
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

/*
 If Task ID were 0x03 entry this method when CORE receive message.
*/
CATaskData* taskTransit(CATaskData* taskData, SDBAssignedInfo *sdbInfo)
{
    txn_data = (txn_t*)(taskData->data[0]+4);
    dump_txn(txn_data);
    
    /* Get Agency from SDB*/
    long long l = *((long long*)txn_data->agencyID);    
    SDBAssign(SDB_INDEX_AGENCY, l);
    SDBRead(SDB_INDEX_AGENCY, 0, agency, sizeof(agency_t));
    dump_agency(agency);
    dump_transfer(&(agency->policy.transfer));
    
    /* Get Account from SDB */
    l = *((long long*)txn_data->cardID);
    SDBAssign(SDB_INDEX_ACCOUNT, l);
    SDBRead(SDB_INDEX_ACCOUNT, 0, account, sizeof(account_t));
    dump_account(account);
    
    /* Get Route from SDB */
    l = *((long long*)txn_data->routeID);
    SDBAssign(SDB_INDEX_ROUTE, l);
    SDBRead(SDB_INDEX_ROUTE,  0, route, sizeof(route_t));
    dump_route(route);
    
    /* Processing Transit Task and Get Response*/
    res_data = app(txn_data, account, agency, route);
    
    /* Add Header for CORE App Data */
    makeCloudAppData(taskData->data[0], &res_data, 0, sizeof(response_t), 0x0101);

/* Add Account and Station to response message */
#ifdef DEBUG_MODE
    taskData->length = sizeof(account_t)*2+sizeof(response_t)+sizeof(station_t)+20;
    int datalen = taskData->length-4;
    int idx = sizeof(response_t)+4;
    u_int16 formatType = 0x0101;
    taskData->dataCount = 1;
    
    blockcopy(&formatType, 0, taskData->data[0], 0, 2);
    blockcopy(&datalen, 0, taskData->data[0], 2, 2);
    makeCloudAppData(taskData->data[0], &beforeAccount, idx, sizeof(account_t), 0x0102); idx += sizeof(account_t)+4;
    makeCloudAppData(taskData->data[0], &afterAccount, idx, sizeof(account_t), 0x0103); idx += sizeof(account_t)+4;
    makeCloudAppData(taskData->data[0], &stationForDebug, idx, sizeof(station_t), 0x0104); idx += sizeof(station_t)+4;
    printf("taskData->length=%d\n", taskData->length);
#endif    
    return taskData;
}


/* Add header for CORE App Data */
void makeCloudAppData(u_int8* dist, u_int8* src, int index, u_int16 datalength, u_int16 formattype)
{
    blockcopy(&formattype, 0, dist, index, 2);
    blockcopy(&datalength, 0, dist, index+2, 2);
    blockcopy(src, 0, dist, index+4, datalength);
#ifdef DEBUG_CONSOLE
    printf("DataLength = %d\n", *((u_int16*) (dist+2)));
#endif
}

u_int8* sdbid_tmp;
/*
 If Task ID were 0x04 Entry this method when CORE receive message.
 Add or Edit SDB.
 CORE delete all data when CORE down.
*/
CATaskData *taskSDBEditor(CATaskData* taskData, SDBAssignedInfo* sdbInfo)
{
    sdbtxn_t* data = (sdbtxn_t*)(taskData->data[0]);
    
    switch(data->sdbindex)
    {
        case SDB_INDEX_ACCOUNT:
        {
            /* Write Account Data to SDB */
            account_t ac = *((account_t*) data->data);
            dump_account(&ac);
            SDBAssign(data->sdbindex, *((long long*)data->sdbID));
            SDBWrite(data->sdbindex, &ac, 0, sizeof(account_t));
            SDBRelease(data->sdbindex);
            break;
        }
        case SDB_INDEX_AGENCY:
        {
            printf("Data Index=%d\n", data->dataIndex);
            agency_t ag;
            
            /* Write Agency Data to SDB */
            long long l = *((long long*)data->sdbID);
            SDBAssign(data->sdbindex, l);
            SDBWrite(data->sdbindex, data->data, data->dataIndex, data->length-13);
            SDBRelease(data->sdbindex);
            
            SDBAssign(data->sdbindex, l);
            SDBRead(data->sdbindex, 0, &ag, sizeof(agency_t));
            dump_policy(&ag.policy);
            SDBRelease(data->sdbindex);
            break;
        }
        case SDB_INDEX_FARE:
        {
            /* Write Fare to SDB */
            fare_t fare = *((fare_t*) data->data);
            long long id = makeFlatFareIDFromFare(&fare);
            SDBAssign(data->sdbindex,  id);
            SDBWrite(data->sdbindex, &fare, 0, sizeof(fare_t));
            SDBRelease(data->sdbindex);
            break;
        }
        case SDB_INDEX_ROUTE:
        {
            /* Write Route Data to SDB */
            route_t route = *((route_t*) data->data);
            SDBAssign(data->sdbindex,  *((long long*)data->sdbID));
            SDBWrite(data->sdbindex, &route, 0, sizeof(route_t));
            SDBRelease(data->sdbindex);
            break;
        }
        case SDB_INDEX_STATION:
        {
            /* Write Station Data to SDB */
            station_t station = *((station_t*) data->data);
            SDBAssign(data->sdbindex, *((long long*)data->sdbID));
            SDBWrite(data->sdbindex, &station, 0, sizeof(station_t));
            SDBRelease(data->sdbindex);
            break;
        }
        case 33: // Temporary
        {
            /* Write Station Data to SDB */
            /* CORE can't receive data over 1000 byte. */
            /* Agency Data Size is over 1000 byte */

            farePolicy_t policy = *((farePolicy_t*) data->data);
            dump_policy(&policy);
            dump_transfer(&(policy.transfer));
            
            agency_t ag;
            /* Write Agency Data to SDB */
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





















