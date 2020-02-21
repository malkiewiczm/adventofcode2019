
#include "template.hpp"

//#define SAMPLE

static inline void do_velocity(int64_t &a, int64_t &b, int64_t &av, int64_t &bv)
{
	if (a > b) {
		++bv;
		--av;
	} else if (a < b) {
		--bv;
		++av;
	}
}

using Vec4 = std::tuple<int64_t, int64_t, int64_t, int64_t>;

static inline int64_t gcd(int64_t a, int64_t b)
{
	while (b) {
		int64_t t = b;
		b = a % b;
		a = t;
	}
	return a;
}

static inline int64_t lcm(int64_t a, int64_t b)
{
	return std::abs(a * b) / gcd(a, b);
}

static int64_t find_loop(int64_t x[4])
{
	std::set<std::pair<Vec4, Vec4>> set;
	int64_t v[4] = { 0 };
	for ( ; ; ) {
		FOR(i, 4) {
			for (int j = i + 1; j < 4; ++j) {
				do_velocity(x[i], x[j], v[i], v[j]);
			}
		}
		FOR(i, 4) {
			x[i] += v[i];
		}
		std::pair<Vec4, Vec4> key = std::make_pair(std::make_tuple(x[0], x[1], x[2], x[3]), std::make_tuple(v[0], v[1], v[2], v[3]));
		auto iter = set.find(key);
		if (iter == set.end())
			set.insert(key);
		else
			break;
	}
	return set.size();
}

int main()
{
#ifdef SAMPLE
	int64_t x[4] = { -8, 5, 2, 9 };
	int64_t y[4] = { -10, 5, -7, -8 };
	int64_t z[4] = { 0, 10, 3, -3 };
#else
	int64_t x[4] = { -4, -13, -17, -16 };
	int64_t y[4] = { -9, -11, -7, 4 };
	int64_t z[4] = { -3, 0, 15, 2 };
#endif
	const int64_t rx = find_loop(x);
	const int64_t ry = find_loop(y);
	const int64_t rz = find_loop(z);
	trace(rx);
	trace(ry);
	trace(rz);
	trace(lcm(lcm(rx, ry), rz));
 }
