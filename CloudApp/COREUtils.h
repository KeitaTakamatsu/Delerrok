//
//  COREUtils.h
//  CORE2
//
//  Created by Haruhiko Soma on 8/13/12.
//  Copyright (c) 2012 QUADRAC Co., Ltd. All rights reserved.
//

#ifndef __CORE2__COREUtils__
#define __CORE2__COREUtils__

#ifdef __cplusplus
#include <iostream>
#include <cstdarg>
using namespace std;
#endif
  

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#define UINT64( A ) (*(uint64_t *)( A ))
#define UINT16( A ) (*(uint16_t *)( A ))

#ifdef DEBUGDEVPRINT
#define ccuprintf(A, ...) ccpprintf_(A, 8, __VA_ARGS__ )
#define ccpprintf(A, ...) ccpprintf_(A, ccpID_, __VA_ARGS__ )
#else
#define ccpprintf( A, ... )
#define ccuprintf( A, ... )
#endif
#ifdef DEBUGPRINT
#define dprintf(A, ...) fprintf(A, __VA_ARGS__)
#define dprintHexData( A, ... ) printHexData( A, __VA_ARGS__ )
//#define delete( A ) fprintf(stderr, "%s:%d delete( %lx ) \n", __FILE__, __LINE__, (unsigned long int)A); delete(A);
void printHexData(uint8_t *data, size_t size);

void ccpprintf_(FILE *stream, uint8_t ccpID, const char *method_name, const char *format, ...);
#else
#define dprintf( A, ... )
#define dprintHexData( A, ...)
#define printHexData( A, ...)
#endif


#endif /* defined(__CORE2__COREUtils__) */
