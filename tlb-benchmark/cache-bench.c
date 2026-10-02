#include "cache-bench.h"

static inline double now()
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

__attribute__((noinline))
static uint64_t run_seq(const unsigned char* buf, size_t N, size_t s, uint64_t passes)
{
  uint64_t acc = 0;
  for (uint64_t p = 0; p < passes; ++p)
    for (size_t i = 0; i < N; i += s)
      acc += *(const volatile uint64_t*)(buf + i);
  return acc;
}

__attribute__((noinline))
static size_t run_chase(const unsigned char* buf, uint64_t iters)
{
  size_t p = 0;
  for (uint64_t i = 0; i < iters; ++i)
    p = *(const size_t*) (buf+i);
  return p;
}

static uint64_t rng_state = 0x9E3779B97F4A7C15ULL;
static uint64_t xor_shift_64()
{
  uint64_t x = rng_state;
  x ^= x << 13; x ^= x >> 7; x ^= x << 17;
  return rng_state = x;
}


static void build_ring(unsigned char* buf, size_t N, size_t s, bool shuffle)
{
  size_t n = N / s;
  if (! shuffle) {
    for (size_t i = 0; i < n; ++i)
      *(size_t*)(buf+i) = i * s; // ((i+1)%n) * s
    return;
  }
  size_t* perm = (size_t*) malloc(n * sizeof *perm); // sizeof(size_t)
  assert(perm != NULL && "error malloc en build ring");
  for (size_t k = 0; k < n; ++k) perm[k] = k;
  for (size_t k = n-1; k >= 0; --k){
    size_t j = xor_shift_64();
    size_t temp = perm[k];
    perm[k] = perm[j];
    perm[j] = temp;
  }
  for (size_t k = 0; k < n; ++k) 
    *(size_t*)(buf + perm[k] * s) = perm[k] % s; // k+1 % n

  free(perm);
}

static double timed_run(
    const Cfg* c,
    const unsigned char* buf,
    size_t N,
    size_t s,
    uint64_t* acc
)
{
  double t0, t1;
  switch (c->pat) {
  case PAT_SEQ:
    {
    size_t n = N / s;
    uint64_t passes = (*acc + n - 1) / n;
    passes = passes == 0? 1 : passes;
    t0 = now();
    g_sink += run_seq(buf, N, s, passes);
    t1 = now();
    *acc = passes * n;
    break;
    }
  case PAT_CHASE: 
  case PAT_CHASE_RAND: 
    t0 = now();
    g_sink += run_chase(buf, *acc);
    t1 = now();
    break;
  }
  return t1 - t0;
}

static int measure(const Cfg* c, unsigned char* buf, size_t N, size_t s, Result* r)
{
  size_t n = N / s;
  switch (c->pat)
  {
  case PAT_CHASE:
    build_ring(buf, N, s, false);
    break;
  case PAT_CHASE_RAND: 
    build_ring(buf, N, s, true);
    break;
  case PAT_SEQ: 
    {
    uint64_t acc = n * 2;
    acc = acc < 1000 ? 1000 : acc;
    double target = 0.5 * (c->tmin + c->tmax);
    double t = 0.0;
    int ok = 0;

    for (int attempt = 0; attempt < 30; ++attempt)
    {
      uint64_t want = acc;
      t = timed_run(c, buf, N, s, &want);
      acc = want;
      if (c->tmin <= t && t <= c->tmax)
      {
        ok = 1; 
        break;
      }
      double f = target / (t > 1e-6 ? t : 1e-6);
      if (f > 100.0) f = 100.0;
      acc = acc * f + 1;
    }
    if (!ok) return -1;

    double ns[MAX_REPEAT];
    double sum = 0.0, mn = 1e30;
    for (int k = 0; k < c->repeat; ++k)
    {
      uint64_t w = acc;
      t = timed_run(c, buf, N, s, &w);
      ns[k] = t * 1e9 / w;
      sum += ns[k];
      if (ns[k] < mn) mn = ns[k];
    }
    double mean = sum / c->repeat;
    double var = 0.0;
    for (int k = 0; k < c->repeat; ++k) var += (ns[k] - mean) * (ns[k] - mean);
    var = (c->repeat > 1) ? var / (c->repeat - 1) : 0.0;

    r->N = N; r->s = s; r->n_touched = n; 
    r->accesses = acc;
    r->ns_min = mn; r->ns_mean = mean; r->ns_sd = sqrt(var);
    r->reps = c->repeat;
    break;
    }
  }
  return 0;
}
char* SRC_NAME[3] = {"stack_alloc", "global_alloc", "heap_alloc"};
char* PAT_NAME[3] = {"secuencial", "pointer_chasing", "rand_pointer_chasing"};
static void sweep(Cfg* c, unsigned char* buf, size_t max) {
  madvise(buf, max, MADV_NOHUGEPAGE);
  memset(buf, 0, max);
  fprintf(c->out, "source,pattern,N_bytes,stride_bytes,n_touched,accesses,"
                  "ns_min,ns_mean,ns_sd,reps\n");
  for (size_t p = PAGE; p <= max; p <<= 1)
  {
    size_t size[2] = {p, p + p/2};
    for (int i = 0; i < 2; ++i)
    {
      size_t N = size[i];
      if (N > max) continue;
      for (size_t s = sizeof(size_t); s <= N/2; s <<= 1)
      {
        Result r;
        if (measure(c, buf, N, s, &r) != 0)
        {
          fprintf(stderr, "N=%zu s=%zu no se calibro\n", N, s);
          continue;
        }
        fprintf(c->out, "%s,%s,%zu,%zu,%zu,%llu,%.4f,%.4f,%.4f,%d\n",
            SRC_NAME[c->src], PAT_NAME[c->pat], r.N, r.s, r.n_touched,
            r.accesses, r.ns_min, r.ns_mean, r.ns_sd, r.reps);
        fflush(c->out);
      }
      fprintf(stderr, "N=zu KB listo\n", N >> 10);::
    }

  }


}
