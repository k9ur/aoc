#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <concepts>
#include <cassert>
#include <cstdint>

using namespace std;

template<class T>
concept subscriptable = requires(T a)
{
	a[0];
};

template<subscriptable T> // Generic difference-counting function
constexpr size_t rows_diffc(const vector<T>& grid, const size_t i, const size_t j)
{
	const auto& s1 = grid[i];
	const auto& s2 = grid[j];
	assert(s1.size() == s2.size());
	size_t diff = 0;
	for(size_t idx = 0; idx != s1.size(); ++idx)
		diff += s1[idx] != s2[idx];
	return diff;
}
template<subscriptable T> // Generic difference-counting function
constexpr size_t cols_diffc(const vector<T>& grid, const size_t i, const size_t j)
{
	size_t diff = 0;
	for(const auto& s : grid)
		diff += s[i] != s[j];
	return diff;
}

template<subscriptable T>
constexpr uint32_t summarise(const function<size_t(const vector<T>&, const size_t, const size_t)>& diff_func, const vector<T>& grid, const size_t cols, const uint32_t mult = 1)
{
	size_t i_upper, i_lower, i = 0;
	while(++i != cols) {
		const auto init_diff = diff_func(grid, i, i - 1);
		if(init_diff < 2) {
			bool used_smudge = init_diff == 1,
			     refl = true;
			i_upper = i;
			i_lower = i; // One too high
			while(--i_lower && ++i_upper != cols) {
				const auto new_diff = diff_func(grid, i_upper, i_lower - 1);
				if(new_diff) {
					if(used_smudge || new_diff > 1) {
						refl = false;
						break;
					} else
						used_smudge = true;
				}
			}
			if(refl && used_smudge)
				return i * mult;
		}
	}

	return 0;
}

int main(void)
{
	vector<string> grid;
	string line;
	size_t cols = 0;
	uint32_t summary = 0;

	while(getline(cin, line)) {
		if(line.empty()) {
			summary += summarise(rows_diffc<string>, grid, grid.size(), 100);
			summary += summarise(cols_diffc<string>, grid, cols);
			grid.clear();
			cols = 0;
		} else {
			grid.push_back(line);
			if(cols)
				assert(cols == line.size());
			else
				assert(cols = line.size());
		}
	}
	summary += summarise(rows_diffc<string>, grid, grid.size(), 100);
	summary += summarise(cols_diffc<string>, grid, cols);

	cout << summary << '\n';
	return 0;
}

