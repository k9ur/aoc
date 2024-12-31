#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <ranges>
#include <cstdint>
#include <cassert>

using namespace std;

using Position = pair<size_t, size_t>; // r, c

struct Garden : vector<string>
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

uint8_t count_corners(const Garden& garden, const size_t r, const size_t c, const char plant_type)
{
	uint8_t corners = 0;

	// convex
	if ((!r || garden[r - 1][c] != plant_type) && (!c || garden[r][c - 1] != plant_type))
		++corners;
	if ((!r || garden[r - 1][c] != plant_type) && (c == garden.cols() - 1 || garden[r][c + 1] != plant_type))
		++corners;
	if ((r == garden.rows() - 1 || garden[r + 1][c] != plant_type) && (!c || garden[r][c - 1] != plant_type))
		++corners;
	if ((r == garden.rows() - 1 || garden[r + 1][c] != plant_type) && (c == garden.cols() - 1 || garden[r][c + 1] != plant_type))
		++corners;

	// concave
	if (r && c
	&& garden[r - 1][c - 1] != plant_type && garden[r - 1][c] == plant_type && garden[r][c - 1] == plant_type)
		++corners;
	if (r && c != garden.cols() - 1
	&& garden[r - 1][c + 1] != plant_type && garden[r - 1][c] == plant_type && garden[r][c + 1] == plant_type)
		++corners;
	if (r != garden.rows() - 1 && c
	&& garden[r + 1][c - 1] != plant_type && garden[r + 1][c] == plant_type && garden[r][c - 1] == plant_type)
		++corners;
	if (r != garden.rows() - 1 && c != garden.cols() - 1
	&& garden[r + 1][c + 1] != plant_type && garden[r + 1][c] == plant_type && garden[r][c + 1] == plant_type)
		++corners;

	assert(corners <= 4);
	return corners;
}

void bfs_add(queue<Position>& region, const Garden& garden, vector<vector<bool>>& visited, const size_t r, const size_t c, const char plant_type, const bool cond = true)
{
	if (cond && !visited[r][c] && garden[r][c] == plant_type) {
		region.emplace(r, c);
		visited[r][c] = true;
	}
}

uint32_t price(const Garden& garden, vector<vector<bool>>& visited, const size_t start_r, const size_t start_c)
{
	queue<Position> region;
	uint16_t area{}, sides{};
	const auto plant_type = garden[start_r][start_c];

	bfs_add(region, garden, visited, start_r, start_c, plant_type);
	do {
		const auto& [ r, c ] = region.front();

		sides += count_corners(garden, r, c, plant_type);
		bfs_add(region, garden, visited, r - 1, c, plant_type, r);
		bfs_add(region, garden, visited, r + 1, c, plant_type, r != garden.rows() - 1);
		bfs_add(region, garden, visited, r, c - 1, plant_type, c);
		bfs_add(region, garden, visited, r, c + 1, plant_type, c != garden.cols() - 1);

		++area;
		region.pop();
	} while (!region.empty());

	return static_cast<uint32_t>(area) * sides;
}

int main(void)
{
	Garden garden{};
	uint32_t total_price = 0;

	{
		string s;
		while (getline(cin, s)) {
			garden.push_back(s);
			assert(s.size() == garden.cols());
		}
	}

	vector<vector<bool>> visited(garden.rows(), vector<bool>(garden.cols()));

	for (const auto r : views::iota(size_t{}, garden.rows()))
		for (const auto c : views::iota(size_t{}, garden.cols()))
			if (!visited[r][c])
				total_price += price(garden, visited, r, c);

	cout << total_price << '\n';
	return 0;
}

