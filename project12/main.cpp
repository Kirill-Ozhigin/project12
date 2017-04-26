#include <stdio.h>


int main(const int argc, const char* const argv[])
{
	printf("hello");

	for (int i = 1; i < argc; ++i)
	{
		printf(", ");
		printf(argv[i]);
	}

	printf("!\n");

	return 0;
}
