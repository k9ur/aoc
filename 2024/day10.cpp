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

struct Map : vector<string>
{
	constexpr size_t rows(void) const noexcept
	{
		return size();
	}
	constexpr size_t cols(void) const noexcept
	{
		return front().size();
	}
};

void find_trailheads(const Map& map, const size_t start_r, const size_t start_c, uint32_t& score_sum)
{
	set<Position> ends, new_ends;
	ends.emplace(start_r, start_c);

	for (const auto step : views::iota('1', '9' + 1)) {
		new_ends.clear();
		for (const auto& [ r, c ] : ends) {
			if (r                   && map[r - 1][c] == step)
				new_ends.emplace(r - 1, c);
			if (c                   && map[r][c - 1] == step)
				new_ends.emplace(r, c - 1);
			if (r != map.rows() - 1 && map[r + 1][c] == step)
				new_ends.emplace(r + 1, c);
			if (c != map.cols() - 1 && map[r][c + 1] == step)
				new_ends.emplace(r, c + 1);
		}
		ends = new_ends;
		if (ends.empty())
			break;
	}

	score_sum += ends.size();
}

int main(void)
{
	Map map{};
	uint32_t score_sum = 0;

	{
		string s;
		while (getline(cin, s)) {
			map.push_back(s);
			assert(s.size() == map.cols());
		}
	}

	for (size_t c, r = 0; r != map.rows(); ++r)
		for (c = 0; c != map.cols(); ++c)
			if (map[r][c] == '0')
				find_trailheads(map, r, c, score_sum);

	cout << score_sum << '\n';
	return 0;
}

