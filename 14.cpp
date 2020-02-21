#include "template.hpp"
#include <sstream>
#include <regex>

//#define PARTONE

using adj_t = std::vector<std::pair<std::string, int64_t>>;

struct Entry {
	int64_t my_amt;
	adj_t adj;
};

using Map = std::map<std::string, Entry>;
using resource_table_t = std::map<std::string, int64_t>;

static inline void go(FILE *f, Map &map)
{
	char *buf = new char[2048];
	while (fgets(buf, 2048, f)) {
		if (buf[0] <= ' ')
			break;
		std::stringstream ss(buf);
		Entry ent;
		for ( ; ; ) {
			int64_t num;
			std::string name;
			ss >> num >> name;
			const bool should_continue = name[name.size() - 1] == ',';
			if (should_continue) {
				name.erase(name.size() - 1);
			}
			ent.adj.push_back({ name, num });
			if (! should_continue)
				break;
		}
		std::string my_name;
		// read in dummy =>
		ss >> my_name;
		ss >> ent.my_amt;
		ss >> my_name;
		if (map.find(my_name) != map.end()) {
			TRACE << "PROBLEM: key already exists: " << my_name << std::endl;
		}
		map.emplace(my_name, ent);
	}
	delete[] buf;
}

static inline int64_t my_div(const int64_t a, const int64_t b)
{
	return (a / b) + (a % b != 0);
}

static int64_t calc(Map &map, resource_table_t &resource_table, const std::string &key, int64_t needed)
{
	if (key == "ORE")
		return needed;
	if (needed <= resource_table[key]) {
		resource_table[key] -= needed;
		return 0;
	} else {
		auto iter = resource_table.find(key);
		needed -= iter->second;
		iter->second = 0;
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		TRACE << "production rule not found : " << key << std::endl;
		abort();
	}
	const int64_t my_amt = my_div(needed, iter->second.my_amt);
	const adj_t &adj = iter->second.adj;
	int64_t ans = 0;
	for (auto item : adj) {
		const std::string &name = item.first;
		const int64_t amt = item.second;
		ans += calc(map, resource_table, name, amt * my_amt);
	}
	resource_table[key] += iter->second.my_amt * my_amt - needed;
	return ans;
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
	Map map;
	go(f, map);
	fclose(f);
	resource_table_t resource_table;
#ifdef PARTONE
	const int64_t ans = calc(map, resource_table, "FUEL", 1);
	trace(ans);
#else
	constexpr int64_t BIG = 1000000000000;
	int64_t low = 0;
	int64_t high = BIG;
	int64_t best = -1;
	while (low <= high) {
		const int64_t mid = low + (high - low) / 2;
		const int64_t ans = calc(map, resource_table, "FUEL", mid);
		if (ans < BIG) {
			low = mid + 1;
			best = mid;
		} else {
			high = mid - 1;
		}
	}
	trace(best);
#endif
}
