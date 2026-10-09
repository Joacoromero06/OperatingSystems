//#ifndef ARENA_H
#include <stddef.h>
#include <stdint.h>
// https://www.dgtlgrove.com/p/untangling-lifetimes-the-arena-allocator
#ifndef ARENA_STDIO
// Input Output
  #include <stdio.h>
  #include <stdarg.h>
// Asserting
  #include <assert.h>
// Useful 
  #include <memory.h>
#endif

#define CAPACITY (1024)
typedef struct Region Region;
struct Region{ // heap region with (8+8+8)bytes of meta data at beggining
  Region* next;
  size_t capacity;
  size_t count;
  uintptr_t data[];
};

typedef struct{
  Region* begin;
  Region* end;
} Arena;


typedef struct {
  Region* region;
  size_t count;
}ArenaMark;

Region* new_region(size_t n);
void free_region(Region* r);



#define ARENA_BACKEND 
#define ARENA_BACKEND_LINUX_MMAP
#define ARENA_BACKEND_LIBC_MALLOC

//#if ARENA_BACKEND == ARENA_BACKEND_LIBC_MALLOC
  #include <stdlib.h>
  #define alloc(type) ((type*) malloc(sizeof(type)))
  #define alloc_bytes(bsize) ((void*) malloc(bsize))

  #define ASSERT_REGION assert 
  Region* new_region(size_t n){
    size_t bsize = sizeof(Region) + sizeof(uintptr_t) * n;
    Region* r = (Region*) alloc_bytes(bsize);
    ASSERT_REGION(r)
    r->next = NULL;
    r->capacity = n;
    r->count = 0;
    return r;
  }

  void free_region(Region* r){
    free(r);
  }


