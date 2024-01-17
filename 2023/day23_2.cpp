#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <cassert>
#include <cstdint>

using namespace std;

using point_t = pair<size_t, size_t>; // r, c

struct Path
{
	enum class dir_t : uint8_t
	{
		UP, LEFT, DOWN, RIGHT
	};
	using enum dir_t;

	vector<point_t> decisions;
	point_t pos;
	size_t length = 0;
	dir_t dir;

	constexpr Path() {}
	constexpr Path(point_t _pos, dir_t _dir)
	  : pos(_pos)
	  , dir(_dir)
	{}

	constexpr bool can_go(const vector<string>& grid, const dir_t direction) const noexcept
	{
		const auto [ r, c ] = pos;
		return direction == UP ? (dir != DOWN && r && grid[r - 1][c] != '#')
			: direction == DOWN ? (dir != UP && r != grid.size() - 1 && grid[r + 1][c] != '#')
			: direction == LEFT ? (dir != RIGHT && c && grid[r][c - 1] != '#')
			: (dir != LEFT && c != grid.front().size() - 1 && grid[r][c + 1] != '#');
	}
	constexpr bool at_intersection(const vector<string>& grid) const noexcept
	{
		return can_go(grid, LEFT) + can_go(grid, RIGHT) + can_go(grid, UP) + can_go(grid, DOWN) > 1;
	}
	constexpr bool go_to_next(const vector<string>& grid) noexcept
	{ // Returns true if it can continue (at an intersection)
		auto& [ r, c ] = pos;
		bool ret = true;
		do {
			if(!can_go(grid, dir)) {
				if(can_go(grid, UP))
					dir = UP;
				else if(can_go(grid, DOWN))
					dir = DOWN;
				else if(can_go(grid, LEFT))
					dir = LEFT;
				else if(can_go(grid, RIGHT))
					dir = RIGHT;
				else {
					ret = false;
					break;
				}
			}
			dir == UP ? --r : dir == DOWN ? ++r : dir == LEFT ? --c : ++c;
			++length;
		} while(!at_intersection(grid));

		return ret;
	}
	constexpr void split_at_intersection(const vector<string>& grid, queue<Path>& paths) const noexcept
	{
		if(ranges::find(decisions, pos) != decisions.cend()) // Only if it hasn't been visited yet
			return;
		auto path_copy = *this;
		path_copy.decisions.push_back(pos);

		if(can_go(grid, LEFT)) {
			path_copy.dir = LEFT;
			paths.push(path_copy);
		}
		if(can_go(grid, RIGHT)) {
			path_copy.dir = RIGHT;
			paths.push(path_copy);
		}
		if(can_go(grid, UP)) {
			path_copy.dir = UP;
			paths.push(path_copy);
		}
		if(can_go(grid, DOWN)) {
			path_copy.dir = DOWN;
			paths.push(path_copy);
		}
	}
};

size_t longest_path(const vector<string>& grid, const point_t& start, const point_t& end)
{
	queue<Path> paths;
	size_t longest = 0;

	paths.emplace(start, Path::DOWN);
	do {
		auto path = paths.front();
		paths.pop();

		if(path.go_to_next(grid)) // Continue to next intersection
			path.split_at_intersection(grid, paths);
		else if(path.pos == end) // If an intersection wasn't reached,
			longest = max(longest, path.length);
	} while(!paths.empty());

	return longest;
}

int main(void)
{
	vector<string> grid;
	string line;

	while(getline(cin, line)) {
		grid.push_back(line);
		assert(line.size() == grid.front().size());
	}
	const auto start_c = grid.front().find_first_of('.'),
	           end_c = grid.back().find_first_of('.');
	assert(start_c != string::npos && end_c != string::npos);

	cout << longest_path(grid, { 0, start_c }, { grid.size() - 1, end_c }) << '\n';
	return 0;
}

