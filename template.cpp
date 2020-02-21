#include "template.hpp"

static std::vector<std::string> ins;

static inline void go(FILE *f)
{
	static char buf[512];
	while (fgets(buf, sizeof(buf), f)) {
		if (buf[0] > ' ')
			ins.push_back(buf);
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
	go(f);
	fclose(f);
	trace(ins.size());
}
