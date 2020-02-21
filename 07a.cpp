#include "template.hpp"

using Vec = std::vector<int>;

static inline int run_program(Vec &ops, int setting, int old_value)
{
	unsigned PC = 0;
	int which = 0;
	for ( ; ; ) {
		const int opcode = ops[PC] % 100;
		const int first_mode = ops[PC] / 100 % 10;
		const int second_mode = ops[PC] / 1000 % 10;
		//const int third_mode = ops[PC] / 10000 % 10;
		//const int C = (PC + 3 >= ops.size()) ? 0 : (third_mode == 0) ? ops[ops[PC + 3]] : ops[PC + 3];
		switch (opcode) {
		case 99:
			return -1;
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
			int &dst = ops[ops[PC + 1]];
			//int input;
			//scanf("%d", &input);
			if (which == 0)
				dst = setting;
			else
				dst = old_value;
			++which;
			PC += 2;
			break;
		}
		case 4: {
			const int A = (first_mode == 0) ? ops[ops[PC + 1]] : ops[PC + 1];
			//printf("%d\n", A);
			return A;
			//PC += 2;
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
			return -222;
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
	Vec orig;
	go(f, orig);
	fclose(f);
	trace(orig.size());
	int ans = -1;
	int s[5] = { 0, 1, 2, 3, 4 };
	do {
		int q = 0;
		FOR(i, 5) {
			Vec ops(orig);
			q = run_program(ops, s[i], q);
		}
		ans = std::max(ans, q);
	} while (std::next_permutation(s, s + 5));
	trace(ans);
}
