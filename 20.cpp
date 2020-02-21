#include "template.hpp"

//#define PARTONE

using Coord = std::pair<int, int>;

static int W;
static int H;

struct tile_t {
	bool is_wall;
	bool is_jump;
	Coord jump_to;
	bool jumps_outside() const {
		const Coord a { std::abs(jump_to.first), std::abs(jump_to.second) };
		const Coord b { std::abs(jump_to.first - H), std::abs(jump_to.second - W) };
		return std::min(std::min(a.first, b.first), std::min(a.second, b.second)) < 4;
	}
};

static char Raw[256][256];
static tile_t Grid[256][256];
static Coord Start, End;

#define is_letter(what)((what) >= 'A' && (what) <= 'Z')

static inline void go(FILE *f)
{
	H = 0;
	while (fgets(Raw[H], 256, f)) {
		++H;
	}
	W = strlen(Raw[0]) - 1;
}

static inline int get_clean_tag(const int y, const int x, Coord &other_loc)
{
	int ret = (Raw[y][x] - 'A') * 32;
	Raw[y][x] = '?';
	const Coord right { y, x + 1 };
	const Coord below { y + 1, x };
	char &rright = Raw[right.first][right.second];
	char &rbelow = Raw[below.first][below.second];
	if (is_letter(rright)) {
		other_loc = right;
		ret += rright - 'A';
		rright = '?';
	} else if (is_letter(rbelow)) {
		other_loc = below;
		ret += rbelow - 'A';
		rbelow = '?';
	} else {
		TRACE << "no surronding tag info found?" << std::endl;
		abort();
	}
	return ret;
}

static inline int involve_count(const Coord &v)
{
	const int dy[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };
	const int dx[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };
	int ret = 0;
	FOR(i, 8) {
		const int ty = v.first + dy[i];
		const int tx = v.second + dx[i];
		if (ty < 0 || ty >= H || tx < 0 || tx >= W)
			continue;
		if (Raw[ty][tx] == '#' || Raw[ty][tx] == '.')
			++ret;
	}
	return ret;
}

static inline Coord choose_more_involved(const Coord &a, const Coord &b)
{
	if (involve_count(a) > involve_count(b))
		return a;
	else
		return b;
}

static inline void make_grid()
{
	std::map<int, Coord> portal[2];
	FOR(y, H) {
		FOR(x, W) {
			if (Raw[y][x] == ' ' || Raw[y][x] == '#') {
				Grid[y][x].is_wall = true;
			} else if (Raw[y][x] == '.') {
				Grid[y][x].is_wall = false;
				Grid[y][x].is_jump = false;
			} else if (is_letter(Raw[y][x])) {
				Coord other_loc;
				const int tag = get_clean_tag(y, x, other_loc);
				auto iter = portal[0].find(tag);
				const Coord tag_loc = choose_more_involved(std::make_pair(y, x), other_loc);
				if (iter == portal[0].end()) {
					portal[0].emplace(tag, tag_loc);
				} else {
					portal[1].emplace(tag, tag_loc);
				}
			} else if (Raw[y][x] != '?') {
				TRACE << "bad character: " << Raw[y][x] << std::endl;
				abort();
			}
		}
	}
	constexpr int key_AA = 0;
	constexpr int key_ZZ = 25 * 32 + 25;
	auto iter_AA = portal[0].find(key_AA);
	if (iter_AA == portal[0].end()) {
		TRACE << "portal AA (start) not found" << std::endl;
		abort();
	}
	auto iter_ZZ = portal[0].find(key_ZZ);
	if (iter_ZZ == portal[0].end()) {
		TRACE << "portal ZZ (end) not found" << std::endl;
		abort();
	}
	Start = iter_AA->second;
	End = iter_ZZ->second;
	portal[0].erase(iter_AA);
	portal[0].erase(iter_ZZ);
	if (portal[0].size() != portal[1].size()) {
		trace(portal[0].size());
		trace(portal[1].size());
		TRACE << "mismatched !" << std::endl;
		abort();
	}
	for (auto item0 : portal[0]) {
		auto iter1 = portal[1].find(item0.first);
		if (iter1 == portal[1].end()) {
			TRACE << "key found in 0 but not in 1" << std::endl;
			abort();
		}
		tile_t &tile0 = Grid[item0.second.first][item0.second.second];
		tile_t &tile1 = Grid[iter1->second.first][iter1->second.second];
		tile0.is_wall = false;
		tile0.is_jump = true;
		tile0.jump_to = iter1->second;
		tile1.is_wall = false;
		tile1.is_jump = true;
		tile1.jump_to = item0.second;
	}
}

