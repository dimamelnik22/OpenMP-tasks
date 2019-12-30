#include <omp.h>
#include <iostream>
#include <ctime>

int GetMax(int* arr, int length, bool threading);
int GetMin(int* arr, int length);
int MaxSearch(int** arr, int height, int length, bool threading);
double runtime = 0;

int main()
{
	srand(time(0));
	int length = 1000;
	int height = 1000;
	int** arr = new int* [height];
	for (int i = 0; i < height; ++i)
	{
		arr[i] = new int[length];
		for (int j = 0; j < length; ++j)
			arr[i][j] = rand() % 10000;
	}

	int max = MaxSearch(arr, height, length, true);
	printf("Parallel result: %d  ", max);
	printf("Parallel time: %f\n", runtime);
	max = MaxSearch(arr, height, length, false);
	printf("Linear result: %d  ", max);
	printf("Linear time: %f\n", runtime);

	return 0;
}

int MaxSearch(int** arr, int height, int length, bool threading)
{
	int* minArr = new int[height];
	runtime = omp_get_wtime();
#pragma omp parallel for if(threading)
	for (int i = 0; i < height; ++i)
	{
		int tmp = GetMin(arr[i], length);
#pragma omp critical
		{
			minArr[i] = tmp;
		}
	}
	runtime = omp_get_wtime() - runtime;
	int max = GetMax(minArr, height, threading);
	return max;
}

int GetMax(int* arr, int length, bool threading)
{
	omp_lock_t maxLock;
	int max = arr[0];
	omp_init_lock(&maxLock);
#pragma omp parallel for if (threading)
	for (int i = 0; i < length; ++i)
		if (arr[i] > max)
		{
			omp_set_lock(&maxLock);
			max = arr[i];
			omp_unset_lock(&maxLock);
		}
	omp_destroy_lock(&maxLock);
	return max;
}

int GetMin(int* arr, int length)
{
	int min = arr[0];
	for (int i = 0; i < length; ++i)
		if (arr[i] < min)
			min = arr[i];
	return min;
}