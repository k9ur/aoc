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
	for(auto& star : stars)
		if(star.x == col && star.y == row) {
			star.gr *= val;
			++star.count;
			break;
		}
}

template<char star_ch = '*'>
void add_to_surrounding_stars(const vector<string>& schem, vector<Star>& stars, const size_t row, const size_t start, const size_t end, const uint32_t val)
{
	const auto cols = schem.front().size(),
	           rows = schem.size();
	if(start && schem[row][start - 1] == star_ch)
		add_to_star(stars, row, start - 1, val);
	if(end != cols && schem[row][end] == star_ch)
		add_to_star(stars, row, end, val);
	size_t i;
	if(row) { // Check above
		for(i = start; i != end; ++i)
			if(schem[row - 1][i] == star_ch)
				add_to_star(stars, row - 1, i, val);
		if(start && schem[row - 1][start - 1] == star_ch)
			add_to_star(stars, row - 1, start - 1, val);
		if(end != cols && schem[row - 1][end] == star_ch)
			add_to_star(stars, row - 1, end, val);
	}
	if(row != rows - 1) { // Check below
		for(i = start; i != end; ++i)
			if(schem[row + 1][i] == star_ch)
				add_to_star(stars, row + 1, i, val);
		if(start && schem[row + 1][start - 1] == star_ch)
			add_to_star(stars, row + 1, start - 1, val);
		if(end != cols && schem[row + 1][end] == star_ch)
			add_to_star(stars, row + 1, end, val);
	}
}

int main(void)
{
	constexpr auto star_ch = '*';
	vector<string> schematic;
	vector<Star> stars;
	string line;

	while(getline(cin, line)) {
		schematic.push_back(line);
		assert(line.size() == schematic.front().size());
	}

	const auto cols = schematic.front().size(),
	           rows = schematic.size();

	size_t col, row;
	for(row = 0; row != rows; ++row)
		for(col = 0; col != cols; ++col)
			if(schematic[row][col] == star_ch)
				stars.emplace_back(col, row);

	for(row = 0; const string_view sv : schematic) {
		for(col = 0; col < cols; ++col)
			if(isdigit(sv[col])) {
				auto start_col = col;
				do
					++col;
				while(col < cols && isdigit(sv[col]));
				const auto val = svto<uint32_t>(sv.substr(start_col, col - start_col));
				add_to_surrounding_stars<star_ch>(schematic, stars, row, start_col, col, val);
			}
		++row;
	}

	uint32_t gr_sum = 0;
	for(const auto& star : stars)
		if(star.count == 2)
			gr_sum += star.gr;

	cout << gr_sum << '\n';
	return 0;
}

