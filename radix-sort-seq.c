#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define g_SIZE 50000000

double time_diff(struct timespec *start, struct timespec *end) {
  return (end->tv_sec - start->tv_sec) + (end->tv_nsec - start->tv_nsec) / 1e9;
}

void randomArray(int *arr, int size) 
{
  srand(time(NULL));
  for (int i = 0; i < size; i++) 
  {
    arr[i] = rand() % 10000000;
  }
}

int getMax(int *arr, int n) 
{
  int max = arr[0];
  for (int i = 1; i < n; i++) 
  {
    if (arr[i] > max) {
      max = arr[i];
    }
  }
  return max;
}

void countingSort(int *arr, int n, int exp) 
{
  int *output = malloc(n * sizeof(int));
  int count[10] = {0};

  for (int i = 0; i < n; i++) 
  {
    int digit = (arr[i] / exp) % 10;
    count[digit]++;
  }

  for (int i = 1; i < 10; i++) 
  {
    count[i] += count[i - 1];
  }

  for (int i = n - 1; i >= 0; i--) 
  {
    int digit = (arr[i] / exp) % 10;
    output[count[digit] - 1] = arr[i];
    count[digit]--;
  }

  memcpy(arr, output, n * sizeof(int));
  free(output);
}

void radixSort(int *arr, int n) 
{
  int max = getMax(arr, n);

  for (int exp = 1; max / exp > 0; exp *= 10) 
  {
    countingSort(arr, n, exp);
  }
}

int main() {
  struct timespec start, end;
  int *arr = malloc(g_SIZE * sizeof(int));
  if (arr == NULL) 
  {
    fprintf(stderr, "Memory allocation failed\n");
    return EXIT_FAILURE;
  }

  randomArray(arr, g_SIZE);

  clock_gettime(CLOCK_MONOTONIC, &start);
  radixSort(arr, g_SIZE);
  clock_gettime(CLOCK_MONOTONIC, &end);

  double elapsed = time_diff(&start, &end);
  printf("Elapsed time %f seconds | Array Size %d\n", elapsed, g_SIZE);

  free(arr);
  return 0;
}
