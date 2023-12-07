#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <charconv>
#include <concepts>
#include <cstdint>
#include <cassert>
#include <cctype>

using namespace std;

struct Star
{
	size_t x;
	size_t y;
	uint32_t gr = 1; // Gear ratio
	uint8_t count = 0;

	constexpr Star() {}
	constexpr Star(size_t c, size_t r)
	  : x(c)
	  , y(r)
	{}
};

template<integral T, int base = 10>
T svto(const string_view& sv)
{
	T val;
	[[maybe_unused]] const errc ec = from_chars(sv.cbegin(), sv.cend(), val, base).ec;
	assert(ec == errc());
	return val;
}

inline void add_to_star(vector<Star>& stars, const size_t row, const size_t col, const uint32_t val)
{
	for(Star& star : stars)
		if(star.x == col && star.y == row) {
			star.gr *= val;
			++star.count;
			break;
		}
}

void add_to_surrounding_stars(const vector<string>& schem, vector<Star>& stars, const size_t row, const size_t start, const size_t end, const uint32_t val)
{
	const auto cols = schem[0].size(),
	           rows = schem.size();
	if(start && schem[row][start - 1] == '*')
		add_to_star(stars, row, start - 1, val);
	if(end != cols && schem[row][end] == '*')
		add_to_star(stars, row, end, val);
	size_t i;
	if(row) { // Check above
		for(i = start; i != end; ++i)
			if(schem[row - 1][i] == '*')
				add_to_star(stars, row - 1, i, val);
		if(start && schem[row - 1][start - 1] == '*')
			add_to_star(stars, row - 1, start - 1, val);
		if(end != cols && schem[row - 1][end] == '*')
			add_to_star(stars, row - 1, end, val);
	}
	if(row != rows - 1) { // Check below
		for(i = start; i != end; ++i)
			if(schem[row + 1][i] == '*')
				add_to_star(stars, row + 1, i, val);
		if(start && schem[row + 1][start - 1] == '*')
			add_to_star(stars, row + 1, start - 1, val);
		if(end != cols && schem[row + 1][end] == '*')
			add_to_star(stars, row + 1, end, val);
	}
}

int main(void)
{
	vector<string> schematic;
	vector<Star> stars;
	string line;
	string::size_type size = 0;

	while(getline(cin, line)) {
		schematic.push_back(line);
		if(size) [[likely]]
			assert(size == line.size());
		else [[unlikely]]
			assert(size = line.size());
	}

	const size_t cols = size,
	             rows = schematic.size();

	size_t col, row;
	for(row = 0; row != rows; ++row)
		for(col = 0; col != cols; ++col)
			if(schematic[row][col] == '*')
				stars.emplace_back(col, row);

	for(row = 0; row != rows; ++row) {
		const string_view sv = schematic[row];
		for(col = 0; col < cols; ++col)
			if(isdigit(sv[col])) {
				string::size_type start_col = col;
				do
					++col;
				while(col < cols && isdigit(sv[col]));
				const auto val = svto<uint32_t>(sv.substr(start_col, col - start_col));
				add_to_surrounding_stars(schematic, stars, row, start_col, col, val);
			}
	}

	uint32_t gr_sum = 0;
	for(const auto& star : stars)
		if(star.count == 2)
			gr_sum += star.gr;

	cout << gr_sum << '\n';
	return 0;
}

