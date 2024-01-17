#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <ranges>
#include <cstdint>

using namespace std;

struct Hailstone
{
	using coord_type = int64_t;
	using vel_type = int16_t;

	coord_type px, py, pz;
	vel_type vx, vy, vz;

	constexpr Hailstone() {}
	constexpr Hailstone(coord_type _px, coord_type _py, coord_type _pz, vel_type _vx, vel_type _vy, vel_type _vz)
	  : px(_px)
	  , py(_py)
	  , pz(_pz)
	  , vx(_vx)
	  , vy(_vy)
	  , vz(_vz)
	{}
	constexpr Hailstone(const array<coord_type, 3>& coord_nums, const array<vel_type, 3>& vel_nums)
	  : Hailstone(coord_nums[0], coord_nums[1], coord_nums[2], vel_nums[0], vel_nums[1], vel_nums[2])
	{}

	constexpr bool in_area_with(const Hailstone& other, const coord_type min_coord, const coord_type max_coord) const noexcept
	{
		const auto det = vx * other.vy - vy * other.vx;
		if(det == 0)
			return false;
		const auto t1_term_numer = static_cast<double>(other.px - px) * other.vy + static_cast<double>(py - other.py) * other.vx;
		if((det > 0) ^ (t1_term_numer > 0))
			return false;
		const auto t2_term_numer = static_cast<double>(other.px - px) * vy + static_cast<double>(py - other.py) * vx;
		if((det > 0) ^ (t2_term_numer > 0))
			return false;
		const auto t2_term = t2_term_numer / det,
		           x = t2_term * other.vx + other.px,
		           y = t2_term * other.vy + other.py;
		return min_coord <= x && x <= max_coord && min_coord <= y && y <= max_coord;
	}
};

int main(void)
{
	constexpr auto min_coord = 200'000'000'000'000,
	               max_coord = 400'000'000'000'000;
	vector<Hailstone> vecs;
	array<Hailstone::coord_type, 3> coord_nums;
	array<Hailstone::vel_type, 3> vel_nums;
	string line;

	while(getline(cin, line)) {
		istringstream iss(line);
		for(auto& coord : coord_nums) {
			iss >> coord;
			iss.get();
		}
		iss.ignore(2, '@');
		for(auto& vel : vel_nums) {
			iss >> vel;
			if(!iss.eof())
				iss.get();
		}
		vecs.emplace_back(coord_nums, vel_nums);
	}

	size_t in_area = 0;
	for(size_t i = 0; const auto& vecA : vecs)
		for(const auto& vecB : vecs | views::drop(++i))
			if(vecA.in_area_with(vecB, min_coord, max_coord))
				++in_area;

	cout << in_area << '\n';
	return 0;
}

