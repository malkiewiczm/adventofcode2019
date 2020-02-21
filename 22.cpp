#include "template.hpp"
#include <fstream>

struct Mne {
	const bool is_reverse;
	const bool is_cut;
	const int param;
	Mne()
		: is_reverse(true), is_cut(false), param(0) {}
	Mne(bool m_is_cut, int m_param)
		: is_reverse(false), is_cut(m_is_cut), param(m_param) {}
};

static inline std::ostream &operator<<(std::ostream &o, const Mne &rhs)
{
	if (rhs.is_reverse) {
		o << "reverse";
	} else {
		if (rhs.is_cut) {
			o << "cut ";
		} else {
			o << "deal ";
		}
		o << rhs.param;
	}
	return o;
}

static inline void go(std::ifstream &f, std::vector<Mne> &mnes)
{
	std::string word;
	while (f >> word) {
		if (word.size() <= 1)
			continue;
		if (word[0] == 'c') {
			int cut_amt;
			f >> cut_amt;
			mnes.emplace_back(true, cut_amt);
		} else {
			f >> word;
			if (word[0] == 'w') {
				f >> word;
				int deal_amt;
				f >> deal_amt;
				mnes.emplace_back(false, deal_amt);
			} else {
				f >> word;
				f >> word;
				mnes.emplace_back();
			}
		}
	}
}

static inline void naive(const std::vector<Mne> &mnes)
{
	std::vector<int> res(10007);
	FOR(i, res.size()) {
		res[i] = i;
	}
	for (auto mne : mnes) {
		if (mne.is_reverse) {
			std::reverse(res.begin(), res.end());
		} else {
			if (mne.is_cut) {
				int amt = mne.param;
				if (amt < 0) {
					amt += res.size();
				}
				const std::vector<int> dup(res);
				FOR(i, res.size()) {
					res[i] = dup[(i + amt) % res.size()];
				}
			} else {
				const std::vector<int> dup(res);
				int index = 0;
				FOR(i, res.size()) {
					res[index] = dup[i];
					index = (index + mne.param) % res.size();
				}
			}
		}
	}
	FOR(i, res.size()) {
		if (res[i] == 2019)
			trace(i);
	}
}

int main(int argc, char **argv)
{
	const char *fname = "in.txt";
	if (argc >= 2)
		fname = argv[1];
	std::ifstream f(fname);
	if (! f) {
		TRACE << "file cannot be opened" << std::endl;
		abort();
	}
	std::vector<Mne> mnes;
	go(f, mnes);
	naive(mnes);
}
