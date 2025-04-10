#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define g_SIZE 50000000

int compare(const void* a, const void* b)
{
  return (*(int*)a - *(int*)b);
}

void randomArray(int *arr, int size)
{
	srand(time(NULL));
	for(int i=0; i < size; ++i) {
		arr[i] = rand() % 10000000;
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

	clock_t begin = clock();
  qsort(arr, g_SIZE, sizeof(int), compare); 
	clock_t end = clock();

	double time_elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Elapsed time %f seconds\n", time_elapsed);

  return 0;
}