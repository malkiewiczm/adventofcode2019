#include "template.hpp"

using Vec = std::vector<int>;

static inline void go(FILE *f, Vec &v)
{
	char *buf = new char[4096];
	if (fgets(buf, 4096, f)) {
		char *c = buf;
		while (*c > ' ') {
			v.push_back(*c - '0');
			++c;
		}
	}
	delete[] buf;
}

static inline void transform(Vec &v)
{
	const int pat[4] = { 0, 1, 0, -1 };
	FOR(i, 100) {
		Vec u(v.size());
		for (size_t h = 0; h < v.size(); ++h) {
			int power = h;
			int p_index = 0;
			int sum = 0;
			for (size_t k = 0; k < v.size(); ++k) {
				if (power == 0) {
					p_index = (p_index + 1) % 4;
					power = h;
				} else {
					--power;
				}
				sum += v[k] * pat[p_index];
			}
			u[h] = std::abs(sum) % 10;
		}
		std::swap(u, v);
	}
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
	Vec v;
	go(f, v);
	fclose(f);
	transform(v);
	TRACE << "";
	FOR(i, 8) {
		std::cout << v[i];
	}
	std::cout << std::endl;
}
