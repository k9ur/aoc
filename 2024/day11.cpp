#include <iostream>
#include <array>
#include <list>
#include <ranges>
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
	constexpr uint8_t cycles = 25;

	uint64_t stone;
	size_t final_stones = 0;

	while (cin >> stone) {
		list<uint64_t> stones;
		stones.push_back(stone);

		for ([[maybe_unused]] const auto cycle : views::iota(uint8_t{}, cycles))
			for (auto it = stones.begin(); it != stones.end(); ++it)
				if (*it) {
					const uint8_t digs = digits(*it);
					if (digs & 1)
						*it *= 2024;
					else {
						const uint64_t half_div = ten_pows[digs >> 1];
						stones.insert(it, *it / half_div);
						*it %= half_div;
					}
				} else
					*it = 1;

		final_stones += stones.size();
	}

	cout << final_stones << '\n';
	return 0;
}

