#ifndef CACHEUTILS_H
#define CACHEUTILS_H

#ifndef HIDEMINMAX
#define MAX(X,Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X,Y) (((X) < (Y)) ? (X) : (Y))
#endif


/*
Correct usage of the rdtscp instruction.
clang produces faulty code with the -O3 flag set using only the inline assembly of rdtsc.
The parameter of the maccess function is pushed into rcx before executing the rdtscp instruction
and therefore the rdtscp instruction clears the rcx register and the movq instruction trys then to
access a null pointer.
*/
uint64_t rdtscp(uint32_t *aux) {
  uint64_t a, d;
  asm volatile("rdtscp" : "=a" (a), "=d" (d), "=c" (*aux) : : );
  a = (d << 32) | a;
  return a;
}

uint64_t rdtscp_noaux() {
  uint64_t a, d;
  uint32_t aux;
  asm volatile("rdtscp" : "=a" (a), "=d" (d), "=c" (aux) : : );
  a = (d << 32) | a;
  return a;
}

uint64_t rdtsc_nofence() {
  uint64_t a, d;
  asm volatile ("rdtsc" : "=a" (a), "=d" (d));
  a = (d<<32) | a;
  return a;
}

uint64_t rdtsc() {
  uint64_t a, d;
  asm volatile ("mfence");
  asm volatile ("rdtsc" : "=a" (a), "=d" (d));
  a = (d<<32) | a;
  asm volatile ("mfence");
  return a;
}

void maccess(void* p)
{
  asm volatile ("movq (%0), %%rax\n"
    :
    : "c" (p)
    : "rax");
}

void flush(void* p) {
    asm volatile ("clflush 0(%0)\n"
      :
      : "c" (p)
      : "rax");
}

void prefetch(void* p)
{
  asm volatile ("prefetcht1 %0" : : "m" (p));
}

void longnop()
{
  asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
                "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
                "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
                "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
                "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
                "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
                "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
                "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");
}
#endif
