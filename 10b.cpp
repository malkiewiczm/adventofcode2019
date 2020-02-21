#include "template.hpp"

//#define SAMPLE
#ifdef SAMPLE
constexpr int LOC_Y = 13;
constexpr int LOC_X = 11;
#else
constexpr int LOC_Y = 18;
constexpr int LOC_X = 20;
#endif

template <typename T0, typename T1>
static inline std::ostream &operator<< (std::ostream &o, const std::pair<T0, T1> rhs)
{
	o << '{' << rhs.first << ", " << rhs.second << '}';
	return o;
}

using Coord = std::pair<int, int>;
using CoordList = std::vector<Coord>;
static std::vector<std::string> grid;

static inline void go(FILE *f, CoordList &list)
{
	static char buf[512];
	int y = 0;
	while (fgets(buf, sizeof(buf), f)) {
		if (buf[0] > ' ') {
			grid.push_back(buf);
			for (int x = 0; buf[x] > ' '; ++x) {
				if (buf[x] == '#')
					list.push_back({ y, x });
			}
		}
		++y;
	}
}

#define sign(what) ((what) >= 0)

static int gcd(int a, int b)
{
	return b == 0 ? a : gcd(b, a % b);
}

static inline Coord rot_right(const Coord &v)
{
	return { -v.second, v.first };
}

static inline Coord get_d(const Coord &v)
{
	// flipped to make this easier to think about
	return { LOC_Y - v.first, v.second - LOC_X };
}

// line of sight
static bool los(const int y, const int x)
{
	if (y == LOC_Y && x == LOC_X)
		return false;
	int dy = LOC_Y - y;
	int dx = LOC_X - x;
	const int m = std::abs(gcd(dy, dx));
	dy /= m;
	dx /= m;
	int sy = y + dy;
	int sx = x + dx;
	while (sy != LOC_Y || sx != LOC_X) {
		if (grid[sy][sx] == '#') {
			return false;
		}
		sy += dy;
		sx += dx;
	}
	return true;
}

static bool my_sort(const Coord &a, const Coord &b)
{
	constexpr double TWO_PI = 6.2831853071796;
	constexpr double eps = 1e-8;
	const Coord da = rot_right(get_d(a));
	const Coord db = rot_right(get_d(b));
	double aa = atan2(da.first, da.second);
	double ab = atan2(db.first, db.second);
	if (aa < eps)
		aa += TWO_PI;
	if (ab < eps)
		ab += TWO_PI;
	return aa > ab;
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
	CoordList list;
	go(f, list);
	fclose(f);
	CoordList visible;
	for (auto item : list) {
		if (los(item.first, item.second))
			visible.push_back(item);
	}
	std::sort(visible.begin(), visible.end(), my_sort);
	trace(visible.size());
	if (visible.size() < 200) {
		TRACE << "NOT ENOUGH VISIBLE ?" << std::endl;
		abort();
	}
	for (size_t i = 0; i < 3; ++i) {
		TRACE << (i+1) << ": " << visible[i].second << ", " << visible[i].first << std::endl;
	}
	const int blend = visible[199].first + visible[199].second*100;
	TRACE << "200th: (x, y) = " << visible[199].second << ", " << visible[199].first << " as " << blend << std::endl;
}
