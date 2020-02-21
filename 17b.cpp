#include "template.hpp"

using Vec = std::vector<int64_t>;

#define GET_VAR(which, N) ((which) == 2) ? ops[base + ops[PC + N]] : ((which) == 0) ? ops[ops[PC + N]] : ops[PC + N]

#define GET_DST(which, N) ((which) == 2) ? ops[base + ops[PC + N]] : ops[ops[PC + N]];

using Coord = std::pair<int, int>;
static char Grid[64][64];

struct InputData {
	const char *const main;
	const char *const a;
	const char *const b;
	const char *const c;
	const char *const video;
	const char *at (int index) const {
		switch(index) {
		case 0:
			return main;
		case 1:
			return a;
		case 2:
			return b;
		case 3:
			return c;
		case 4:
			return video;
		default:
			TRACE << "bad index on input data struct : " << index << std::endl;
			abort();
			return nullptr;
		}
	}
};

static inline void run_program(Vec &ops, const InputData *const input_data)
{
	unsigned PC = 0;
	int64_t base = 0;
	int which_input = 0;
	int which_index = 0;
	Coord pos { 0, 0 };
	for ( ; ; ) {
		const int64_t opcode = ops[PC] % 100;
		const int64_t first_mode = ops[PC] / 100 % 10;
		const int64_t second_mode = ops[PC] / 1000 % 10;
		const int64_t third_mode = ops[PC] / 10000 % 10;
		switch (opcode) {
		case 1: {
			const int64_t A = GET_VAR(first_mode, 1);
			const int64_t B = GET_VAR(second_mode, 2);
			int64_t &dst = GET_DST(third_mode, 3);
			dst = A + B;
			PC += 4;
			break;
		}
		case 2: {
			const int64_t A = GET_VAR(first_mode, 1);
			const int64_t B = GET_VAR(second_mode, 2);
			int64_t &dst = GET_DST(third_mode, 3);
			dst = A * B;
			PC += 4;
			break;
		}
		case 3: {
			int64_t input;
			if (input_data) {
				if (input_data->at(which_input)[which_index] == '\0') {
					++which_input;
					which_index = 0;
				}
				input = input_data->at(which_input)[which_index];
				++which_index;
			} else {
				printf("> ");
				if (scanf("%ld", &input) != 1) {
					TRACE << "failed: no input" << std::endl;
					abort();
				}
			}
			int64_t &dst = GET_DST(first_mode, 1);;
			dst = input;
			PC += 2;
			break;
		}
		case 4: {
			const int64_t A = GET_VAR(first_mode, 1);
			if (input_data) {
				if (A <= 127)
					putchar(A);
				else
					printf("output %ld\n", A);
			} else {
				if (A == 10) {
					++pos.first;
					pos.second = 0;
				} else {
					Grid[pos.first][pos.second] = A;
					++pos.second;
				}
			}
			PC += 2;
			break;
		}
		case 5: {
			const int64_t A = GET_VAR(first_mode, 1);
			const int64_t B = GET_VAR(second_mode, 2);
			if (A) {
				PC = B;
			} else {
				PC += 3;
			}
			break;
		}
		case 6: {
			const int64_t A = GET_VAR(first_mode, 1);
			const int64_t B = GET_VAR(second_mode, 2);
			if (!A) {
				PC = B;
			} else {
				PC += 3;
			}
			break;
		}
		case 7: {
			const int64_t A = GET_VAR(first_mode, 1);
			const int64_t B = GET_VAR(second_mode, 2);
			int64_t &dst = GET_DST(third_mode, 3);
			dst = A < B;
			PC += 4;
			break;
		}
		case 8: {
			const int64_t A = GET_VAR(first_mode, 1);
			const int64_t B = GET_VAR(second_mode, 2);
			int64_t &dst = GET_DST(third_mode, 3);
			dst = A == B;
			PC += 4;
			break;
		}
		case 9: {
			const int64_t A = GET_VAR(first_mode, 1);
			base += A;
			PC += 2;
			break;
		}
		case 99:
			return;
		default:
			TRACE << "BAD OPCODE " << opcode << " (PC = " << PC << ")" << std::endl;
			abort();
		}
	}
}

static inline void go(FILE *f, Vec &ops)
{
	int64_t v;
	unsigned i = 0;
	if (fscanf(f, "%ld", &v) != 1)
		return;
	ops[i++] = v;
	while (fscanf(f, ",%ld", &v) == 1) {
		ops[i++] = v;
	}
}

static inline bool out_bound(const int v)
{
	return v < 0 || v >= 60;
}

