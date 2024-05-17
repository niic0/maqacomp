#include <stdint.h>

uint64_t rdtsc() {
   uint64_t x;
   __asm__ volatile ("rdtsc" : "=A" (x));
   return x;
}
