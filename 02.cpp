#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>

#define PARTONE

#define TRACE (std::cout << "[line " << __LINE__ << "] ")
#define trace(what)(TRACE << #what " = " << (what) << std::endl)
#define lintf(fmt, ...)(printf("[line %d] " fmt, __LINE__, __VA_ARGS__))

#define FOR(i, to) for (int i = 0; i < to; ++i)

static std::vector<int> ops;

static inline void run_program()
{
	for (unsigned PC = 0; ; PC += 4) {
		const int opcode = ops[PC];
		if (opcode == 99)
			return;
		const int A = ops[ops[PC + 1]];
		const int B = ops[ops[PC + 2]];
		int &dst = ops[ops[PC + 3]];
		switch (opcode) {
		case 1:
			dst = A + B;
			break;
		case 2:
			dst = A * B;
			break;
		default:
			TRACE << "BAD OPCODE " << opcode << std::endl;
			abort();
		}
	}
}

static inline void go(FILE *f)
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
	go(f);
	fclose(f);
#ifndef PARTONE
	std::vector<int> orig(ops);
	for (int noun = 0; noun < 100; ++noun) {
		for (int verb = 0; verb < 100; ++verb) {
			ops = orig;
			ops[1] = noun;
			ops[2] = verb;
			run_program();
			if (ops[0] == 19690720) {
				trace(noun);
				trace(verb);
				trace(100 * noun + verb);
			}
		}
	}
#else
	ops[1] = 12;
	ops[2] = 2;
	run_program();
	trace(ops[0]);
#endif
}
