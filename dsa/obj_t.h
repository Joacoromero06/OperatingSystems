#ifndef __obj_h__
#define __obj_h__

#include <stdlib.h>
#include "data_t.h"

typedef enum {STRING, LIST, SET, TUPLE, FUNCTION} OBJ_TYPE;
typedef struct obj{
  OBJ_TYPE type;
  
} obj_t;

typedef struct{
  obj_t base;
  data_t value;
} objList_t;

#endif 

