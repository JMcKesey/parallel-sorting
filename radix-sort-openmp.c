// references: https://www.geeksforgeeks.org/radix-sort/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <time.h>

#define g_SIZE 50000000
#define g_MAX_THREADS 16  // the amount of threads

void randomArray(int *arr, int size)
{
  srand(time(NULL));
	for(int i=0; i < size; i++) 
  {
		arr[i] = rand() % 10000000;
	}
}

int getMax(int *arr, int n) {
  int max = arr[0];
  #pragma omp parallel for reduction(max:max)
  for (int i = 1; i < n; i++) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }
  return max;
}

void countingSortParallel(int *arr, int n, int exp) {
  int num_threads;
  int *output = malloc(n * sizeof(int));
  int i, t;

  #pragma omp parallel
  {
    num_threads = omp_get_num_threads();
    // printf("num threads: %d", num_threads);
  }

  int **local_counts = malloc(num_threads * sizeof(int *));
  for (i = 0; i < num_threads; i++) {
    local_counts[i] = calloc(10, sizeof(int));
  }

  #pragma omp parallel
  {
    int tid = omp_get_thread_num();
    int *count = local_counts[tid];

    #pragma omp for nowait
    for (int i = 0; i < n; i++) 
    {
      int digit = (arr[i] / exp) % 10;
      count[digit]++;
    }
  }

  int global_count[10] = {0};
  for (t = 0; t < num_threads; t++) 
  {
    for (i = 0; i < 10; i++) 
    {
      global_count[i] += local_counts[t][i];
    }
  }

  int positions[10];
  positions[0] = 0;
  for (i = 1; i < 10; i++) 
  {
    positions[i] = positions[i - 1] + global_count[i - 1];
  }

  int **thread_offsets = malloc(num_threads * sizeof(int *));
  for (t = 0; t < num_threads; t++) 
  {
    thread_offsets[t] = malloc(10 * sizeof(int));
    memcpy(thread_offsets[t], positions, 10 * sizeof(int));
    for (i = 0; i < 10; i++) 
    {
      for (int tt = 0; tt < t; tt++) 
      {
        thread_offsets[t][i] += local_counts[tt][i];
      }
    }
  }

  #pragma omp parallel
  {
    int tid = omp_get_thread_num();
    int *offsets = thread_offsets[tid];

    #pragma omp for
    for (int i = 0; i < n; i++) 
    {
      int digit = (arr[i] / exp) % 10;
      int pos = offsets[digit]++;
      output[pos] = arr[i];
    }
  }

  memcpy(arr, output, n * sizeof(int));

  for (i = 0; i < num_threads; i++) 
  {
    free(local_counts[i]);
    free(thread_offsets[i]);
  }
  free(local_counts);
  free(thread_offsets);
  free(output);
}

void radixSortParallel(int *arr, int n) {
  int max = getMax(arr, n);

  for (int exp = 1; max / exp > 0; exp *= 10) 
  {
    countingSortParallel(arr, n, exp);
  }
}

int main() {
  int *arr = malloc(g_SIZE * sizeof(int));
	if(arr == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return EXIT_FAILURE;
	}
	randomArray(arr, g_SIZE);

  omp_set_num_threads(g_MAX_THREADS);

  double start_time = omp_get_wtime();
  radixSortParallel(arr, g_SIZE);
  double end_time = omp_get_wtime();

	printf("Elapsed time %f seconds | Array Size %d\n", end_time - start_time, g_SIZE);

  free(arr);
  return 0;
}
