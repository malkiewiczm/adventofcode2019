#include "template.hpp"

static int W;
static int H;
static std::vector<std::string> grid;

using IntGrid = std::vector<std::vector<int>>;

static inline void go(FILE *f)
{
	static char buf[512];
	while (fgets(buf, sizeof(buf), f)) {
		if (buf[0] > ' ')
			grid.push_back(buf);
	}
	H = grid.size();
	W = grid[0].length() - 1;
}

static int gcd(int a, int b) {
	return b == 0 ? a : gcd(b, a % b);
}

static inline int cell_count(int my_y, int my_x)
{
	int ret = 0;
	for (int y = my_y + 1; y < H; ++y) {
		if (grid[y][my_x] == '#') {
			++ret;
			break;
		}
	}
	for (int y = my_y - 1; y >= 0; --y) {
		if (grid[y][my_x] == '#') {
			++ret;
			break;
		}
	}
	for (int x = my_x + 1; x < W; ++x) {
		if (grid[my_y][x] == '#') {
			++ret;
			break;
		}
	}
	for (int x = my_x - 1; x >= 0; --x) {
		if (grid[my_y][x] == '#') {
			++ret;
			break;
		}
	}
	FOR(y, H) {
		FOR(x, W) {
			if (grid[y][x] != '#')
				continue;
			if (x == my_x || y == my_y)
				continue;
			int dy = my_y - y;
			int dx = my_x - x;
			const int m = std::abs(gcd(dy, dx));
			dy /= m;
			dx /= m;
			int sy = y + dy;
			int sx = x + dx;
			bool clear = true;
			while (sy != my_y || sx != my_x) {
				if (grid[sy][sx] == '#') {
					clear = false;
					break;
				}
				sy += dy;
				sx += dx;
			}
			if (clear) {
				++ret;
			}
		}
	}
	return ret;
}

static inline void do_count(IntGrid &count)
{
	FOR(y, H) {
		FOR(x, W) {
			if (grid[y][x] != '#')
				continue;
			count[y][x] = cell_count(y, x);
		}
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
	IntGrid count(H, std::vector<int>(W));
	do_count(count);
	int big = -1;
	int big_y = 0;
	int big_x = 0;
	FOR(y, H) {
		FOR(x, W) {
			if (count[y][x] > big) {
				big = count[y][x];
				big_y = y;
				big_x = x;
			}
		}
	}
	trace(big_y);
	trace(big_x);
	trace(big);
}
