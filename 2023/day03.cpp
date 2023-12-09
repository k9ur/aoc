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

template<integral T, int base = 10>
T svto(const string_view& sv)
{
	T val;
	[[maybe_unused]] const errc ec = from_chars(sv.cbegin(), sv.cend(), val, base).ec;
	assert(ec == errc());
	return val;
}

#ifdef gnu
[[using gnu : const]]
#endif
constexpr bool is_symbol(const char c)
{
	return c != '.' && !isdigit(c);
}

bool symbol_in_surroundings(const vector<string>& schem, const size_t row, const size_t start, const size_t end)
{
	const auto cols = schem[0].size(),
	           rows = schem.size();
	if(start && is_symbol(schem[row][start - 1]))
		return true;
	if(end != cols && is_symbol(schem[row][end]))
		return true;
	size_t i;
	if(row) { // Check above
		for(i = start; i != end; ++i)
			if(is_symbol(schem[row - 1][i]))
				return true;
		if(start && is_symbol(schem[row - 1][start - 1]))
			return true;
		if(end != cols && is_symbol(schem[row - 1][end]))
			return true;
	}
	if(row != rows - 1) { // Check below
		for(i = start; i != end; ++i)
			if(is_symbol(schem[row + 1][i]))
				return true;
		if(start && is_symbol(schem[row + 1][start - 1]))
			return true;
		if(end != cols && is_symbol(schem[row + 1][end]))
			return true;
	}
	return false;
}

int main(void)
{
	vector<string> schematic;
	string line;
	string::size_type size = 0;
	uint32_t part_sum = 0;

	while(getline(cin, line)) {
		schematic.push_back(line);
		if(size) [[likely]]
			assert(size == line.size());
		else [[unlikely]]
			assert(size = line.size());
	}

	const auto cols = size,
	           rows = schematic.size();

	for(size_t col, row = 0; row != rows; ++row) {
		const string_view sv = schematic[row];
		for(col = 0; col < cols; ++col)
			if(isdigit(sv[col])) {
				string::size_type start_col = col;
				do
					++col;
				while(col < cols && isdigit(sv[col]));
				if(symbol_in_surroundings(schematic, row, start_col, col))
					part_sum += svto<uint32_t>(sv.substr(start_col, col - start_col));
			}
	}

	cout << part_sum << '\n';
	return 0;
}

