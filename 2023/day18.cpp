#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cassert>
#include <cstdint>

using namespace std;

template<class T>
struct NegativeIndexGrid
{
	using index_type = int64_t;

  private:
	vector<vector<T>> pypx, pynx, nypx, nynx;
	const T def_val{};
	index_type min_y = numeric_limits<index_type>::max(),
	           min_x = numeric_limits<index_type>::max(),
	           max_y = numeric_limits<index_type>::min(),
	           max_x = numeric_limits<index_type>::min();

  public:
	constexpr NegativeIndexGrid() {}
	constexpr NegativeIndexGrid(T _def_val)
	  : def_val(_def_val)
	{}

	constexpr void set(index_type y, index_type x, const T val) noexcept
	{
		min_y = min(min_y, y);
		min_x = min(min_x, x);
		max_y = max(max_y, y);
		max_x = max(max_x, x);
		auto& v = y >= 0 ? (x >= 0 ? pypx : pynx) : x >= 0 ? nypx : nynx;
		if(y < 0)
			y = -y - 1;
		if(x < 0)
			x = -x - 1;
		if(v.size() <= static_cast<size_t>(y))
			v.resize(y + 1);
		if(v[y].size() <= static_cast<size_t>(x))
			v[y].resize(x + 1, def_val);
		v[y][x] = val;
	}

	constexpr vector<vector<T>> conv_to_normal(void) const noexcept
	{
		if(min_y > max_y || min_x > max_x)
			return {};
		vector<vector<T>> grid(max_y - min_y + 1, vector<T>(max_x - min_x + 1, def_val));

		for(uint8_t grid_i = 0; const auto& grid_quart : { pypx, pynx, nypx, nynx }) {
			for(size_t r = grid_i & 2 ? 0 : -min_y; const auto& row : grid_quart) {
				for(size_t c = grid_i & 1 ? 0 : -min_x; const auto t : row) {
					grid[grid_i & 2 ? -min_y - 1 - r : r][grid_i & 1 ? -min_x - 1 - c : c] = t;
					++c;
				}
				++r;
			}
			++grid_i;
		}
		return grid;
	}
};

size_t count_enclosed(const vector<vector<bool>>& grid)
{ // Will only work if edges don't touch
	size_t enclosed = 0;
	for(size_t c, r = 0; const auto& row : grid) {
		bool conn_above, conn_below, inside = false;
		for(c = 0; c != row.size(); ++c) {
			if(row[c]) {
				++enclosed;
				conn_above = r && grid[r - 1][c];
				conn_below = r != grid.size() - 1 && grid[r + 1][c];
				if(conn_above && conn_below)
					inside = !inside;
				else {
					do {
						++c;
						++enclosed;
					} while(c != row.size() - 1 && row[c + 1]);
					const bool end_conn_above = r && grid[r - 1][c],
                               end_conn_below = r != grid.size() - 1 && grid[r + 1][c];
					assert(end_conn_above ^ end_conn_below); // This solution won't work with this data
					if((conn_above && end_conn_below) || (conn_below && end_conn_above))
						inside = !inside;
				}
			} else if(inside)
				++enclosed;
		}
		++r;
	}
	return enclosed;
}

int main(void)
{
	NegativeIndexGrid<bool> nigrid{};
	NegativeIndexGrid<bool>::index_type x{}, y{};
	string line;

	while(getline(cin, line)) {
		char dir;
		uint16_t steps;
		istringstream iss(line);
		iss >> dir >> steps;

		while(steps) {
			nigrid.set(dir == 'D' ? ++y : dir == 'U' ? --y : y, dir == 'R' ? ++x : dir == 'L' ? --x : x, true);
			--steps;
		}
	}
	assert(y == 0 && x == 0);

	cout << count_enclosed(nigrid.conv_to_normal()) << '\n';
	return 0;
}

