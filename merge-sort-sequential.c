#include <stdio.h>

void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int left_half_count = m - l + 1;
	int right_half_count = r - m;

	int L[left_half_count], R[right_half_count];

	for(i=0; i<left_half_count; i++)
	{
		L[i] = arr[l + i];
	}
	for(j=0; j<right_half_count; j++)
	{
		R[j] = arr[m + 1 + j];
	}

	i=0;
	j=0;
	k=l;
	while(i < left_half_count && j < right_half_count)
	{
		if(L[i] <= R[j])
		{
			arr[k] = L[i];
			i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	while(i < left_half_count)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	while(j < right_half_count)
	{
		arr[k] = R[j];
		j++;
		k++;
	}
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
	int arr[] = { 11, 14, 5, 6, 7 };
	int arr_size = sizeof(arr) / sizeof(arr[0]);

	merge_sort(arr, 0, arr_size-1);

	int i;
	for(i=0; i<arr_size; i++)
	{
		printf("%d\n", arr[i]);
	}

	return 0;
}
