#ifndef __data_h__
#define __data_h__


#include <stdlib.h>
#include "obj_t.h"

typedef enum {NUMERO, REAL, BOOL, OBJ} DATA_TYPE;
typedef struct{
  union{
    int numero;
    float real;
    bool b;
    obj_t* obj;
  };
} data_t;

#endif
