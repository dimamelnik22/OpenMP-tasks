#include <omp.h>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
#include <vector>

std::vector<int> SubStringSearch(const char* string, const char* substring, bool threading);
double runtime = 0;


int main()
{
	/*const char* string = "29149012839082948120830921849021893129456";
	const char* subString = "12";*/
	int strLength = 1000000;
	int subLength = 1000;
	char* string = new char[strLength + 1];
	srand(time(0));
	char* subString = new char[subLength + 1];
	for (int i = 0; i < subLength; ++i)
		subString[i] = 'a' + rand() % 26;
	subString[subLength] = '\0';
	int add = 0;
	int change = 0;
	for (int i = 0; i < strLength; ++i)
	{
		if (rand() % 10000 == 0 && strLength - i > subLength + 1)
		{
			for (int j = 0; j < subLength; ++j)
				string[i + j] = subString[j];
			i += subLength;
			add++;
			if (!(rand() % 2))
			{
				string[i - 1] = 'a' + rand() % 26;
				change++;
			}
		}

		string[i] = 'a' + rand() % 26;
	}
	string[strLength] = '\0';
	//printf("%s\n", string);
	//printf("%s\n", subString);
	std::vector<int> indexes;
	printf("add %d\n", add);
	printf("change %d\n", change);


	indexes = SubStringSearch(string, subString, true);
	/*for (int i = 0; i < indexes.size(); ++i)
		printf("%d\n", indexes[i]);*/
	printf("Parallel time: %f, matches: %d\n", runtime, indexes.size());

	indexes = SubStringSearch(string, subString, false);
	/*for (int i = 0; i < indexes.size(); ++i)
		printf("%d\n", indexes[i]);*/
	printf("Linear time: %f, matches: %d\n", runtime, indexes.size());
	return 0;
}

std::vector<int> SubStringSearch(const char* string, const char* substring, bool threading)
{
	std::vector<int> indexes;
	runtime = omp_get_wtime();
#pragma omp parallel for if(threading)
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