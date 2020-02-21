#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>
#include <map>

#define TRACE (std::cout << "[line " << __LINE__ << "] ")
#define trace(what)(TRACE << #what " = " << (what) << std::endl)
#define lintf(fmt, ...)(printf("[line %d] " fmt, __LINE__, __VA_ARGS__))

#define FOR(i, to) for (int i = 0; i < to; ++i)\

//#define PARTONE

using Map = std::map<std::pair<int, int>, int>;
static Map Grid0;
static Map Grid1;

static void amend(int &x, int &y, char dir, int amt, Map &grid, int &count)
{
	switch (dir) {
	case 'D':
		for (const int target = y - amt; y > target; --y) {
			grid.emplace(std::make_pair(x, y), count++);
		}
		break;
	case 'U':
		for (const int target = y + amt; y < target; ++y) {
			grid.emplace(std::make_pair(x, y), count++);
		}
		break;
	case 'R':
		for (const int target = x + amt; x < target; ++x) {
			grid.emplace(std::make_pair(x, y), count++);
		}
		break;
	case 'L':
		for (const int target = x - amt; x > target; --x) {
			grid.emplace(std::make_pair(x, y), count++);
		}
		break;
	default:
		TRACE << "no such dir " << dir << std::endl;
		abort();
	}
}

static inline void go(FILE *f)
{
	char dir;
	int amt;
	for (int i = 1; ; ++i) {
		if (fscanf(f, "%c%d", &dir, &amt) != 2)
			return;
		int x = 0, y = 0;
		Map &grid = (i == 1) ? Grid0 : Grid1;
		int count = 1;
		amend(x, y, dir, amt, grid, count);
		while (fscanf(f, ",%c%d", &dir, &amt) == 2) {
			amend(x, y, dir, amt, grid, count);
		}
		fscanf(f, "\n");
	}
}

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
	Grid0.erase({ 0, 0 });
	Grid1.erase({ 0, 0 });
#ifdef PARTONE
	int closest = 0x3f3f3f3f;
	for (auto item : Grid0) {
		if (Grid1.find(item.first) == Grid1.end())
			continue;
		closest = std::min(closest, std::abs(item.first.first) + std::abs(item.first.second));
	}
	trace(closest);
#else
	int best = 0x3f3f3f3f;
	for (auto one : Grid0) {
		auto two = Grid1.find(one.first);
		if (two == Grid1.end())
			continue;
		best = std::min(best, one.second + two->second);
	}
	trace(best - 2);
#endif

}
