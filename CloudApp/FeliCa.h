//
//  FeliCa.h
//  FeliCa
//
//  Created by Tadahiro Wakasa on 10/5/12.
//  Copyright (c) 2012 developer. All rights reserved.
//

#ifndef __CORE2_FeliCa_FeliCa_h
#define __CORE2_FeliCa_FeliCa_h

#include "CORE2.h"
#ifdef __cplusplus
extern "C" {
#endif
  
#define SYSTEM_LENGTH 2
#define MAX_AREA_NUMBER 16
#define AREA_LENGTH 2
#define MAX_AREA_LENGTH AREA_LENGTH * MAX_AREA_NUMBER //32
#define MAX_SERVICE_NUMBER 32
#define SERVICE_LENGTH 2
#define MAX_SERVICE_LENGTH SERVICE_LENGTH * MAX_SERVICE_NUMBER //64
#define SERVICE_KEYVERSION_LENGTH 2
#define MAX_SERVICE_KEYVERSION_LENGTH SERVICE_KEYVERSION_LENGTH * MAX_SERVICE_NUMBER //64
#define MAX_BLOCK_NUMBER 14
#define BLOCK_LIST_LENGTH 2
#define MAX_BLOCK_LIST_LENGTH BLOCK_LIST_LENGTH * MAX_BLOCK_NUMBER //28
#define BLOCK_DATA_LENGTH 16
#define MAX_BLOCK_DATA_LENGTH BLOCK_DATA_LENGTH * MAX_BLOCK_NUMBER //224
#define KEY_GROUP_MASTER_LENGTH 8
#define KEY_GROUP_ACCESS_LENGTH 8
#define MAX_RECIEVE_NUMBER 14
#define IDMPMM_LENGTH 16
#define IDM_LENGTH 8
#define PMM_LENGTH 8
#define IDI_LENGTH 8
#define PMI_LENGTH 8

  typedef struct {
    uint8_t system[SYSTEM_LENGTH];
    uint8_t timeslot;
    uint8_t area_number;
    uint8_t area[MAX_AREA_LENGTH];
    uint8_t service_number;
    uint8_t service[MAX_SERVICE_LENGTH];
    uint8_t service_keyversion[MAX_SERVICE_KEYVERSION_LENGTH];
    uint8_t block_number;
    uint8_t block_list[MAX_BLOCK_LIST_LENGTH];
    uint8_t block_data[MAX_BLOCK_DATA_LENGTH];
    uint8_t key_group_master[KEY_GROUP_MASTER_LENGTH];
    uint8_t key_group_access[KEY_GROUP_ACCESS_LENGTH];
    uint8_t idmpmm[MAX_RECIEVE_NUMBER][IDMPMM_LENGTH];
    uint8_t idm[IDM_LENGTH];
    uint8_t pmm[PMM_LENGTH];
    uint8_t idi[IDI_LENGTH];
    uint8_t pmi[PMI_LENGTH];
    uint8_t receive_number;
    uint8_t mode;
    uint8_t status1;
    uint8_t status2;
    uint8_t result;
    uint8_t index;
  } CAFeliCaCard;
  
  CATaskData *CAFeliCaPolling(CATaskData *taskData, CAFeliCaCard *card);
  CATaskData *CAFeliCaRequestService(CATaskData *taskData, CAFeliCaCard *card);
  CATaskData *CAFeliCaRequestResponse(CATaskData *taskData, CAFeliCaCard *card);
  CATaskData *CAFeliCaMutualAuthentication(CATaskData *taskData, CAFeliCaCard *card);
  CATaskData *CAFeliCaReadBlock(CATaskData *taskData, CAFeliCaCard *card);
  CATaskData *CAFeliCaWriteBlock(CATaskData *taskData, CAFeliCaCard *card);
  CATaskData *CAFeliCaReadBlockWithoutEncryption(CATaskData *taskData, CAFeliCaCard *card);
  CATaskData *CAFeliCaWriteBlockWithoutEncryption(CATaskData *taskData, CAFeliCaCard *card);
  void CAFeliCaRelease(CAFeliCaCard *card);
  void CAFeliCaSetIDmPMm(CAFeliCaCard *card);
#ifdef __cplusplus
}
#endif
#endif //__CORE2_FeliCa_FeliCa_h
