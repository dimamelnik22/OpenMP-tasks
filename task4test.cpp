#include <omp.h>
#include <ctime>
#include <cstdlib>
#include <stdio.h>

int** ProductLine(int** A, int** B, int heightA, int size, int lengthB, bool threading);
int** ProductBlock(int** A, int** B, int heightA, int size, int lengthB, bool threading);
double runtime = 0;
int numOfThreads =0;

int main()
{
	srand(time(0));
	int n = 100;
	int heightA = n;
	int size = n;
	int lengthB = n;
	int** A = new int* [heightA];
	for (int i = 0; i < heightA; ++i)
	{
		A[i] = new int[size];
		for (int j = 0; j < size; ++j)
			A[i][j] = rand() % 100;
	}
	int** B = new int* [size];
	for (int i = 0; i < size; ++i)
	{
		B[i] = new int[lengthB];
		for (int j = 0; j < lengthB; ++j)
			B[i][j] = rand() % 100;
	}
	int** C;
	printf("Parallel line\n");
	for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
	{
		C = ProductLine(A, B, heightA, size, lengthB, true);
		printf("%f\n", runtime);
	}
	printf("Parallel block\n");
	for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
	{
		C = ProductBlock(A, B, heightA, size, lengthB, true);
		printf("%f\n", runtime);
	}
	C = ProductLine(A, B, heightA, size, lengthB, false);
	printf("Linear time: %f\n", runtime);

	n = 500;
	heightA = n;
	size = n;
	lengthB = n;
	A = new int* [heightA];
	for (int i = 0; i < heightA; ++i)
	{
		A[i] = new int[size];
		for (int j = 0; j < size; ++j)
			A[i][j] = rand() % 100;
	}
	B = new int* [size];
	for (int i = 0; i < size; ++i)
	{
		B[i] = new int[lengthB];
		for (int j = 0; j < lengthB; ++j)
			B[i][j] = rand() % 100;
	}
	C;
	printf("Parallel line\n");
	for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
	{
		C = ProductLine(A, B, heightA, size, lengthB, true);
		printf("%f\n", runtime);
	}
	printf("Parallel block\n");
	for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
	{
		C = ProductBlock(A, B, heightA, size, lengthB, true);
		printf("%f\n", runtime);
	}
	C = ProductLine(A, B, heightA, size, lengthB, false);
	printf("Linear time: %f\n", runtime);

	n = 1000;
	heightA = n;
	size = n;
	lengthB = n;
	A = new int* [heightA];
	for (int i = 0; i < heightA; ++i)
	{
		A[i] = new int[size];
		for (int j = 0; j < size; ++j)
			A[i][j] = rand() % 100;
	}
	B = new int* [size];
	for (int i = 0; i < size; ++i)
	{
		B[i] = new int[lengthB];
		for (int j = 0; j < lengthB; ++j)
			B[i][j] = rand() % 100;
	}
	C;
	printf("Parallel line\n");
	for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
	{
		C = ProductLine(A, B, heightA, size, lengthB, true);
		printf("%f\n", runtime);
	}
	printf("Parallel block\n");
	for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
	{
		C = ProductBlock(A, B, heightA, size, lengthB, true);
		printf("%f\n", runtime);
	}
	C = ProductLine(A, B, heightA, size, lengthB, false);
	printf("Linear time: %f\n", runtime);
	return 0;
}


int** ProductLine(int** A, int** B, int heightA, int size, int lengthB, bool threading)
{
	int** C = new int* [heightA];
	for (int i = 0; i < heightA; ++i)
		C[i] = new int[lengthB];
	
	runtime = omp_get_wtime();
#pragma omp parallel if(threading) num_threads(numOfThreads) 
	{
		int lineWidth = heightA / omp_get_num_threads();
#pragma omp for
		for (int t = 0; t < heightA; t+=lineWidth)
		{
			for (int i = 0; i < lineWidth && (t+i) < heightA; ++i)
			{
				for (int j = 0; j < lengthB; ++j)
				{
					C[i + t][j] = 0;
					for (int k = 0; k < size; k++)
					{
						C[i + t][j] += A[i + t][k] * B[k][j];
					}
				}
			}
		}
	}
	runtime = omp_get_wtime() - runtime;
	return C;
}

int** ProductBlock(int** A, int** B, int heightA, int size, int lengthB, bool threading)
{
	int** C = new int* [heightA];
	for (int i = 0; i < heightA; ++i)
		C[i] = new int[lengthB];

	runtime = omp_get_wtime();
#pragma omp parallel if(threading) num_threads(numOfThreads) 
	{
		int blockHeight = heightA / omp_get_num_threads();
		int blockWidth = lengthB / omp_get_num_threads();
#pragma omp for
		for (int t = 0; t < (heightA / blockHeight + 1) * (lengthB / blockWidth + 1); ++t)
		{
			int rowIndex = t / (heightA / blockHeight + 1);
			int colIndex = t % (heightA / blockHeight + 1);
			for (int i = 0; i < blockHeight && (rowIndex * blockHeight + i) < heightA; ++i)
			{
				for (int j = 0; j < blockWidth && (colIndex * blockWidth + j) < lengthB; ++j)
				{
					C[rowIndex * blockHeight + i][colIndex * blockWidth + j] = 0;
					for (int k = 0; k < size; k++)
					{
						C[rowIndex * blockHeight + i][colIndex * blockWidth + j] += A[rowIndex * blockHeight + i][k] * B[k][colIndex * blockWidth + j];
					}
				}
			}
		}
	}
	runtime = omp_get_wtime() - runtime;
	return C;
}