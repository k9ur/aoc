#include <iostream>
#include <array>
#include <unordered_map>
#include <ranges>
#include <numeric>
#include <functional>
#include <execution>
#include <cstdint>

using namespace std;

constexpr uint8_t digits(uint64_t num)
{
	uint8_t res = 1;
	while (num /= 10)
		++res;
	return res;
}

int main(void)
{
	constexpr array<uint64_t, 10> ten_pows = { 1, 10, 100, 1'000, 10'000, 100'000, 1'000'000, 10'000'000, 100'000'000, 1'000'000'000 };
	constexpr uint8_t cycles = 75;

	unordered_map<uint64_t, uint64_t> stone_counts, new_stone_counts;

	{
		uint64_t stone;
		while (cin >> stone)
			++stone_counts[stone];
	}

	for ([[maybe_unused]] const auto cycle : views::iota(uint8_t{}, cycles)) {
		new_stone_counts.clear();
		for (const auto& [ stone, count ] : stone_counts)
			if (stone) {
				const uint8_t digs = digits(stone);
				if (digs & 1)
					new_stone_counts[stone * 2024] += count;
				else {
					const uint64_t half_div = ten_pows[digs >> 1];
					new_stone_counts[stone / half_div] += count;
					new_stone_counts[stone % half_div] += count;
				}
			} else
				new_stone_counts[1] += count;
		stone_counts = new_stone_counts;
	}

	cout << transform_reduce(execution::par_unseq, stone_counts.cbegin(), stone_counts.cend(), static_cast<uint64_t>(0), plus(), mem_fn(&decltype(stone_counts)::value_type::second)) << '\n';
	return 0;
}

