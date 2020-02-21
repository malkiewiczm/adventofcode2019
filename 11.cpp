#include "template.hpp"

using Vec = std::vector<int64_t>;

#define GET_VAR(which, N) ((which) == 2) ? ops[base + ops[PC + N]] : ((which) == 0) ? ops[ops[PC + N]] : ops[PC + N]

#define GET_DST(which, N) ((which) == 2) ? ops[base + ops[PC + N]] : ops[ops[PC + N]];

using Map = std::map<std::pair<int, int>, bool>;

static inline void run_program(Vec &ops, Map &tiles)
{
	unsigned PC = 0;
	int64_t base = 0;
	int x = 0;
	int y = 0;
	int64_t dir = 0;
	bool is_color = true;
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
			auto iter = tiles.find({ y, x });
			if (iter == tiles.end())
				input = 0;
			else
				input = iter->second;
			int64_t &dst = GET_DST(first_mode, 1);
			dst = input;
			PC += 2;
			break;
		}
		case 4: {
			const int64_t A = GET_VAR(first_mode, 1);
			if (is_color) {
				auto iter = tiles.find({ y, x });
				if (iter == tiles.end())
					tiles.emplace(std::make_pair(y, x), A);
				else
					iter->second = A;
			} else {
				if (A) {
					dir = (dir + 1) % 4;
				} else {
					--dir;
					if (dir < 0)
						dir = 3;
				}
				switch (dir) {
				case 0:
					y += 1;
					break;
				case 1:
					x += 1;
					break;
				case 2:
					y -= 1;
					break;
				case 3:
					x -= 1;
					break;
				}
			}
			is_color = ! is_color;
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
	if (fscanf(f, "%I64d", &v) != 1)
		return;
	ops[i++] = v;
	while (fscanf(f, ",%I64d", &v) == 1) {
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
		trace("file cannot be opened");
		exit(1);
	}
	Vec ops(4096);
	go(f, ops);
	fclose(f);
	Map map;
#ifndef PARTONE
	map.emplace(std::make_pair(0, 0), 1);
#endif
	run_program(ops, map);
	trace(map.size());
#ifndef PARTONE
	constexpr int inf = 0x3f3f3f3f;
	std::pair<int, int> top_left { -inf, inf };
	std::pair<int, int> bottom_right { inf, -inf };
	for (auto item : map) {
		if (item.second == 0)
			continue;
		const int y = item.first.first;
		const int x = item.first.second;
		top_left.first = std::max(y, top_left.first);
		top_left.second = std::min(x, top_left.second);
		bottom_right.first = std::min(y, bottom_right.first);
		bottom_right.second = std::max(x, bottom_right.second);
	}
	trace(top_left.second);
	trace(top_left.first);
	trace(bottom_right.second);
	trace(bottom_right.first);
	for (int y = top_left.first; y >= bottom_right.first; --y) {
		for (int x = top_left.second; x <= bottom_right.second; ++x) {
			auto iter = map.find({ y, x});
			if (iter == map.end())
				continue;
			if (iter->second) {
				putchar('8');
			} else {
				putchar(' ');
			}
		}
		putchar('\n');
	}
#endif
}
