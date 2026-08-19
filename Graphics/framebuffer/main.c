#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  SDL_Init(SDL_INIT_VIDEO);
  printf("Hello SDL multiplattform library is working!\n");
  return EXIT_SUCCESS;

}

