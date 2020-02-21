#include "template.hpp"

constexpr int W = 25;
constexpr int H = 6;
constexpr int L = 100;

static inline void go(FILE *f, int image[L][H][W])
{
	FOR(i, L) {
		FOR(y, H) {
			FOR(x, W) {
				int c = fgetc(f);
				if (c == EOF) {
					TRACE << "input not big enough (at layer " << L << ')' << std::endl;
					abort();
				}
				if (c == '0' || c == '1' || c == '2') {
					image[i][y][x] = c - '0';
			} else {
					TRACE << "bad character " << static_cast<char>(c) << " = " << c << " (at layer " << L << ')' << std::endl;
					abort();
				}
			}
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
	int image[L][H][W];
	go(f, image);
	fclose(f);
	FOR(y, H) {
		FOR(x, W) {
			for (int i = L - 2; i >= 0; --i) {
				if (image[i][y][x] == 2) {
					image[i][y][x] = image[i + 1][y][x];
				}
			}
		}
	}
	FOR(y, H) {
		FOR(x, W) {
			const char *const fmt = " 8.";
			putchar(fmt[image[0][y][x]]);
		}
		putchar('\n');
	}
}
