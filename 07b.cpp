#include "template.hpp"

using Vec = std::vector<int>;

struct ProgramState {
	Vec ops;
	unsigned PC;
	int which;
};

static inline std::pair<bool, int> run_program(ProgramState *ps, int setting, int old_value)
{
	Vec &ops = ps->ops;
	unsigned &PC = ps->PC;
	int &which = ps->which;
	for ( ; ; ) {
		const int opcode = ops[PC] % 100;
		const int first_mode = ops[PC] / 100 % 10;
		const int second_mode = ops[PC] / 1000 % 10;
		//const int third_mode = ops[PC] / 10000 % 10;
		//const int C = (PC + 3 >= ops.size()) ? 0 : (third_mode == 0) ? ops[ops[PC + 3]] : ops[PC + 3];
		switch (opcode) {
		case 99:
			return { false, -1 };
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
			PC += 2;
			return { true, A };
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
			return { false, -222 };
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
	int s[5] = { 5, 6, 7, 8, 9 };
	do {
		ProgramState ps[5];
		FOR(i, 5) {
			ps[i].ops = orig;
			ps[i].PC = 0;
			ps[i].which = 0;
		}
		int q = 0;
		for (int i = 0; ; i = (i + 1) % 5) {
			std::pair<bool, int> res = run_program(ps + i, s[i], q);
			if (! res.first) {
				break;
			}
			q = res.second;
		}
		ans = std::max(ans, q);
	} while (std::next_permutation(s, s + 5));
	trace(ans);
}