static inline bool is_corner(const int y, const int x)
{
	if (Grid[y][x] == '^')
		return true;
	if (Grid[y][x] != '#')
		return false;
	const Coord A[4] = {
		{ -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 }
	};
	const Coord B[4] = {
		{ 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 }
	};
	FOR(i, 4) {
		const Coord a { y + A[i].first, x + A[i].second };
		const Coord b { y + B[i].first, x + B[i].second };
		if (out_bound(a.first) || out_bound(a.second) || out_bound(b.first) || out_bound(b.second))
			continue;
		const char ta = Grid[a.first][a.second];
		const char tb = Grid[b.first][b.second];
		if (ta == '#' && tb == '#') {
			return true;
		}
	}
	int ncount = 0;
	FOR(i, 4) {
		const Coord a { y + A[i].first, x + A[i].second };
		if (out_bound(a.first) || out_bound(a.second))
			continue;
		const char ta = Grid[a.first][a.second];
		if (ta == '#') {
			++ncount;
		}
	}
	return ncount == 1;
}

static inline void m_print_grid(const int line)
{
	printf("[line %d]\n", line);
	FOR(y, 60) {
		FOR(x, 60) {
			if (Grid[y][x])
				putchar(Grid[y][x]);
		}
		putchar('\n');
	}
}

#define print_grid() m_print_grid(__LINE__)

static inline char translate_dir(const Coord &delta)
{
	if (delta.first < 0)
		return 'U';
	if (delta.first > 0)
		return 'D';
	if (delta.second < 0)
		return 'L';
	if (delta.second > 0)
		return 'R';
	return 'X';
}

static inline void print_turn(const char dir, const char new_dir)
{
	switch (dir) {
	case 'U':
		switch (new_dir) {
		case 'L':
			printf("L,");
			break;
		case 'R':
			printf("R,");
			break;
		}
		break;
	case 'D':
		switch (new_dir) {;
		case 'L':
			printf("R,");
			break;
		case 'R':
			printf("L,");
			break;
		}
		break;
	case 'L':
		switch (new_dir) {
		case 'U':
			printf("R,");
			break;
		case 'D':
			printf("L,");
			break;
		}
		break;
	case 'R':
		switch (new_dir) {
		case 'U':
			printf("L,");
			break;
		case 'D':
			printf("R,");
			break;
		}
		break;
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
	Vec orig(4*4096);
	go(f, orig);
	fclose(f);
	Vec ops(orig);
	run_program(ops, nullptr);
	// print out your options
	std::set<Coord> corners;
	FOR(y, 60) {
		FOR(x, 60) {
			if (is_corner(y, x)) {
				corners.insert({ y, x });
			}
		}
	}
	int counter = 0;
	const char *const codes = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/\\";
	std::vector<Coord> hotspot(128);
	for (auto item : corners) {
		const char code = codes[counter % 64];
		TRACE << code << " = " << item << std::endl;
		Grid[item.first][item.second] = code;
		hotspot[static_cast<int>(code)] = item;
		++counter;
	}
	print_grid();
	const char *order = "520189df67cahgklzyJIDApmFEvxijruHGq";
	char dir = 'U';
	Coord pos = hotspot[order[0]];
	for (int i = 1; order[i]; ++i) {
		Coord next = hotspot[order[i]];
		Coord diff { next.first - pos.first, next.second - pos.second };
		if ((diff.first && diff.second) || diff.first == diff.second) {
			TRACE << "bad order" << std::endl;
			trace(order[i]);
			trace(diff);
			trace(i);
			abort();
		}
		const int new_dir = translate_dir(diff);
		if (new_dir != dir) {
			print_turn(dir, new_dir);
			dir = new_dir;
		}
		if (diff.first) {
			printf("%d,", std::abs(diff.first));
		} else {
			printf("%d,", std::abs(diff.second));
		}
		pos = next;
	}
	putchar('\n');
	/*
	  Use some online longest repeating substring to calculate
	`a0`,`b0`,`a1`,`b1`,`c0`,`c1`,`b2`,`a2`,`c2`,`a3`,
	 A,B,A,B,C,C,B,A,C,A,
	 L,10,R,8,R,6,R,10,
	 L,12,R,8,L,12
	 L,10,R,8,R,8
	*/
	ops = orig;
	InputData in_data { "A,B,A,B,C,C,B,A,C,A\n", "L,10,R,8,R,6,R,10\n", "L,12,R,8,L,12\n", "L,10,R,8,R,8\n", "n\n" };
	ops[0] = 2;
	run_program(ops, &in_data);
}
