
#include "template.hpp"

#define SAMPLE

struct Vec3 {
	int x, y, z;
};

static inline std::ostream &operator<< (std::ostream &o, const Vec3 &v)
{
	o << '{' << v.x << ", " << v.y << ", " << v.z << '}';
	return o;
}

static inline void do_velocity(int &a, int &b, int &av, int &bv)
{
	if (a > b) {
		++bv;
		--av;
	} else if (a < b) {
		--bv;
		++av;
	}
}

int main()
{
#ifdef SAMPLE
	std::vector<Vec3> coord = {
		{ -1, 0, 2 },
		{ 2, -10, -7 },
		{ 4, -8, 8 },
		{ 3, 5, -1 },
	};
#else
	std::vector<Vec3> coord = {
		{ -4, -9, -3 },
		{ -13, -11, 0 },
		{ -17, -7, 15 },
		{ -16, 4, 2 },
	};
#endif
	const int N = coord.size();
	std::vector<Vec3> vel(N);
	FOR(step, 1000) {
		FOR(i, N) {
			for (int j = i + 1; j < N; ++j) {
				Vec3 &a = coord[i];
				Vec3 &b = coord[j];
				Vec3 &av = vel[i];
				Vec3 &bv = vel[j];
				do_velocity(a.x, b.x, av.x, bv.x);
				do_velocity(a.y, b.y, av.y, bv.y);
				do_velocity(a.z, b.z, av.z, bv.z);
			}
		}
		FOR(i, N) {
			coord[i].x += vel[i].x;
			coord[i].y += vel[i].y;
			coord[i].z += vel[i].z;
		}
		int energy = 0;
		FOR(i, N) {
			const int pot = std::abs(coord[i].x) + std::abs(coord[i].y) + std::abs(coord[i].z);
			const int kin = std::abs(vel[i].x) + std::abs(vel[i].y) + std::abs(vel[i].z);
			energy += pot * kin;
		}

	}
		trace(energy);
 }
