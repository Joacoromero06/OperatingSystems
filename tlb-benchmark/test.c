#include <stdio.h>
#include <stdint.h>

static uint64_t rng_state = 8ULL;
uint64_t xorshift64() {
  uint64_t x = rng_state;
  x ^= x << 13; x ^= x >> 7; x ^= x << 17;
  return rng_state = x;
}
int main()
{
  uint64_t x = 8;
  printf("x << 13 = %ld\n", x ^= x << 13);
  printf("x >> 7  = %ld\n", x ^= x >> 7);
  printf("x << 17 = %ld\n", x ^= x << 17);

  printf("xorshift64(x) = %ld\n", xorshift64());
  printf("xorshift64(x) = %ld\n", xorshift64());
  printf("xorshift64(x) = %ld\n", xorshift64());
}
