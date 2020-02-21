#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>

#define TRACE (std::cout << "[line " << __LINE__ << "] ")
#define trace(what)(TRACE << #what " = " << (what) << std::endl)
#define lintf(fmt, ...)(printf("[line %d] " fmt, __LINE__, __VA_ARGS__))

#define FOR(i, to) for (int i = 0; i < to; ++i)

//#define PARTONE

#ifndef PARTONE
static void freq(int n, int *v)
{
	char buf[10];
	sprintf(buf, "%d", n);
	int count = 1;
	for (int i = 0; i < 5; ++i) {
		if (buf[i] == buf[i + 1]) {
			++count;
		} else {
			*v = count;
			count = 1;
			++v;
		}
	}
	*v = count;
}
#endif

static bool check(int n)
{
	bool okay = false;
#ifndef PARTONE
	int v[6];
	freq(n, v);
	for (int i = 0; i < 6; ++i) {
		if (v[i] == 2)
			okay = true;
	}
#else
	char buf[10];
	sprintf(buf, "%d", n);
	for (int i = 0; i < 5; ++i) {
		if (buf[i] == buf[i + 1])
			okay = true;
	}
#endif
	if (! okay)
		return false;
	int prev = 99;
	while (n) {
		const int digit = n % 10;
		n /= 10;
		if (digit > prev)
			return false;
		prev = digit;
	}
	return true;
}

int main()
{
	constexpr int low = 271973;
	constexpr int high = 785961;
	int count = 0;
	for (int i = low; i <= high; ++i) {
		if (check(i))
			++count;
	}
	trace(count);
}
