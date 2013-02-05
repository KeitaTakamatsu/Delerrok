//
//  COREUtils.cpp
//  CORE2
//
//  Created by Haruhiko Soma on 8/13/12.
//  Copyright (c) 2012 QUADRAC Co., Ltd. All rights reserved.
//

#include "COREUtils.h"

#ifdef DEBUGPRINT
void printHexData(uint8_t *data, size_t size)
{
  for(int i=0;i<size;i++)
  {
    fprintf(stdout, "%x ", data[i]);
  }
  fprintf(stdout, "\n");
}

void ccpprintf_(FILE *stream, uint8_t ccpID, const char *method_name, const char *format, ...)
{
  va_list ap;
  va_start(ap, format);
  
  fprintf(stream, "[%llu][%s] ", (uint64_t)ccpID, method_name);
  vfprintf(stream, format, ap);
  fflush(stream);
  
  va_end(ap);
}
#endif
