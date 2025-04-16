#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define g_SIZE 50000000

double time_diff(struct timespec *start, struct timespec *end) {
  return (end->tv_sec - start->tv_sec) + (end->tv_nsec - start->tv_nsec) / 1e9;
}

void randomArray(int *arr, int size)
{
	srand(time(NULL));
	for(int i=0; i < size; ++i) {
		arr[i] = rand() % 10000000;
	}
}

void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int left_half_count = m - l + 1;
	int right_half_count = r - m;

	int *L = malloc(left_half_count * sizeof(int));
	int *R = malloc(right_half_count * sizeof(int));

	if (L == NULL || R == NULL)
	{
		fprintf(stderr, "Memory allocation failed in merge\n");
		exit(EXIT_FAILURE);
	}

	for(i=0; i<left_half_count; i++)
	{
		L[i] = arr[l + i];
	}
	for(j=0; j<right_half_count; j++)
	{
		R[j] = arr[m + 1 + j];
	}

	i=0; j=0; k=l;
	while(i < left_half_count && j < right_half_count)
	{
		if(L[i] <= R[j])
			arr[k++] = L[i++];
		else
			arr[k++] = R[j++];
	}
	while(i < left_half_count)
		arr[k++] = L[i++];
	while(j < right_half_count)
		arr[k++] = L[j++];

	free(L);
	free(R);
}

void merge_sort(int arr[], int l, int r)
{
	if(l < r)
	{
		int m = l + (r - l) / 2;
		merge_sort(arr, l, m);
		merge_sort(arr, m+1, r);
		merge(arr, l, m, r);
	}
}

int main()
{
  struct timespec start, end;
	int *arr = malloc(g_SIZE * sizeof(int));
	if(arr == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return EXIT_FAILURE;
	}
	randomArray(arr, g_SIZE);

  clock_gettime(CLOCK_MONOTONIC, &start);
	merge_sort(arr, 0, g_SIZE-1);
  clock_gettime(CLOCK_MONOTONIC, &end);

	// correctness check
	// for(int i=1; i<g_SIZE-1; ++i)
	// {
	// 	if(arr[i] < arr[i-1]) return EXIT_FAILURE;
	// 	else if(i==g_SIZE-2) printf("completely correct\n");
	// }

  double elapsed = time_diff(&start, &end);
	printf("Elapsed time %f seconds | Array Size %d\n", elapsed, g_SIZE);

	return 0;
}
