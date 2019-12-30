#include <omp.h>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
#include <vector>

std::vector<int> SubStringSearch(const char* string, const char* substring, bool threading);
double runtime = 0;
int numOfThreads =0;


int main()
{
	srand(time(0));
	for (int n = 1; n <= 1000; n *= 10)
	{
		int strLength = 1000 * n;
		int subLength = n;
		char* string = new char[strLength + 1];

		char* subString = new char[subLength + 1];
		for (int i = 0; i < subLength; ++i)
			subString[i] = 'a' + rand() % 26;
		subString[subLength] = '\0';
		for (int i = 0; i < strLength; ++i)
		{
			if (rand() % 10000 == 0 && strLength - i > subLength + 1)
			{
				for (int j = 0; j < subLength; ++j)
					string[i + j] = subString[j];
				i += subLength;
				if (!(rand() % 2))
				{
					string[i - 1] = 'a' + rand() % 26;
				}
			}

			string[i] = 'a' + rand() % 26;
		}
		string[strLength] = '\0';
		std::vector<int> indexes;

		for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
		{
			indexes = SubStringSearch(string, subString, true);
			printf("%f\n", runtime);
		}
		indexes = SubStringSearch(string, subString, false);
		printf("Linear time: %f, matches: %d\n", runtime, indexes.size());
	}
	int n = 2000;
	int strLength = 1000 * n;
	int subLength = n;
	char* string = new char[strLength + 1];

	char* subString = new char[subLength + 1];
	for (int i = 0; i < subLength; ++i)
		subString[i] = 'a' + rand() % 26;
	subString[subLength] = '\0';
	for (int i = 0; i < strLength; ++i)
	{
		if (rand() % 10000 == 0 && strLength - i > subLength + 1)
		{
			for (int j = 0; j < subLength; ++j)
				string[i + j] = subString[j];
			i += subLength;
			if (!(rand() % 2))
			{
				string[i - 1] = 'a' + rand() % 26;
			}
		}

		string[i] = 'a' + rand() % 26;
	}
	string[strLength] = '\0';
	std::vector<int> indexes;

	for (numOfThreads = 1; numOfThreads <= 16; ++numOfThreads)
	{
		indexes = SubStringSearch(string, subString, true);
		printf("%f\n", runtime);
	}
	indexes = SubStringSearch(string, subString, false);
	printf("Linear time: %f, matches: %d\n", runtime, indexes.size());
	return 0;
}

std::vector<int> SubStringSearch(const char* string, const char* substring, bool threading)
{
	std::vector<int> indexes;
	runtime = omp_get_wtime();
#pragma omp parallel for if(threading) num_threads(numOfThreads)
	for (int i = 0; i < strlen(string) - strlen(substring) + 1; ++i)
	{
		bool match = true;
		for (int j = 0; j < strlen(substring); ++j)
		{
			if (string[i + j] != substring[j])
			{
				match = false;
				break;
			}
		}
		if (match)
		{
#pragma omp critical
			indexes.push_back(i);
		}
	}
	runtime = omp_get_wtime() - runtime;
	return indexes;
}