#ifdef PARTONE

static inline int shortest_path()
{
	const int dy[4] = { 0, 0, 1, -1 };
	const int dx[4] = { 1, -1, 0, 0 };
	constexpr int inf = 0x3f3f3f3f;
	PriorityQueue<std::pair<int, Coord>> pq;
	std::map<Coord, int> distance;
	distance.emplace(Start, -1);
	pq.push(std::make_pair(-1, Start));
	while (! pq.empty()) {
		const int pathw = pq.top().first;
		const Coord me = pq.top().second;
		pq.pop();
		FOR(i, 4) {
			const Coord t { me.first + dy[i], me.second + dx[i] };
			if (t.first < 0 || t.first >= H || t.second < 0 || t.second >= W)
				continue;
			const tile_t &tile = Grid[t.first][t.second];
			if (tile.is_wall)
				continue;
			if (distance.find(t) == distance.end())
					distance.emplace(t, inf);
			if (tile.is_jump) {
				if (pathw < distance[t]) {
					distance[t] = pathw;
					pq.push(std::make_pair(pathw, tile.jump_to));
				}
			} else {
				if (pathw + 1 < distance[t]) {
					distance[t] = pathw + 1;
					pq.push(std::make_pair(pathw + 1, t));
				}
			}
		}
	}
	auto iter = distance.find(End);
	if (iter == distance.end()) {
		TRACE << "no shortest path found" << std::endl;
		abort();
	}
	return iter->second - 1;
}

#else

static inline int shortest_path()
{
	const int dy[4] = { 0, 0, 1, -1 };
	const int dx[4] = { 1, -1, 0, 0 };
	constexpr int inf = 0x3f3f3f3f;
	using Coord3 = std::pair<Coord, int>;
	PriorityQueue<std::pair<int, Coord3>> pq;
	std::map<Coord3, int> distance;
	distance.emplace(std::make_pair(Start, 0), -1);
	pq.push(std::make_pair(-1, std::make_pair(Start, 0)));
	while (! pq.empty()) {
		const int pathw = pq.top().first;
		const Coord3 me = pq.top().second;
		pq.pop();
		FOR(i, 4) {
			Coord3 t { { me.first.first + dy[i], me.first.second + dx[i] }, me.second };
			if (t.first.first < 0 || t.first.first >= H || t.first.second < 0 || t.first.second >= W)
				continue;
			const tile_t &tile = Grid[t.first.first][t.first.second];
			if (tile.is_jump) {
				if (tile.jumps_outside())
					++t.second;
				else
					--t.second;
			}
			if (tile.is_wall)
				continue;
			if (distance.find(t) == distance.end())
				distance.emplace(t, inf);
			if (tile.is_jump) {
				if (t.second < 0)
					continue;
				if (t.second > 100)
					continue;
				if (pathw < distance[t]) {
					distance[t] = pathw;
					pq.push(std::make_pair(pathw, std::make_pair(tile.jump_to, t.second)));
				}
			} else {
				if (pathw + 1 < distance[t]) {
					distance[t] = pathw + 1;
					pq.push(std::make_pair(pathw + 1, t));
				}
			}
		}
	}
	auto iter = distance.find(std::make_pair(End, 0));
	if (iter == distance.end()) {
		TRACE << "no shortest path found" << std::endl;
		abort();
	}
	return iter->second - 1;
}

#endif

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
	make_grid();
	trace(shortest_path());
}
