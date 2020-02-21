#include "template.hpp"


static inline void go(FILE *f, int image[10][6][26])
{
	FOR(i, 10) {
		FOR(y, 6) {
			FOR(x, 25) {
				image[i][y][x] = fgetc(f) - '0';
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
	int image[10][6][26];
	go(f, image);
	fclose(f);
	int count[10][10] = { 0 };
	FOR(i, 10) {
		FOR(y, 6) {
			FOR(x, 25) {
				++count[i][image[i][y][x]];
			}
		}
	}
	int which_layer = 0;
	FOR(i, 10) {
		if (count[i][0] < count[which_layer][0]) {
			which_layer = i;
		}
	}
	trace(count[which_layer][1] * count[which_layer][2]);
}
