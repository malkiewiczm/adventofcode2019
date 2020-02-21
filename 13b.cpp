#include "template.hpp"

using Vec = std::vector<int64_t>;

using Coord = std::pair<int64_t, int64_t>;
using Map = std::map<Coord, int>;

constexpr int H = 24;
constexpr int W = 42;

#define GET_VAR(which, N) ((which) == 2) ? ops[base + ops[PC + N]] : ((which) == 0) ? ops[ops[PC + N]] : ops[PC + N]

#define GET_DST(which, N) ((which) == 2) ? ops[base + ops[PC + N]] : ops[ops[PC + N]];

static inline void get_props(const Map &map, Coord &paddle, Coord &ball)
{
	auto score = map.find({ 0, -1 });
	if (score == map.end())
		TRACE << "no score" << std::endl;
	else
		trace(score->second);
	for (auto item : map) {
		switch (item.second) {
		case 3:
			paddle = item.first;
			break;
		case 4:
			ball = item.first;
			break;
		}
	}
}

static inline void draw_map(const Map &map)
{
	constexpr const char *tiles = " 8*_.";
	FOR(y, H) {
		FOR(x, W) {
			auto iter = map.find({ y, x });
			if (iter == map.end())
				putchar(tiles[0]);
			else
				putchar(tiles[iter->second]);
		}
		putchar('\n');
	}
}

static inline int64_t get_next_dir(const Coord &paddle, const Coord &ball, const Coord &last_ball)
{
	const Coord dir { ball.first - last_ball.first, ball.second - last_ball.second };
	const Coord next { ball.first + dir.first, ball.second + dir.second };
	if (next.second == ball.second)
		return 0;
	if (next.second > paddle.second)
		return 1;
	return -1;
}

static inline void run_program(Vec &ops, Map &map)
{
	unsigned PC = 0;
	int64_t base = 0;
	int which_output = 0;
	Coord last_ball { 19, 19 };
	Coord saved_coord;
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
			Coord paddle, ball;
			get_props(map, paddle, ball);
			trace(paddle);
			trace(ball);
			int64_t &dst = GET_DST(first_mode, 1);;
			dst = get_next_dir(paddle, ball, last_ball);
			last_ball = ball;
			PC += 2;
			break;
		}
		case 4: {
			const int64_t A = GET_VAR(first_mode, 1);
			switch (which_output) {
			case 0:
				saved_coord.second = A;
				break;
			case 1:
				saved_coord.first = A;
				break;
			case 2: {
				auto iter = map.find(saved_coord);
				if (iter == map.end())
					map.emplace(saved_coord, static_cast<int>(A));
				else
					iter->second = static_cast<int>(A);
				break;
			}
			}
			which_output = (which_output + 1) % 3;
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
		trace("file cannot be opened");
		exit(1);
	}
	Vec ops(4096);
	go(f, ops);
	fclose(f);
	Map map;
	//ops[0] = 2;
	run_program(ops, map);
	draw_map(map);
	// not 13942
	Coord dummy[2];
	get_props(map, dummy[0], dummy[1]);
}
