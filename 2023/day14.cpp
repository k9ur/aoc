#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdint>

using namespace std;

struct Dish
{
	vector<string> grid;

	constexpr Dish() {}

	constexpr size_t rows(void) const noexcept
	{
		return grid.size();
	}
	constexpr size_t cols(void) const noexcept
	{
		return grid.front().size();
	}

	constexpr uint32_t move_north(const size_t r, const size_t c) noexcept
	{
		size_t new_r = r;
		while(new_r && grid[new_r - 1][c] == '.')
			--new_r;
		grid[r][c] = '.';
		grid[new_r][c] = 'O';
		return rows() - new_r;
	}

	constexpr void cycle_north(uint32_t& load) noexcept
	{
		for(size_t c, r = 0; const auto& row : grid) {
			for(c = 0; c != cols(); ++c)
				if(row[c] == 'O')
					load += move_north(r, c);
			++r;
		}
	}
};

int main(void)
{
	Dish dish{};
	string line;
	uint32_t load = 0;

	while(getline(cin, line)) {
		dish.grid.push_back(line);
		assert(line.size() == dish.cols());
	}
	assert(dish.rows());

	dish.cycle_north(load);

	cout << load << '\n';
	return 0;
}

