#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define g_SIZE 50000000						// array size
#define g_MAX_THREADS 50			  // the amount of threads
#define g_MIN_TASK_SIZE 2000000		  // min size to not use threads

void randomArray(int *arr, int size)
{
	srand(time(NULL));
	for(int i=0; i < size; i++) {
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
		arr[k++] = R[j++];

	free(L);
	free(R);
}

void merge_sort(int arr[], int l, int r)
{
	if(l < r)
	{
		int m = l + (r - l) / 2;

		if ((r - l + 1) <= g_MIN_TASK_SIZE) {
			merge_sort(arr, l, m);
			merge_sort(arr, m+1, r);
		} else {
			#pragma omp task shared(arr)
			merge_sort(arr, l, m);

			#pragma omp task shared(arr)
			merge_sort(arr, m+1, r);

			#pragma omp taskwait
		}
		merge(arr, l, m, r);
	}
}

int main()
{
	int *arr = malloc(g_SIZE * sizeof(int));
	if(arr == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return EXIT_FAILURE;
	}
	randomArray(arr, g_SIZE);

	omp_set_num_threads(g_MAX_THREADS);

	double start_time = omp_get_wtime();
	#pragma omp parallel
	{
		#pragma omp single
		{
			merge_sort(arr, 0, g_SIZE - 1);
		}
	}

	double end_time = omp_get_wtime();
	printf("Elapsed time %f seconds | Array Size %d\n", end_time - start_time, g_SIZE);

	free(arr);
	return 0;
}
