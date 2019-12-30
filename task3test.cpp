#include <omp.h>
#include <ctime>
#include <cstdlib>
#include <stdio.h>

int* ProductRow(int** A, int* vector, int heightA, int lengthA, bool threading);
int* ProductCol(int** A, int* vector, int heightA, int lengthA, bool threading);
int* ProductBlock(int** A, int* vector, int heightA, int lengthA, bool threading);
double runtime = 0;
int numOfThreads =0;

int main()
{
	srand(time(0));
	for (int n = 100; n <= 10000; n *= 10)
	{
		int heightA = n;
		int lengthA = n;
		int** A = new int* [heightA];
		for (int i = 0; i < heightA; ++i)
		{
			A[i] = new int[lengthA];
			for (int j = 0; j < lengthA; ++j)
				A[i][j] = rand() % 100;
		}
		int* vector = new int[heightA];
		for (int i = 0; i < heightA; ++i)
		{
			vector[i] = rand() % 100;
		}
		int* C;
		printf("Parallel row\n");
		for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
		{			
			C = ProductRow(A, vector, heightA, lengthA, true);
			printf("%f\n", runtime);
		}

		printf("Parallel col\n");
		for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
		{
			C = ProductCol(A, vector, heightA, lengthA, true);
			printf("%f\n", runtime);
		}

		printf("Parallel block\n");
		for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
		{
			C = ProductBlock(A, vector, heightA, lengthA, true);
			printf("%f\n", runtime);
		}
		
		C = ProductRow(A, vector, heightA, lengthA, false);
		printf("Linear time: %f\n", runtime);
	}
	int n = 20000;
	int heightA = n;
	int lengthA = n;
	int** A = new int* [heightA];
	for (int i = 0; i < heightA; ++i)
	{
		A[i] = new int[lengthA];
		for (int j = 0; j < lengthA; ++j)
			A[i][j] = rand() % 100;
	}
	int* vector = new int[heightA];
	for (int i = 0; i < heightA; ++i)
	{
		vector[i] = rand() % 100;
	}
	int* C;
	printf("Parallel row\n");
	for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
	{			
		C = ProductRow(A, vector, heightA, lengthA, true);
		printf("%f\n", runtime);
	}

	printf("Parallel row\n");
	for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
	{
		C = ProductCol(A, vector, heightA, lengthA, true);
		printf("%f\n", runtime);
	}

	printf("Parallel block\n");
	for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
	{
		C = ProductBlock(A, vector, heightA, lengthA, true);
		printf("%f\n", runtime);
	}
	
	C = ProductRow(A, vector, heightA, lengthA, false);
	printf("Linear time: %f\n", runtime);

	return 0;
}


int* ProductRow(int** A, int* vector, int heightA, int lengthA, bool threading)
{
	int* res = new int [heightA];
	runtime = omp_get_wtime();
#pragma omp parallel for if(threading) num_threads(numOfThreads) 
	for (int i = 0; i < heightA; ++i)
	{
		int sum = 0;
		for (int k = 0; k < lengthA; ++k)
		{
			sum += A[i][k] * vector[i];
		}
		res[i] = sum;
	}
	runtime = omp_get_wtime() - runtime;
	return res;
}

int* ProductCol(int** A, int* vector, int heightA, int lengthA, bool threading)
{
	int* res = new int[heightA];
	for (int i = 0; i < heightA; ++i)
		res[i] = 0;
	runtime = omp_get_wtime();

#pragma omp parallel for if(threading) num_threads(numOfThreads) 
	for (int i = 0; i < lengthA; ++i)
	{
		for (int k = 0; k < heightA; ++k)
		{
#pragma omp atomic
			res[k] += A[k][i] * vector[k];
		}
	}
	runtime = omp_get_wtime() - runtime;
	return res;
}

int* ProductBlock(int** A, int* vector, int heightA, int lengthA, bool threading)
{
	int* res = new int[heightA];
	for (int i = 0; i < heightA; ++i)
		res[i] = 0;

	runtime = omp_get_wtime();

#pragma omp parallel if (threading) num_threads(numOfThreads) 
	{
		int blockHeight = heightA / omp_get_num_threads();
		int blockWidth = lengthA / omp_get_num_threads();
		int deb = (heightA / blockHeight) * (lengthA / blockWidth);
#pragma omp for
		for (int k = 0; k < (heightA / blockHeight + 1) * (lengthA / blockWidth + 1); ++k)
		{
			int i = k / (heightA / blockHeight + 1);
			int j = k % (heightA / blockHeight + 1);
			for (int y = i * blockHeight; y < (i + 1) * blockHeight && y < heightA; ++y)
			{
				int sum = 0;
				for (int x = j * blockWidth; x < (j + 1) * blockWidth && x < lengthA; ++x)
					sum += A[y][x] * vector[y];
#pragma omp atomic
				res[y] += sum;
			}

		}
	}
	
	runtime = omp_get_wtime() - runtime;
	return res;
}