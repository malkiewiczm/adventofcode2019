#include "template.hpp"

using Vec = std::vector<int>;

static inline std::ostream &operator<< (std::ostream &o, const Vec &v)
{
	o << '[';
	for (auto item : v) {
		o << item;
	}
	o << ']';
	return o;
}

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
	const int VN = static_cast<int>(v.size());
	FOR(repeat, 100) {
		Vec u(VN);
		u.back() = v.back();
		for (int i = VN - 2; i >= 0; --i) {
			u[i] = (v[i] + u[i + 1]) % 10;
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
	Vec v0;
	go(f, v0);
	fclose(f);
	const int VN = static_cast<int>(v0.size());
	int offset = 0;
	FOR(i, 7) {
		offset *= 10;
		offset += v0[i];
	}
	trace(offset);
	const int VNN = VN*10000 - offset;
	Vec v(VNN);
	FOR(i, VNN) {
		v[i] = v0[(i + offset) % VN];
	}
	transform(v);
	TRACE << "";
	FOR(i, 8) {
		std::cout << v[i];
	}
	std::cout << std::endl;
}
