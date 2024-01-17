#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <cstdint>

using namespace std;

using grid_t = vector<string>;

struct Dish
{
	grid_t grid;

	constexpr Dish() {}

	constexpr size_t rows(void) const noexcept
	{
		return grid.size();
	}
	constexpr size_t cols(void) const noexcept
	{
		return grid.front().size();
	}

	template<bool north>
	constexpr void move_NS(const size_t r, const size_t c) noexcept
	{
		size_t new_r = r;
		while((north ? new_r : new_r != rows() - 1) && grid[north ? new_r - 1 : new_r + 1][c] == '.')
			north ? --new_r : ++new_r;
		grid[r][c] = '.';
		grid[new_r][c] = 'O';
	}
	template<bool west>
	constexpr void move_WE(const size_t r, const size_t c) noexcept
	{
		size_t new_c = c;
		while((west ? new_c : new_c != cols() - 1) && grid[r][west ? new_c - 1 : new_c + 1] == '.')
			west ? --new_c : ++new_c;
		grid[r][c] = '.';
		grid[r][new_c] = 'O';
	}

	template<bool north>
	constexpr void cycle_NS(void) noexcept
	{
		for(size_t c, r = north ? 1 : rows(); north ? r <= rows() : r; north ? ++r : --r) {
			const auto& row = grid[r - 1];
			for(c = 0; c != cols(); ++c)
				if(row[c] == 'O')
					move_NS<north>(r - 1, c);
		}
	}
	template<bool west>
	constexpr void cycle_WE(void) noexcept
	{
		for(size_t r, c = west ? 1 : cols(); west ? c <= cols() : c; west ? ++c : --c)
			for(r = 0; r != rows(); ++r)
				if(grid[r][c - 1] == 'O')
					move_WE<west>(r, c - 1);
	}

	template<bool north>
	constexpr uint32_t calc_load_NS(void) const noexcept
	{
		uint32_t load = 0;
		for(size_t r = 0; const auto& row : grid) {
			for(const auto c : row)
				if(c == 'O')
					load += north ? rows() - r : r + 1;
			++r;
		}
		return load;
	}
	template<bool west>
	constexpr uint32_t calc_load_WE(void) const noexcept
	{
		uint32_t load = 0;
		for(size_t r, c = 0; c != cols(); ++c) {
			for(r = 0; r != rows(); ++r)
				if(grid[r][c] == 'O')
					load += west ? cols() - c : c + 1;
			++c;
		}
		return load;
	}
};

int main(void)
{
	constexpr uint32_t cycles = 1'000'000'000;
	Dish dish{};
	string line;

	while(getline(cin, line)) {
		dish.grid.push_back(line);
		assert(line.size() == dish.cols());
	}
	assert(dish.rows());

	vector<pair<uint32_t, uint32_t>> loads; // Gives a hint of the loop without having to store the entire grid each time
	vector<pair<grid_t, uint32_t>> grid_cache; // Cache grid and index whenever a load-pair repeats
	uint32_t cycle_i = 0;
	do {
		dish.cycle_NS<true>();
		dish.cycle_WE<true>();
		dish.cycle_NS<false>();
		dish.cycle_WE<false>();

		const pair load_pair = { dish.calc_load_NS<true>(), dish.calc_load_WE<true>() };
		if(ranges::find(loads, load_pair) != loads.cend()) {
			const auto iter = ranges::find_if(grid_cache, [&dish](const auto& cache) {
				return cache.first == dish.grid;
			});
			if(iter != grid_cache.cend()) {
				const uint32_t loop_begin = iter->second,
				               loop_len = loads.size() - loop_begin;
				cycle_i = loop_begin + ((cycles - 1 - loop_begin) % loop_len) + 1;
				break;
			} else
				grid_cache.emplace_back(dish.grid, cycle_i);
		}
		loads.push_back(load_pair);
	} while(++cycle_i != cycles);

	cout << loads[cycle_i - 1].first << '\n';
	return 0;
}

