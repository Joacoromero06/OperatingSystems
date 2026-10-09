#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

void read_page_table_info();

int main()
{
  size_t page_size = sysconf(_SC_PAGESIZE);
  printf("page size in this architecture %zu\n", page_size);

  read_page_table_info();
  return 0;
}

void read_page_table_info()
{
  FILE* f = fopen("/proc/self/status", "r");
  assert(f != NULL);

  char b[256]; // buffer for save lines readen
  while (fgets(b, sizeof(b), f) != NULL) // != EOF
  {
    printf("%s", b);
    if (strncmp(b, "VmPTE:", 6) == 0)
    {
      printf("\nMemory used by specific process %s\n\n", b+6);
    }
  }
}
