#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Taille des matrices
#define SIZE 150

extern uint64_t rdtsc();

extern uint64_t perf_event_count(uint64_t n, float a[n][n], float b[n][n], float x);

extern void kernel (unsigned n, float a[n][n], float b[n][n], float x);


static void init_array (int x, int y, float a[x][y]) {
  int i, j;

  for (i=0; i<x; i++)
    for (j=0; j<y; j++)
      a[i][j] = (float) rand() / RAND_MAX;
}

int main(int argc, char **argv) {
  uint64_t cycles, nb_inner_iters;

  /* allocate arrays */
  float (*a)[SIZE] = malloc (SIZE * SIZE * sizeof a[0][0]);
  float (*b)[SIZE] = malloc (SIZE * SIZE * sizeof b[0][0]);

  /* init arrays */
  srand(0);
  init_array(SIZE, SIZE, a);      
  init_array(SIZE, SIZE, b);

  /* init x */
  float x = (float) rand() / RAND_MAX;

  /* Mesurer les cycles avec perf_event */
  cycles = perf_event_count(SIZE, a, b, x);
  nb_inner_iters = SIZE * SIZE * SIZE;
    
  printf ("%lu perf_event-cycles (%.2f per inner-iter)\n",
	  cycles, (float) cycles / nb_inner_iters);

  /* Mesurer les cycles avec rdtsc */
  const uint64_t t1 = rdtsc();
  kernel (SIZE, a, b, x);
  const uint64_t t2 = rdtsc();

  uint64_t tdiff = t2 - t1;
  
  printf ("%lu RDTSC-cycles (%.2f per inner-iter)\n",
	  tdiff, (float) tdiff / nb_inner_iters);


  return 0;
}
