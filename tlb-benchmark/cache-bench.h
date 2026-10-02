#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <sys/mman.h>
#include <memory.h>
/*
i7-9700k 8-core unthreaded
-----------------------------    --------------------------
|          CACHE            |    |          TLB           | 
-----------------------------    --------------------------
32KB L1 (I) cache /pcore          L1 I-TLB /pcore
- cache line size 64 bytes        - 64 Entries (4kb) 
- associativity 8-ways            - 8 Entries (2-4m)

32KB L1 (D) cache /pcore          L1 D-TLB /pcore
- cache line size 64 bytes        - 64 Entries (4kb) 
- associativity 8-ways            - 8 Entries (2-4mb)

256 kb L2 cache /pcore            L2 STLV (shared)
- cache line size 64 bytes        - 1536 (4kb-2mb) 6-ways
- associativity 4-ways

12 MB L3 cache (shared)
- cache line size 64 bytes
- associativity 12-ways
*/
#define PAGE        4096UL
#define GLOBAL_MAX  (64UL << 20) // 64MB
#define MAX_REPEAT  64


typedef enum {SRC_STACK, SRC_GLOBAL, SRC_HEAP} Source;

static unsigned char g_buf[GLOBAL_MAX] /*__atribute__((aligned(4096)))*/;

typedef enum{
  PAT_SEQ,
  PAT_CHASE,
  PAT_CHASE_RAND
} Pattern;

typedef struct {
  Source src;
  Pattern pat;
  double tmin, tmax;
  int repeat;
  size_t max_bytes;
  int cpu;
  const char* out_path;
  FILE* out;
} Cfg;

typedef struct{
  size_t N, s, n_touched;
  uint64_t accesses;
  double ns_min, ns_mean, ns_sd;
  int reps;
} Result;

static volatile uint64_t g_sink;


