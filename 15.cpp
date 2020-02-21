#include "template.hpp"
#include <stack>

using Vec = std::vector<int64_t>;

#define GET_VAR(which, N) ((which) == 2) ? ops[base + ops[PC + N]] : ((which) == 0) ? ops[ops[PC + N]] : ops[PC + N]

#define GET_DST(which, N) ((which) == 2) ? ops[base + ops[PC + N]] : ops[ops[PC + N]];

using Coord = std::pair<int, int>;
using Map = std::map<Coord, int>;

static inline bool is_valid_move(int move, const Coord &pos, const Map &map)
{
	const int dy[4] = { -1, 1, 0, 0 };
	const int dx[4] = { 0, 0, -1, 1 };
	const Coord t { pos.first + dy[move - 1], pos.second + dx[move - 1] };
	return map.find(t) == map.end();
}

static inline int get_valid_move(const Coord &pos, const Map &map)
{
	FOR(i, 4) {
		if (is_valid_move(i + 1, pos, map))
			return i + 1;
	}
	return -1;
}

static inline void perform_move(Coord &pos, int move)
{
	switch (move) {
	case 1:
		pos.first -= 1;
		break;
	case 2:
		pos.first += 1;
		break;
	case 3:
		pos.second -= 1;
		break;
	case 4:
		pos.second += 1;
		break;
	}
}

static inline void run_program(Vec &ops, Map &map)
{
	unsigned PC = 0;
	int64_t base = 0;
	Coord pos { 0, 0 };
	std::stack<int> stack;
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
			int64_t &dst = GET_DST(first_mode, 1);
			dst = get_valid_move(pos, map);
			if (dst == -1) {
				if (stack.empty())
					return;
				dst = stack.top();
				stack.pop();
			} else {
				switch (dst) {
				case 1:
					stack.push(2);
					break;
				case 2:
					stack.push(1);
					break;
				case 3:
					stack.push(4);
					break;
				case 4:
					stack.push(3);
					break;
				}
			}
			perform_move(pos, dst);
			PC += 2;
			break;
		}
		case 4: {
			const int64_t A = GET_VAR(first_mode, 1);
			map[pos] = A;
			if (A == 0) {
				if (stack.empty())
					return;
				perform_move(pos, stack.top());
				stack.pop();
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
			TRACE << "op 99" << std::endl;
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

static inline int bfs(const Map &map, Coord &final_pos)
{
	constexpr int inf = 0x3f3f3f3f;
	std::queue<std::pair<Coord, int>> q;
	q.push(std::make_pair(std::make_pair(0, 0), 0));
	Map distance;
	for (auto item : map) {
		distance.emplace(item.first, inf);
	}
	distance[{ 0, 0 }] = 0;
	while (! q.empty()) {
		const int y = q.front().first.first;
		const int x = q.front().first.second;
		const int pathw = q.front().second;
		q.pop();
		const int dy[4] = { -1, 1, 0, 0 };
		const int dx[4] = { 0, 0, 1, -1 };
		FOR(i, 4) {
			const int ty = y + dy[i];
			const int tx = x + dx[i];
			const int new_pathw = pathw + 1;
			auto iter = map.find({ ty, tx });
			if (iter == map.end())
				continue;
			if (iter->second == 0)
				continue;
			if (iter->second == 2) {
				final_pos.first = ty;
				final_pos.second = tx;
				return new_pathw;
			}
			auto diter = distance.find({ ty, tx});
			if (diter == distance.end()) {
				TRACE << "problem" << std::endl;
				abort();
			}
			if (new_pathw < diter->second) {
				diter->second = new_pathw;
				q.push(std::make_pair(std::make_pair(ty, tx), new_pathw));
			}
		}
	}
	return -1;
}

static inline int bfs2(const Map &map, const Coord &oxy_loc)
{
	constexpr int inf = 0x3f3f3f3f;
	std::queue<std::pair<Coord, int>> q;
	q.push(std::make_pair(oxy_loc, 0));
	Map distance;
	for (auto item : map) {
		distance.emplace(item.first, inf);
	}
	distance[oxy_loc] = 0;
	while (! q.empty()) {
		const int y = q.front().first.first;
		const int x = q.front().first.second;
		const int pathw = q.front().second;
		q.pop();
		const int dy[4] = { -1, 1, 0, 0 };
		const int dx[4] = { 0, 0, 1, -1 };
		FOR(i, 4) {
			const int ty = y + dy[i];
			const int tx = x + dx[i];
			const int new_pathw = pathw + 1;
			auto iter = map.find({ ty, tx });
			if (iter == map.end())
				continue;
			if (iter->second == 0)
				continue;
			auto diter = distance.find({ ty, tx});
			if (diter == distance.end()) {
				TRACE << "problem" << std::endl;
				abort();
			}
			if (new_pathw < diter->second) {
				diter->second = new_pathw;
				q.push(std::make_pair(std::make_pair(ty, tx), new_pathw));
			}
		}
	}
	int ret = 0;
	for (auto item : distance) {
		if (item.second == inf)
			continue;
		ret = std::max(ret, item.second);
	}
	return ret;
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
	Map map;
	map.emplace(std::make_pair(0, 0), 1);
	run_program(ops, map);
	trace(map.size());
	Coord oxy_loc;
	trace(bfs(map, oxy_loc));
	trace(oxy_loc);
	trace(bfs2(map, oxy_loc));
}
