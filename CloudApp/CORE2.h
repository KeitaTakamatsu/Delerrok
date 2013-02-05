//
//  CORE2.h
//  CORE2
//
//  Created by Haruhiko Soma on 8/23/12.
//  Copyright (c) 2012 QUADRAC Co., Ltd. All rights reserved.
//

#ifndef __CORE2_CORE2__
#define __CORE2_CORE2__

#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif
  
#define SDBMAXINDEX 8
  typedef struct {
    uint8_t   formatType;
    uint8_t   cloudAppID;
    uint16_t  taskID;
    uint8_t  clientID[8];
    uint8_t   dataCount;
    uint16_t  length; ///< Length of Cloud App Data
    void   *data[256]; ///< Cloud App Data
    int timeout;
  } CATaskData;
  
  typedef struct {
    int numID;
    int SDBIndex[SDBMAXINDEX];
    uint64_t ID[SDBMAXINDEX];
    int count[SDBMAXINDEX];
  } SDBAssignedInfo;
  
  typedef struct {
    int numID;
    int SDBIndex[SDBMAXINDEX];
    int IDOffset[SDBMAXINDEX];
    int IDLength[SDBMAXINDEX];
  } SDBAssignInfo;
  
#define SDBASSIGNINFO_IDOFFSET_CLIENTID -1
  
  typedef struct {
    uint64_t used;
    uint64_t free;
    uint64_t dataSize;
  } SDBInfo;
  
  typedef struct {
    uint64_t used;
    uint64_t free;
    uint64_t dataSize;
    uint64_t lastIndex;
  } SDBCyclicRecordInfo;
  
  int SDBAssign(uint8_t SDBIndex, uint64_t ID);
  int SDBAssignStatic(uint8_t SDBIndex,uint64_t ID);
  int SDBRelease(uint8_t SDBIndex);
  
  bool SDBRead(uint8_t SDBIndex , int fromOffset, void *to, int size);
  bool SDBWrite(uint8_t SDBIndex , void *from, int toOffset, int size);
  bool SDBClear(uint8_t SDBIndex, int fromOffset, int size);
  
  uint8_t * SDBGetReadBuffer(uint8_t SDBIndex, int fromOffset, int size);
  void SDBLockReadBuffer(uint8_t SDBIndex);
  void SDBUnlockReadBuffer(uint8_t SDBIndex);
  
  bool SDBCyclicAppendSubRecord(uint8_t SDBIndex, uint8_t *data, int size);
  bool SDBCyclicReadSubRecordAt(uint8_t SDBIndex, uint64_t startIndex, uint64_t indexOffset, uint8_t *data);
  bool SDBCyclicWriteSubRecordAt(uint8_t SDBIndex, uint64_t startIndex, uint64_t indexOffset, uint8_t *data);
  bool SDBCyclicRemoveLatestSubRecord(uint8_t SDBIndex);
  
  void SDBGetPartitionInfo(uint8_t SDBIndex, SDBInfo *info);
  void SDBGetCyclicRecordInfo(uint8_t SDBIndex, SDBCyclicRecordInfo *info);
  
  
  void CALog(const char *str, ...);
  
  void CAGetTimestamp(struct timeval *timestamp);
  
  bool CARegisterTaskHandler(uint16_t taskID, CATaskData *(*handler_routine)( CATaskData *, SDBAssignedInfo *),SDBAssignInfo *sdbInfo);
  bool CARegisterTimeoutHandler(uint16_t taskID, CATaskData *(*handler_routine)( CATaskData *, SDBAssignedInfo *), SDBAssignInfo *sdbInfo);
  
  /**
   * Task
   */
  //CATaskData *handler_routine(CATaskData *taskData, SDBAssignedInfo *sdbInfo);
  
#ifdef __cplusplus
}
#endif

#endif/* defined(__CORE2__CORE2__) */