//#elif ARENA_BACKEND == ARENA_BACKEND_LINUX_MMAP
  #include <unistd.h>
  #include <sys/mman.h>
  
  Region* new_region(size_t capacity){
    size_t bsize = sizeof(Region) + sizeof(uintptr_t) * capacity;
    Region* r = mmap(NULL, bsize, PROT_READ | PROT_WRITE,  MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_REGION(r != MAP_FAILED);
    r->next = NULL;
    r->capacity = n;
    r->count = 0;
  }
  void free_region(Region* r){
    size_t bsize = sizeof(Region) + sizeof(uintptr_t) * r->capacity;
    int ret = munmap(r, bsize);
    ASSERT_REGION(ret == 0);
  }

#define TWO_PAGES 4096
void* arena_alloc(Arena* a, size_t bsize)
{
#define p_WSIZE_ENTER(a, wsize) (a->end->count + wsize <= a->end->capacity)
  size_t wsize = (bsize + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);

  if (a->end == NULL)
  { // Si la arena esta vacia se crea el primer "nodo" Region tando begin end apuntan a el
    assert(a->begin == NULL);
    wsize = (wsize < TWO_PAGES)? TWO_PAGES : wsize;

    a->end = new_region(wsize);
    a->begin = a->end;
  }
  // no entiendo porque el repositorio en su algoritmo recorre desde end y no desde begin, tiene mas sentido crear un Region* nav y recorrer mas que actualizar el cursor end
  // Parece que end y begin no funcionan como cursores en lista enlazada para delimitar sino que tiene una semantica distinta
  // Quizas interpreto mal la estructura Arena con su begin end como un iterador de Region
  // En este algoritmo tambien solo se crea un nuevo nodo Region si no cabe wsize desde end hasta end, por eso es raro que significan end y begin
  // Porque cuando no cabe en end crea la nuevas region y actualiza end como el nuevo ultimo entonces nunca se busca en las Region creadas anteriormente
  // Tambien no tenemos control de que hara el usuario con el puntero
  while (!p_WSIZE_ENTER(a, wsize) && a->end->next != NULL)
  {
    a->end = a->end->next;
  }

  if (!p_WSIZE_ENTER(a, wsize))
  {
    assert(a->end->next == NULL);
    wsize = (wsize < TWO_PAGES)? TWO_PAGES : wsize;

    a->end->next = new_region(wsize);
    a->end = a->end->next;
  }

  void* p = & a->end->data[a->end->count];
  a->end->count += wsize;
  return p;
}

void* arena_realloc(Arena* a, void* old_p, size_t old_bsize, size_t new_bsize)
{
#define BSIZE_to_WSIZE(bsize) ((bsize + sizeof(uintptr_t) - 1) / sizeof(uintptr_t))
  if (new_bsize <= old_bsize)
  { // supoongo que el usuario utiliza old_p con old_bsize, UB si no lo hace
    return old_p;
  }
  // enves de encontrar old_p en Regions de Arena 
  // Utiliza el algoritmo de encontrar la primera Region desde end donde quepa o crea uno
  // Ineficiente porque quizas old_p esta en una Region desocupada
  // En ningun caso es necesario usar memmove, memcpy es seguro y correcto
  void* new_p = arena_alloc(a, new_bsize);
  return memcpy(new_p, old_p, BSIZE_to_WSIZE(old_bsize));
}

size_t arena_strlen(const char* s){
/* INPUT: C-string null terminated, with const modifier
 * OUTPUT: length of quantity of charactes in s. Not count '\0'
 *
 * Para un puntero alocado en arena, reinterpretado como char* 
 * Calcula la cantidad de caracteres ASCII.
*/
  size_t len = 0;
  while (*s) len++;
  return len;
}
void* arena_memcpy(void* dest, void* src, size_t size)
{
  /*SYNOPSIS:
   * destination pointer of at least size bytes array
   * source pointer of at least size bytes array
   *DESCRIPTION:
   * Both pointers allocated from Arena Allocator, at least dest from Arena
   * If size is not correct, Undefined Behaviour will occur, accessing,modifying data from other pointers or non a defined memory for the process SEGFAULT
   * If pointers are not safe Segmentation occur due to incorrect dereference
   */
  char* d = dest, *s = src;
  for (;size;size--) *d++ = *s++;
  return dest;
}

char* arena_strdup(Arena* a, const char* s){
  /*SYNOPSIS:
    s es un c-string null terminated, stack, malloc, arena, allocated
   DESCRIPCION:
    Alloca la cantidad necesaria para duplicar los datos de s
    Copia los datos de s
    Retorna el c-string null terminated allocado por Arena
   */
  size_t len = arena_strlen(s);
  char* dup = arena_alloc(a, len+1);
  dup = arena_memcpy(dup, (void*) s, len);
  dup[len] = '\0';
  return dup;
}

ArenaMark arena_snapshot(Arena* a)
{
  ArenaMark m;
  if (a->end == NULL) // clearly a->end == NULL has a menaning but i cant realized one and name it, is like Arena Empty that should assert begin==NULL
  {
    assert(a->begin == NULL);
    m.region = NULL;
    m.count = 0;
  } else { // Guarda la snapshot en una envoltura semantica Mark de una Region que es util para el que la necesita
           // Mark es un puntero y su count actual stack values a valores manejados por el Allocator Arena, me recuerda a vector donde la struct esa en cualquier lenguaje son valores primitivos que se copian pero alk ser punteros a otra memoria es raro
           // No entiendo porque es util o cuando usarlo bien... Osea la idea de heap como un lugar magico es falso... pero ciertamente usar solo stack tambien vuelve la programacion no llevable y Arena Allocator es una solucion efectiva y sencilla pero requiere cambiar un poico como pensamos la memoria que no es stack y mejorar como pensdamos la memoria de stack

    m.region = a->end;
    m.count = a->end->count;
  }
}

void reset_arena(Arena* a)
{
  /* Cleans all memory eliminating all structure of allocation that where done in all regions in the arena
   */
  for (Region* r = a->begin; r != NULL; r = r->next) 
  {
    r->count = 0;
  }
  a->end = a->begin; // So all pages that where requested to map from current process are maintained and because where trigerred or hitted when changing count or accessing next or referencing the region that pages are in RAM until SO swap them. So this resetting is quite strange actually, and because end = begin means that future allocation start looking from begin to the real end, thats fit good in the algorithm, but .. I dont finish understanding this decisions that means if someone allocates 32 bytes and then 4096 bytes and then other 4000 bytes then end is pointing to last Region with count = 4000 and is left more or less 4096 -32 bytes and the complete arena is something like one region with just 64 bytes from 4096 and then second region of 5028 bytes using two pages and then a third region of 4000 bytes that no fits in the lefting bytes and need a new region and if someone wants 64 bytes start looking in the last Region without searchin previous ones. Thas fragmentation really bad no?
}
void arena_rewind(Arena* a, ArenaMark m)
{
  if (m.region == NULL) // snapshot uninintialized i dont undestand why this is need to manage this case or whats means
  {
    reset_arena(a);
    return;
  }

  m.region->count = m.count; // La abstraccion Mark no es propoensa a errores por el count en su region y el count en el wrappe or siginifcan y se usan distintos
  for (Region* r = m.region->next; r != NULL; r = r->next)
  {
    r->count = 0; // porque itera en todas las regiones desde Mark en adelante, poniendo esos buffers de memoria vacios?
  }
  a->end = m.region; // entiendo esto, si bien tenemos toda esa memoria reservada en adelante, el allocator la usara al pone end que es nuestro verdadero unico cursor desde donde buscamos espacio suficiente
}

void arena_free(Arena* a)
{
  // frees al Regions allocated internally in arena during using arena allocator
  Region* r = a->begin;
  while (r != NULL) 
  {
    Region* r0 = r;
    r = r->next;
    free_region(r0);
  }
  a->begin = a->end = NULL;
}
void arena_trim(Arena* a)
{
  Region* r = a->end->next; // que pasa si end es null 
  // la idea parece liberar y liberar esas paginas que el proceso tenia sin usar debido a algun tipo de rewind o reset que como decia eran paginas que quizas eran hiteadas pero realmente no se usaban 
  while (r != NULL)
  {
    Region* r0 = r;
    r = r->next;
    free_region(r);
  }
  a->end->next = NULL;
}
//#else 
//#error "Arena Backend no conocido"
//#endif
