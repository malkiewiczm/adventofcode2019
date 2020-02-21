#include "template.hpp"

//#define PARTONE

using Coord = std::pair<int, int>;

static Coord START;
static int W, H;
static char Grid[128][128];
#ifdef PARTONE
static int TotalKeys;
#else
static int TotalKeys[4];
#endif
static Coord Doors[26];

#define sign(x) ((x) < 0)

static inline int setbit(int v, int i)
{
	return v | (1 << i);
}

static inline int get_quad(const int y, const int x)
{
	if (y < START.first) {
		if (x < START.second) {
			return 0;
		} else {
			return 1;
		}
	} else {
		if (x < START.second) {
			return 2;
		} else {
			return 3;
		}
	}
}

static inline void m_print_grid(const int line)
{
	std::cout << "[line " << line << "]" << std::endl;
	FOR(y, H) {
		FOR(x, W) {
			putchar(Grid[y][x]);
		}
		putchar('\n');
	}
}

#define print_grid() (m_print_grid(__LINE__))

static inline void go(FILE *f)
{
	H = 0;
	while (fgets(Grid[H], 128, f)) {
		++H;
	}
	W = strlen(Grid[0]) - 1;
#ifdef PARTONE
	TotalKeys = 0;
#else
	FOR(i, 4) {
		TotalKeys[i] = 0;
	}
#endif
	FOR(i, 26) {
		Doors[i].first = 127;
		Doors[i].second = i;
	}
	FOR(y, H) {
		FOR(x, W) {
			const char c = Grid[y][x];
			if (c == '@') {
				START.first = y;
				START.second = x;
				Grid[y][x] = '.';
			}
#ifdef PARTONE
			else if (c >= 'a' && c <= 'z') {
				++TotalKeys;
			}
#endif
			else if (c >= 'A' && c <= 'Z') {
				Doors[c - 'A'] = { y, x };
			}
		}
	}
#ifndef PARTONE
	FOR(y, H) {
		FOR(x, W) {
			if (Grid[y][x] >= 'a' && Grid[y][x] <= 'z') {
				++TotalKeys[get_quad(y, x)];
			}
		}
	}
	const int dy[5] = { 0, 1, -1, 0, 0 };
	const int dx[5] = { 0, 0, 0, 1, -1 };
	FOR(i, 5) {
		Grid[START.first + dy[i]][START.second + dx[i]] = '#';
	}
	FOR(y, H) {
		FOR(x, W) {
			if (Grid[y][x] >= 'a' && Grid[y][x] <= 'z') {
				Coord &door = Doors[Grid[y][x] - 'a'];
				if (get_quad(door.first, door.second) != get_quad(y, x)) {
					Grid[door.first][door.second] = '.';
					TRACE << "removed " << Grid[y][x] << std::endl;
				}
			}
		}
	}
#endif
}

static inline void find_targets(Coord source, std::map<Coord, int> &costs)
{
	std::queue<std::pair<Coord, int>> q;
	static bool visited[128][128];
	FOR(y, H) {
		FOR(x, W) {
			visited[y][x] = false;
		}
	}
	visited[source.first][source.second] = true;
	q.push(std::make_pair(source, 0));
	const int dy[4] = { 0, 0, -1, 1 };
	const int dx[4] = { 1, -1, 0, 0 };
	while (! q.empty()) {
		const int y = q.front().first.first;
		const int x = q.front().first.second;
		const int pathw = q.front().second;
		q.pop();
		FOR(i, 4) {
			const int ty = y + dy[i];
			const int tx = x + dx[i];
			const int tw = pathw + 1;
			if (ty < 0 || ty >= H || tx < 0 || tx >= W)
				continue;
			if (visited[ty][tx])
				continue;
			visited[ty][tx] = true;
			if (Grid[ty][tx] >= 'a' && Grid[ty][tx] <= 'z') {
				costs.emplace(std::make_pair(ty, tx), tw);
			} else if (Grid[ty][tx] != '.') {
				continue;
			}
			q.push(std::make_pair(std::make_pair(ty, tx), tw));
		}
	}
}

constexpr int inf = 0x3f3f3f3f;
using dp_t = std::map<std::pair<Coord, int>, int>;

static int part1(const Coord pos, const int num_collected, const int goal, const int bitmask, dp_t &dp)
{
	if (num_collected == goal)
		return 0;
	auto iter = dp.find(std::make_pair(pos, bitmask));
	if (iter != dp.end())
		return iter->second;
	std::map<Coord, int> targets;
	find_targets(pos, targets);
	if (targets.empty()) {
		TRACE << "no targets" << std::endl;
		abort();
	}
	int ret = inf;
	for (auto item : targets) {
		char &key_loc = Grid[item.first.first][item.first.second];
		char &door_loc = Grid[Doors[key_loc - 'a'].first][Doors[key_loc - 'a'].second];
		char orig_key = key_loc;
		char orig_door = door_loc;
		key_loc = '.';
		door_loc = '.';
		const int res = item.second + part1(item.first, num_collected + 1, goal, setbit(bitmask, (orig_key - 'a')), dp);
		key_loc = orig_key;
		door_loc = orig_door;
		ret = std::min(ret, res);
	}
	dp[std::make_pair(pos, bitmask)] = ret;
	return ret;
}

int main(int argc, char **argv)
{
	const char *fname = "in.txt";
	if (argc >= 2)
		fname = argv[1];
	FILE *f = fopen(fname, "r");
	if (f == nullptr) {
		TRACE << "file cannot be opened" << std::endl;
		abort();
	}
	go(f);
	fclose(f);
	print_grid();
#ifdef PARTONE
	trace(TotalKeys);
	dp_t dp;
	const int ans = part1(START, 0, TotalKeys, 0, dp);
	trace(ans);
#else
	int total = 0;
	// the order of these are important
	const int dy[4] = { -1, -1, 1, 1 };
	const int dx[4] = { -1, 1, -1, 1 };
	FOR(i, 4) {
		trace(TotalKeys[i]);
		dp_t dp;
		const Coord start { START.first + dy[i], START.second + dx[i] };
		Grid[start.first][start.second] = '@';
		const int ans = part1(start, 0, TotalKeys[i], 0, dp);
		trace(ans);
		total += ans;
	}
	trace(total);
#endif
}
