#include "template.hpp"

using Vec = std::vector<int64_t>;

#define GET_VAR(which, N) ((which) == 2) ? ops[base + ops[PC + N]] : ((which) == 0) ? ops[ops[PC + N]] : ops[PC + N]

#define GET_DST(which, N) ((which) == 2) ? ops[base + ops[PC + N]] : ops[ops[PC + N]];

static inline void run_program(Vec &ops)
{
	unsigned PC = 0;
	int64_t base = 0;
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
			printf("> ");
			if (scanf("%ld", &input) != 1) {
				TRACE << "failed: no input" << std::endl;
				abort();
			}
			int64_t &dst = GET_DST(first_mode, 1);;
			dst = input;
			PC += 2;
			break;
		}
		case 4: {
			const int64_t A = GET_VAR(first_mode, 1);
			printf("%ld\n", A);
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
	run_program(ops);
}
