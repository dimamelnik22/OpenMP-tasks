#include <omp.h>
#include <ctime>
#include <cstdlib>
#include <stdio.h>

int** Product(int** A, int** B, int heightA, int size, int lengthB, bool threading);
double runtime = 0;
int numOfThreads = 0;

int main()
{
	srand(time(0));
	
	for (int n = 10; n <= 1000; n *= 10)
	{
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
		
		for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
		{
			C = Product(A, B, heightA, size, lengthB, true);
			printf("%f\n", runtime);
		}
		C = Product(A, B, heightA, size, lengthB, false);
		printf("Linear %f\n", runtime);
	}
	int n = 2000;
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
	
	for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
	{
		C = Product(A, B, heightA, size, lengthB, true);
		printf("%f\n", runtime);
	}
	C = Product(A, B, heightA, size, lengthB, false);
	printf("Linear %f\n", runtime);
	return 0;
}


int** Product(int** A, int** B, int heightA, int size, int lengthB, bool threading)
{
	int** C = new int* [heightA];
	for (int i = 0; i < heightA; ++i)
		C[i] = new int[lengthB];
	runtime = omp_get_wtime();
#pragma omp parallel for if(threading) num_threads(numOfThreads)
	for (int t = 0; t < heightA * lengthB; ++t)
	{
		int i = t / heightA;
		int j = t % heightA;
		C[i][j] = 0;
		for (int k = 0; k < size; k++)
		{
			C[i][j] += A[i][k] * B[k][j];
		}
		
	}
	runtime = omp_get_wtime() - runtime;
	return C;
}