#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <cstdint>

using namespace std;

#ifdef gnu
[[using gnu : const]]
#endif
constexpr bool seen_yet(const char c)
{
	return c > '9';
}
#ifdef gnu
[[using gnu : const]]
#endif
constexpr char parse(const char c)
{
	return seen_yet(c) ? c - 10 : c;
}

constexpr void compute(string::value_type& ch, char& highest, uint32_t& visible)
{
	if(parse(ch) <= highest)
		return;
	highest = parse(ch);
	if(!seen_yet(ch)) {
		ch += 10;
		++visible;
	}
}

int main(void)
{
	vector<string> trees;
	string line;
	string::size_type size = 0;

	while(getline(cin, line)) {
		trees.push_back(line);
		if(size) [[likely]]
			assert(size == line.size());
		else [[unlikely]]
			assert(size = line.size());
	}

	const auto cols = size,
	           rows = trees.size();
	uint32_t visible = rows * 2 + cols * 2 - 4;

	// For each row
	for(size_t r = 1; r < rows - 1; ++r) {
		char highest = trees[r][0]; // Can be viewed from left
		for(size_t c = 1; c < cols - 1; ++c)
			compute(trees[r][c], highest, visible);
		highest = trees[r][cols - 1]; // Can be viewed from right
		for(size_t c = cols - 2; c; --c)
			compute(trees[r][c], highest, visible);
	}
	// For each col
	for(size_t c = 1; c < cols - 1; ++c) {
		auto highest = trees[0][c]; // Can be viewed from top
		for(size_t r = 1; r < rows - 1; ++r)
			compute(trees[r][c], highest, visible);
		highest = trees[rows - 1][c]; // Can be viewed from bottom
		for(size_t r = rows - 2; r; --r)
			compute(trees[r][c], highest, visible);
	}

	cout << visible << '\n';
	return 0;
}

