#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <limits>
#include <cstdint>

using namespace std;

using point_t = pair<size_t, size_t>;

enum class cell_t : uint8_t
{
	AIR, ROCK, SAND
};
using enum cell_t;

inline bool can_move(const vector<vector<cell_t>>& grid, const size_t sand_x, const size_t sand_y, const size_t cols)
{
	return grid[sand_y][sand_x] == AIR
		|| (sand_x && grid[sand_y][sand_x - 1] == AIR)
		|| (sand_x < cols && grid[sand_y][sand_x + 1] == AIR);
}

int main(void)
{
	constexpr size_t sand_drop_x = 500,
	                 sand_drop_y = 0;
	                  
	vector<vector<point_t>> paths;
	string line;

	auto max_x = numeric_limits<size_t>::min(),
	     min_x = numeric_limits<size_t>::max(),
	     max_y = numeric_limits<size_t>::min();
	while(getline(cin, line)) {
		vector<point_t> path;

		size_t x, y;
		istringstream is(line);
		while(!is.eof()) {
			is >> x;
			max_x = max(max_x, x);
			min_x = min(min_x, x);
			is.get();

			is >> y;
			max_y = max(max_y, y);
			for(uint8_t i = 0; i != 4; ++i) {
				if(is.eof())
					break;
				is.get();
			}

			path.emplace_back(x, y);
		}
		paths.push_back(path);
	}

	assert(sand_drop_x >= min_x && sand_drop_x <= max_x);

	const auto sand_enter_x = sand_drop_x - min_x,
	           cols = max_x - min_x + 1;
	vector<vector<cell_t>> grid(max_y + 1);
	for(auto& row : grid)
		row.resize(cols, AIR);

	for(auto& path : paths) { // Draw each path
		auto& prev = path.front();

		for(size_t i = 1; i != path.size(); ++i) {
			auto& cur = path[i];
			if(cur.first == prev.first)
				for(size_t y = prev.second; y != cur.second; cur.second > prev.second ? ++y : --y)
					grid[y][cur.first - min_x] = ROCK;
			else
				for(size_t x = prev.first; x != cur.first; cur.first > prev.first ? ++x : --x)
					grid[cur.second][x - min_x] = ROCK;
			prev = cur;
		}
		grid[path.back().second][path.back().first - min_x] = ROCK; // Last cell won't be done
	}

	auto sand_x = sand_enter_x,
	     sand_y = sand_drop_y;
	uint32_t sand_count = 0;
	do {
		do {
			while(sand_y != max_y && grid[sand_y + 1][sand_x] == AIR)
				++sand_y;
			if(sand_y == max_y)
				goto done;

			if(!sand_x) // Can move to left off screen
				goto done;
			else if(grid[sand_y + 1][sand_x - 1] == AIR) {
				++sand_y;
				--sand_x;
			} else if(sand_x == cols - 1) // Can move to right off screen
				goto done;
			else if(grid[sand_y + 1][sand_x + 1] == AIR) {
				++sand_y;
				++sand_x;
			} else
				break;
		} while(1);
		grid[sand_y][sand_x] = SAND;
		++sand_count;

		sand_x = sand_enter_x;
		sand_y = sand_drop_y;
	} while(can_move(grid, sand_x, sand_y, cols)); // Also possible that the spawn becomes blocked

done:
	cout << sand_count << '\n';
	return 0;
}

