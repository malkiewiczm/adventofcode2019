#include "template.hpp"

using Vec = std::vector<int64_t>;

#define GET_VAR(which, N) ((which) == 2) ? ops[base + ops[PC + N]] : ((which) == 0) ? ops[ops[PC + N]] : ops[PC + N]

#define GET_DST(which, N) ((which) == 2) ? ops[base + ops[PC + N]] : ops[ops[PC + N]];

static inline bool run_program(Vec &ops, const int64_t y, const int64_t x)
{
	unsigned PC = 0;
	int64_t base = 0;
	bool is_y = false;
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
			if (is_y) {
				input = y;
			} else {
				input = x;
			}
			is_y = ! is_y;
			int64_t &dst = GET_DST(first_mode, 1);;
			dst = input;
			PC += 2;
			break;
		}
		case 4: {
			const int64_t A = GET_VAR(first_mode, 1);
			return A;
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
			return false;
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

static inline bool check(const Vec &ops, const int64_t y, const int64_t x)
{
	Vec nops(ops);
	return run_program(nops, y, x);
}

using Coord = std::pair<int, int>;

static bool is_okay(const Vec &ops, int my_y, Coord &data)
{
	int low_x = 0;
	int high_x = 0;
	for (int x = 0; ; ++x) {
		if (check(ops, my_y, x)) {
			if (! low_x) {
				low_x = x;
			}
		} else if (low_x) {
			high_x = x - 1;
			break;
		}
	}
	if (high_x - low_x + 1 < 100)
		return false;
	const int cand_y = my_y + 99;
	const int cand_x = high_x - 99;
	data.first = my_y;
	data.second = cand_x;
	return check(ops, cand_y, cand_x);
}

static inline int decode(const Coord &v)
{
	return v.second * 10000 + v.first;
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
	Vec ops(4096);
	go(f, ops);
	fclose(f);
	int low = 0;
	int high = 10000;
	Coord best;
	while (low <= high) {
		const int mid = low + (high - low) / 2;
		Coord data;
		if (is_okay(ops, mid, data)) {
			high = mid - 1;
			best = data;
		} else {
			low = mid + 1;
		}
	}
	trace(best);
	trace(decode(best));
	FOR(y, 100) {
		FOR(x, 100) {
			const Coord me { y + best.first, x + best.second };
			if (! check(ops, me.first, me.second)) {
				TRACE << "PROBLEM: " << me << std::endl;
				abort();
			}
		}
	}
}
