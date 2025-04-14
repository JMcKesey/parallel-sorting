#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define g_SIZE 50000000
#define g_MIN_TASK_SIZE 2000000
#define g_MAX_DEPTH 16

double time_diff(struct timespec *start, struct timespec *end) {
  return (end->tv_sec - start->tv_sec) + (end->tv_nsec - start->tv_nsec) / 1e9;
}

typedef struct {
  int *arr;
  int l;
  int r;
  int depth; // tracks recursion depth
} sort_params;

void merge_sort_seq(int arr[], int l, int r);

void merge(int arr[], int l, int m, int r)
{
  int i, j, k;
  int left_count = m - l + 1;
  int right_count = r - m;

  int *L = malloc(left_count * sizeof(int));
  int *R = malloc(right_count * sizeof(int));
  if(L == NULL || R == NULL)
  {
    fprintf(stderr, "Memory allocation failed in merge\n");
    exit(EXIT_FAILURE);
  }

  for(i = 0; i < left_count; i++)
  {
    L[i] = arr[l + i];
  }
  for(j = 0; j < right_count; j++)
  {
    R[j] = arr[m + 1 + j];
  }

  i = 0; j = 0; k = l;
  while(i < left_count && j < right_count)
  {
    if(L[i] <= R[j])
      arr[k++] = L[i++];
    else
      arr[k++] = R[j++];
  }
  while(i < left_count)
    arr[k++] = L[i++];
  while(j < right_count)
    arr[k++] = R[j++];

  free(L);
  free(R);
}

void merge_sort_seq(int arr[], int l, int r)
{
  if(l<r)
  {
    int m = l + (r-l) / 2;
    merge_sort_seq(arr, l, m);
    merge_sort_seq(arr, m+1, r);
    merge(arr, l, m, r);
  }
}

void *merge_sort_parallel(void *arg)
{
  sort_params *params = (sort_params *)arg;
  int l = params->l, r = params->r, depth = params->depth;
  int *arr = params->arr;

  if (r - l + 1 <= g_MIN_TASK_SIZE || depth >= g_MAX_DEPTH)
  {
    merge_sort_seq(arr, l, r);
    return NULL;
  }

  int m = l + (r - l) / 2;
  pthread_t tid_left, tid_right;

  sort_params *left_params = malloc(sizeof(sort_params));
  if(!left_params) {perror("malloc"); exit(EXIT_FAILURE);};
  left_params->arr = arr;
  left_params->l = l;
  left_params->r = m;
  left_params->depth = depth + 1;

  sort_params *right_params = malloc(sizeof(sort_params));
  if(!right_params) {perror("malloc"); exit(EXIT_FAILURE);};
  right_params->arr = arr;
  right_params->l = m+1;
  right_params->r = r;
  right_params->depth = depth + 1;
  
  if(pthread_create(&tid_left, NULL, merge_sort_parallel, left_params) != 0)
  {
    perror("Failed to create thread for left half");
    exit(EXIT_FAILURE);
  }

  if(pthread_create(&tid_right, NULL, merge_sort_parallel, right_params) != 0)
  {
    perror("Failed to create thread for right half");
    exit(EXIT_FAILURE);
  }
  
  pthread_join(tid_left, NULL);
  pthread_join(tid_right, NULL);

  merge(arr, l, m, r);
  return NULL;
}

void randomArray(int *arr, int size)
{
  srand(time(NULL));
  for(int i=0; i < size; i++)
  {
    arr[i] = rand() % g_SIZE;
  }
}

int main()
{
  struct timespec start, end;
  int *arr = malloc(g_SIZE * sizeof(int));
  if(arr == NULL)
  {
    fprintf(stderr, "Memory allocation failed\n");
    return EXIT_FAILURE;
  }
  randomArray(arr, g_SIZE);

  sort_params *params = malloc(sizeof(sort_params));
  if(!params)
  {
    perror("malloc");
    free(arr);
    return EXIT_FAILURE;
  }
  params->arr = arr;
  params->l = 0;
  params->r = g_SIZE - 1;
  params->depth = 0;

  clock_gettime(CLOCK_MONOTONIC, &start);
  merge_sort_parallel(params);
  clock_gettime(CLOCK_MONOTONIC, &end);

  double elapsed = time_diff(&start, &end);
	printf("Elapsed time %f seconds | Array Size %d\n", elapsed, g_SIZE);

  free(arr);
  return 0;
}