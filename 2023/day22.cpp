#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <span>
#include <array>
#include <utility>
#include <tuple>
#include <algorithm>
#include <functional>
#include <compare>
#include <cstdint>

using namespace std;

struct Brick
{
	using coord_type = size_t;
	using coords_type = tuple<coord_type, coord_type, coord_type>;

	coords_type start, end;
	vector<Brick*> above, below;

	constexpr Brick() {}
	constexpr Brick(coords_type&& _start, coords_type&& _end)
	  : start(move(_start))
	  , end(move(_end))
	{}
	constexpr Brick(coord_type x1, coord_type y1, coord_type z1, coord_type x2, coord_type y2, coord_type z2)
	  : Brick({ x1, y1, z1 }, { x2, y2, z2 })
	{}
	constexpr Brick(const array<coord_type, 6>& coord_nums)
	  : Brick(coord_nums[0], coord_nums[1], coord_nums[2], coord_nums[3], coord_nums[4], coord_nums[5])
	{}

	constexpr strong_ordering operator<=>(const Brick& other) const noexcept
	{ // Sort by min z-coord, for std::less
		return min_z() <=> other.min_z();
	}

	constexpr coord_type min_z(void) const noexcept
	{
		return min(get<2>(start), get<2>(end));
	}
	constexpr coord_type max_z(void) const noexcept
	{
		return max(get<2>(start), get<2>(end));
	}
	constexpr bool on(const Brick& other) const noexcept
	{
		return other.max_z() == min_z() - 1
			&& get<0>(start) <= get<0>(other.end) && get<0>(end) >= get<0>(other.start)
			&& get<1>(start) <= get<1>(other.end) && get<1>(end) >= get<1>(other.start);
	}
	constexpr bool landed(span<Brick>& bricks) noexcept
	{
		if(min_z() == 1)
			return true;
		bool ret = false;
		for(auto& brick : bricks) {
			if(!on(brick))
				continue;
			ret = true;
			brick.above.push_back(this);
			below.push_back(&brick);
		}
		return ret;
	}
};

int main(void)
{
	vector<Brick> bricks;
	array<Brick::coord_type, 6> coord_nums;
	string line;

	while(getline(cin, line)) {
		istringstream iss(line);
		for(auto& coord : coord_nums) {
			iss >> coord;
			if(!iss.eof())
				iss.get();
		}
		bricks.emplace_back(coord_nums);
	}
	ranges::sort(bricks, less<Brick>());

	Brick::coord_type top_z = 0;
	for(size_t i = 0; auto& brick : bricks) {
		auto& start_z = get<2>(brick.start);
		auto& end_z = get<2>(brick.end);

		const auto brick_min_z = brick.min_z();
		if(brick_min_z > top_z + 1) {
			start_z -= brick_min_z - top_z - 1;
			end_z -= brick_min_z - top_z - 1;
		}

		span bricks_span(bricks.begin(), i);
		while(!brick.landed(bricks_span)) {
			--start_z;
			--end_z;
		}
		top_z = max(top_z, brick.max_z());
		++i;
	}

	const auto removable = ranges::count_if(bricks, [](const auto& brick) {
		return ranges::all_of(brick.above, [](const auto* const brick_ptr) {
			return brick_ptr->below.size() > 1;
		});
	});

	cout << removable << '\n';
	return 0;
}

