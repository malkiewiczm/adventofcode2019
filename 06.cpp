#include "template.hpp"

//#define PARTONE

using Graph = std::map<std::string, std::vector<std::string>>;

static inline void go(FILE *f, Graph &g)
{
	char left[4] = { 0 };
	char right[4] = { 0 };
	while (fscanf(f, "%c%c%c)%c%c%c\n", left, left + 1, left + 2, right, right + 1, right + 2) == 6) {
		g[left].push_back(right);
	}
}

#ifdef PARTONE

static int dfs(const Graph &g, const std::string &node, int depth)
{
	auto iter = g.find(node);
	if (iter == g.end())
		return depth;
	int sum = depth;
	for (auto item : iter->second) {
		sum += dfs(g, item, depth + 1);
	}
	return sum;
}

#else

static int dfs_find(const Graph &g, const std::string &start, const std::string &target)
{
	if (start == target)
		return 1;
	auto iter = g.find(start);
	if (iter == g.end())
		return 0;
	for (auto item : iter->second) {
		const int res = dfs_find(g, item, target);
		if (res)
			return res + 1;
	}
	return 0;
}

static int dfs(const Graph &g, const std::string &start)
{
	const int you = dfs_find(g, start, "YOU");
	const int san = dfs_find(g, start, "SAN");
	int ans;
	if (you == 0 || san == 0) {
		ans = 0x3f3f3f3f;
	} else {
		ans = you + san;
	}
	auto iter = g.find(start);
	if (iter == g.end())
		return ans;
	for (auto item : iter->second) {
		const int res = dfs(g, item);
		ans = std::min(ans, res);
	}
	return ans;
}

#endif

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
	Graph g;
	go(f, g);
	fclose(f);
#ifdef PARTONE
	trace(dfs(g, "COM", 0));
#else
	trace(dfs(g, "COM") - 4);
#endif
}
