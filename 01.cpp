#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <algorithm>

#define PARTONE

#define trace(what)(std::cout << "[line " << __LINE__ << "] " #what " = " << what << std::endl)
#define lintf(fmt, ...)(printf("[line %d] " fmt, __LINE__, __VA_ARGS__))

#define FOR(i, to) for (int i = 0; i < to; ++i)


#ifdef PARTONE

static inline void go(FILE *f)
{
	int sum = 0;
	int V;
	while (fscanf(f, "%d", &V) == 1) {
		for (; ; ) {
			V = (V / 3) - 2;
			if (V <= 0)
				break;
			sum += V;
		}
	}
	trace(sum);
}

#else

static inline void go(FILE *f)
{
	int sum = 0;
	int V;
	while (fscanf(f, "%d", &V) == 1) {
		sum += (V / 3) - 2;
	}
	trace(sum);
}

#endif

int main(int argc, char **argv)
{
	const char *fname = "in.txt";
	if (argc >= 2)
		fname = argv[1];
	FILE *f = fopen(fname, "r");
	if (f == nullptr) {
		trace("file cannot be opened");
		exit(1);
	}
	go(f);
	fclose(f);
}
