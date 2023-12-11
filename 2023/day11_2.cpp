#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <utility>
#include <concepts>
#include <cassert>
#include <cstdint>
#include <cmath>

using namespace std;

#ifdef gnu
[[using gnu : const]]
#endif
template<unsigned_integral T>
constexpr T udist(const T x, const T y)
{
	return x > y ? x - y : y - x;
}

int main(void)
{
	constexpr size_t expansion_f = 1'000'000;
	constexpr char galaxy_ch = '#';
	vector<pair<size_t, size_t>> galaxies; // x, y
	vector<bool> has_col, has_row;
	string line;

	size_t x, y{}, cols{};
	while(getline(cin, line)) {
		if(cols) [[likely]]
			assert(cols == line.size());
		else [[unlikely]] {
			assert(cols = line.size());
			has_col.resize(cols, false);
		}
		bool row_has_galaxy = false;
		for(x = 0; x != cols; ++x)
			if(line[x] == galaxy_ch) {
				galaxies.emplace_back(x, y);
				has_col[x] = true;
				row_has_galaxy = true;
			}
		has_row.push_back(row_has_galaxy);
		if(row_has_galaxy)
			++y;
		else // fix y
			y += expansion_f;
	}
	size_t incr = 0;
	for(x = 0; x != cols; ++x) // fix x
		if(!has_col[x]) {
			for(auto& [ gal_x, gal_y ] : galaxies)
				if(gal_x > incr && gal_x - incr > x)
					gal_x += expansion_f - 1;
			incr += expansion_f - 1;
		}

	size_t path_sum = 0;
	for(size_t i = 0; const auto& galA : galaxies)
		for(const auto& galB : galaxies | views::drop(++i))
			path_sum += udist(galA.first, galB.first) + udist(galA.second, galB.second);

	cout << path_sum << '\n';
	return 0;
}

