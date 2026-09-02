#include <stdio.h>
#include <stdlib.h>

enum OBJS {NUMERO, REAL, CADENA};
typedef struct{
  OBJS type;
  // more metadata about the object instance itself
} obj_base;

typedef struct{
  obj_base* base;

}obj_t;


typedef struct node{
  obj_t value;
  struct node* next;
} node_t;

enum types {INT, FLOAT, CHAR, STRING, BIT};
typedef struct{
  node_t* head;
  size_t n;
  enum types type;
} lld_t;






