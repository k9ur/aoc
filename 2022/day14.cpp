#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <cstdint>

using namespace std;

struct Point {
	size_t x;
	size_t y;

	Point() {}
	Point(size_t _x, size_t _y)
	  : x(_x)
	  , y(_y)
	{}
};

enum cell_t : uint8_t {
	AIR, ROCK, SAND
};
static_assert(AIR == 0);

inline bool can_move(const vector<vector<cell_t>>& grid, const size_t sand_x, const size_t sand_y, const size_t cols) {
	return grid[sand_y][sand_x] == AIR
		|| (sand_x && grid[sand_y][sand_x - 1] == AIR)
		|| (sand_x < cols && grid[sand_y][sand_x + 1] == AIR);
}

int main(void) {
	constexpr size_t sand_drop_x = 500,
	                 sand_drop_y = 0;
	                  
	vector<vector<Point>> paths;
	string line;

	size_t max_x = numeric_limits<size_t>::min(),
	       min_x = numeric_limits<size_t>::max(),
	       max_y = numeric_limits<size_t>::min();
	while(getline(cin, line)) {
		vector<Point> path;

		size_t x, y;
		istringstream is(line);
		while(!is.eof()) {
			is >> x;
			if(x > max_x)
				max_x = x;
			if(x < min_x)
				min_x = x;
			is.get();

			is >> y;
			if(y > max_y)
				max_y = y;
			for(uint8_t i = 0; i != 4; ++i) {
				if(is.eof())
					break;
				is.get();
			}

			path.push_back(Point(x, y));
		}
		paths.push_back(path);
	}

	assert(sand_drop_x >= min_x && sand_drop_x <= max_x);

	const size_t sand_enter_x = sand_drop_x - min_x,
	             cols = max_x - min_x + 1;
	vector<vector<cell_t>> grid(max_y + 1);
	for(vector<cell_t>& row : grid)
		row.resize(cols);

	for(vector<Point>& path : paths) { // Draw each path
		Point& previous = path.front();

		for(size_t i = 1; i != path.size(); ++i) {
			Point& current = path[i];
			if(current.x == previous.x)
				for(size_t y = previous.y; y != current.y; current.y > previous.y ? ++y : --y)
					grid[y][current.x - min_x] = ROCK;
			else
				for(size_t x = previous.x; x != current.x; current.x > previous.x ? ++x : --x)
					grid[current.y][x - min_x] = ROCK;
			previous = current;
		}
		grid[path.back().y][path.back().x - min_x] = ROCK; // Last cell won't be done
	}

	size_t sand_x = sand_enter_x,
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
