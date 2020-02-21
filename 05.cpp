#include "template.hpp"

using Vec = std::vector<int>;

static inline void run_program(Vec &ops)
{
	unsigned PC = 0;
	for ( ; ; ) {
		const int opcode = ops[PC] % 100;
		const int first_mode = ops[PC] / 100 % 10;
		const int second_mode = ops[PC] / 1000 % 10;
		if (opcode == 99)
			return;
		switch (opcode) {
		case 1: {
			const int A = (first_mode == 0) ? ops[ops[PC + 1]] : ops[PC + 1];
			const int B = (second_mode == 0) ? ops[ops[PC + 2]] : ops[PC + 2];
			int &dst = ops[ops[PC + 3]];
			dst = A + B;
			PC += 4;
			break;
		}
		case 2: {
			const int A = (first_mode == 0) ? ops[ops[PC + 1]] : ops[PC + 1];
			const int B = (second_mode == 0) ? ops[ops[PC + 2]] : ops[PC + 2];
			int &dst = ops[ops[PC + 3]];
			dst = A * B;
			PC += 4;
			break;
		}
		case 3: {
			int input;
			scanf("%d", &input);
			ops[ops[PC + 1]] = input;
			PC += 2;
			break;
		}
		case 4: {
			const int A = (first_mode == 0) ? ops[ops[PC + 1]] : ops[PC + 1];
			printf("%d\n", A);
			PC += 2;
			break;
		}
		case 5: {
			const int A = (first_mode == 0) ? ops[ops[PC + 1]] : ops[PC + 1];
			const int B = (second_mode == 0) ? ops[ops[PC + 2]] : ops[PC + 2];
			if (A) {
				PC = B;
			} else {
				PC += 3;
			}
			break;
		}
		case 6: {
			const int A = (first_mode == 0) ? ops[ops[PC + 1]] : ops[PC + 1];
			const int B = (second_mode == 0) ? ops[ops[PC + 2]] : ops[PC + 2];
			if (!A) {
				PC = B;
			} else {
				PC += 3;
			}
			break;
		}
		case 7: {
			const int A = (first_mode == 0) ? ops[ops[PC + 1]] : ops[PC + 1];
			const int B = (second_mode == 0) ? ops[ops[PC + 2]] : ops[PC + 2];
			int &dst = ops[ops[PC + 3]];
			dst = A < B;
			PC += 4;
			break;
		}
		case 8: {
			const int A = (first_mode == 0) ? ops[ops[PC + 1]] : ops[PC + 1];
			const int B = (second_mode == 0) ? ops[ops[PC + 2]] : ops[PC + 2];
			int &dst = ops[ops[PC + 3]];
			dst = A == B;
			PC += 4;
			break;
		}
		default:
			TRACE << "BAD OPCODE " << opcode << std::endl;
			abort();
		}
	}
}

static inline void go(FILE *f, Vec &ops)
{
	int v;
	if (fscanf(f, "%d", &v) != 1)
		return;
	ops.push_back(v);
	while (fscanf(f, ",%d", &v) == 1) {
		ops.push_back(v);
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
	Vec ops;
	go(f, ops);
	fclose(f);
	trace(ops.size());
	run_program(ops);
}
