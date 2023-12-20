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

template<subscriptable T>
constexpr bool rows_same(const vector<T>& grid, const size_t i, const size_t j)
{
	return grid[i] == grid[j];
}
template<subscriptable T>
constexpr bool cols_same(const vector<T>& grid, const size_t i, const size_t j)
{
	for(const auto& s : grid)
		if(s[i] != s[j])
			return false;
	return true;
}

template<subscriptable T>
constexpr uint32_t summarise(const function<bool(const vector<T>&, const size_t, const size_t)>& same_func, const vector<T>& grid, const size_t cols, const uint32_t mult = 1)
{
	size_t i_upper, i_lower, i = 0;
	while(++i != cols)
		if(same_func(grid, i, i - 1)) {
			bool refl = true;
			i_upper = i;
			i_lower = i; // One too high
			while(--i_lower && ++i_upper != cols)
				if(!same_func(grid, i_upper, i_lower - 1)) {
					refl = false;
					break;
				}
			if(refl)
				return i * mult;
		}

	return 0;
}

int main(void)
{
	vector<string> grid;
	string line;
	uint32_t summary = 0;

	while(getline(cin, line)) {
		if(line.empty()) {
			summary += summarise(rows_same<string>, grid, grid.size(), 100);
			summary += summarise(cols_same<string>, grid, grid.front().size());
			grid.clear();
		} else {
			grid.push_back(line);
			assert(line.size() == grid.front().size());
		}
	}
	summary += summarise(rows_same<string>, grid, grid.size(), 100);
	summary += summarise(cols_same<string>, grid, grid.front().size());

	cout << summary << '\n';
	return 0;
}

