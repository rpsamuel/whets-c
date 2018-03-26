#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ONE_BILLION 1000000000

int hamming_distance(uint16_t *a, uint16_t *b, int count) {
    int total = 0;
    while(count > 0) {
        if (*a != *b) {
            total += 1;
        }
        a++;
        b++;
        count--;
    }
    return total;
}

unsigned int tmetrics_hamming (unsigned int len, uint16_t *a, uint16_t *b)
{
  unsigned int acc = 0, i;
  for (i = 0; i < len; i++)
    {
      if (*(a + i) != *(b + i)) acc++;
    }
  return acc;
}

uint64_t benchmark_hamming(uint16_t *a, uint16_t *b, int size, int repeats) {
    struct timespec start, end;
    // We use a black hole here so that our benchmark code will not get dead code eliminated
    int64_t black_hole = 0;
    int64_t total_nanoseconds = 0;

    // Run it once to reduce the effects of caching
    int dist = hamming_distance(a, b, size);
    printf("%d", dist);

    int i;
    for(i = 0; i < repeats; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        black_hole += hamming_distance(a, b, size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        total_nanoseconds += ONE_BILLION*(end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    }
    printf("%ld\n", black_hole);
    return total_nanoseconds;
}

void initialiseArray(uint16_t *a, uint16_t *b, int size) {
    int i;
    for (i = 0; i < size; i++) {
        a[i] = i;
        b[i] = i;
    }
}

void benchmark_hamming_length(int size) {
    const int repeats = 1000;
    uint16_t *a = malloc(size*sizeof(uint16_t));
    uint16_t *b = malloc(size*sizeof(uint16_t));

    initialiseArray(a, b, size);

    uint64_t nanoseconds = benchmark_hamming(a, b, size, repeats);
    printf("String Size: %d characters\n", size);
    printf("Time taken: %ld seconds and %ld microseconds\n", nanoseconds/ONE_BILLION, (nanoseconds % ONE_BILLION)/1000);
    uint64_t average_time = nanoseconds / repeats;
    printf("Average time taken: %ld seconds and %ld microseconds\n", average_time/ONE_BILLION, (average_time % ONE_BILLION)/1000);
    free(a);
    free(b);
}


int main () {
    benchmark_hamming_length(100000);
    benchmark_hamming_length(1000000);
    benchmark_hamming_length(10000000);
    benchmark_hamming_length(100000000);
    benchmark_hamming_length(1000000000);
}
