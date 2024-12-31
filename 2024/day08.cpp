#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <ranges>
#include <cassert>
#include <cstdint>

using namespace std;

using Position = pair<size_t, size_t>; // r, c

struct City : vector<string>
{
	constexpr size_t rows(void) const noexcept
	{
		return size();
	}
	constexpr size_t cols(void) const noexcept
	{
		return front().size();
	}

	constexpr bool in_bounds(const int64_t r, const int64_t c) const noexcept
	{
		return r >= 0 && r < static_cast<int64_t>(rows())
			&& c >= 0 && c < static_cast<int64_t>(cols());
	}
};

void add_antinodes(const City& map, set<Position>& antinode_positions, const char freq)
{
	constexpr int64_t i64_two = 2;

	vector<Position> freq_positions;

	for (size_t c, r = 0; r != map.rows(); ++r)
		for (c = 0; c != map.cols(); ++c)
			if (map[r][c] == freq)
				freq_positions.emplace_back(r, c);

	for (size_t i = 0; const auto& pos_a : freq_positions)
		for (const auto& pos_b : freq_positions | views::drop(++i)) {
			int64_t antinode_r = i64_two * pos_b.first  - pos_a.first,
			        antinode_c = i64_two * pos_b.second - pos_a.second;
			if (map.in_bounds(antinode_r, antinode_c))
				antinode_positions.emplace(antinode_r, antinode_c);

			antinode_r = i64_two * pos_a.first  - pos_b.first;
			antinode_c = i64_two * pos_a.second - pos_b.second;
			if (map.in_bounds(antinode_r, antinode_c))
				antinode_positions.emplace(antinode_r, antinode_c);
		}
}

int main(void)
{
	City map{};
	set<Position> antinode_positions;

	{
		string s;
		while (getline(cin, s)) {
			map.push_back(s);
			assert(s.size() == map.cols());
		}
	}

	for (const auto c : views::iota('0', '9' + 1))
		add_antinodes(map, antinode_positions, c);
	for (const auto c : views::iota('A', 'Z' + 1))
		add_antinodes(map, antinode_positions, c);
	for (const auto c : views::iota('a', 'z' + 1))
		add_antinodes(map, antinode_positions, c);

	cout << antinode_positions.size() << '\n';
	return 0;
}

