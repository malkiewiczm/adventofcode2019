#include "template.hpp"

static inline int go(FILE *f)
{
	int n = 0;
	int i = 0;
	while (i < 25) {
		int c = fgetc(f);
		if (c == EOF) {
			TRACE << "eof detected" << std::endl;
			abort();
			return -1;
		}
		if (c == '#') {
			n |= (1 << i);
			++i;
		} else if (c == '.') {
			++i;
		}
	}
	return n;
}

static inline int get_bit(const int v, const int i)
{
	return (v >> i) & 1;
}

static inline void set_bit(int &v, const int i)
{
	v = v | (1 << i);
}

static inline int get_index(const int y, const int x)
{
	return y*5 + x;
}

static inline void m_print_board(const int line, const int board)
{
	printf("[line %d] printed board =\n", line);
	FOR(y, 5) {
		FOR(x, 5) {
			if (get_bit(board, get_index(y, x)))
				putchar('#');
			else
				putchar('.');
		}
		putchar('\n');
	}
}

#define print_board(board) (m_print_board(__LINE__, (board)))

static inline int step(const int board)
{
	int res = 0;
	const int dy[4] = { 1, -1, 0, 0 };
	const int dx[4] = { 0, 0, 1, -1 };
	FOR(y, 5) {
		FOR(x, 5) {
			const int me = get_index(y, x);
			int nc = 0;
			FOR(i, 4) {
				const int ty = y + dy[i];
				const int tx = x + dx[i];
				if (ty < 0 || ty >= 5 || tx < 0 || tx >= 5)
					continue;
				nc += get_bit(board, get_index(ty, tx));
			}
			const int am_bug = get_bit(board, me);
			if (am_bug) {
				if (nc == 1) {
					set_bit(res, me);
				}
			} else {
				if (nc == 1 || nc == 2) {
					set_bit(res, me);
				}
			}
		}
	}
	return res;
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
	int board = go(f);
	fclose(f);
	std::set<int> seen;
	seen.insert(board);
	for ( ; ; ) {
		board = step(board);
		if (seen.find(board) == seen.end()) {
			seen.insert(board);
		} else {
			trace(board);
			break;
		}
	}
}
