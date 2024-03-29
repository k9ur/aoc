#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <algorithm>
#include <utility>
#include <cassert>
#include <cstdint>
#include <cctype>

using namespace std;

enum class hand_t : uint8_t
{
	HIGH_CARD, ONE_PAIR, TWO_PAIR, THREE_OF_A_KIND, FULL_HOUSE, FOUR_OF_A_KIND, FIVE_OF_A_KIND
};
using enum hand_t;

#if gnu
[[using gnu : const]]
#endif
constexpr uint8_t card_val(const char c)
{
	assert(isdigit(c) || c == 'A' || c == 'K' || c == 'Q' || c == 'J' || c == 'T');
	return c == 'J' ? 0 : c == 'T' ? 9 : c == 'Q' ? 10 : c == 'K' ? 11 : c == 'A' ? 12 : c - '2' + 1;
}

constexpr hand_t hand_type(const string& str)
{
	array<string::size_type, 13> counts = { 0 };
	string::size_type cur_max = 0,
                      jokers = 0;
	bool second_pair = false;
	for(const auto c : str) {
		assert(isdigit(c) || c == 'A' || c == 'K' || c == 'Q' || c == 'J' || c == 'T');
		if(c == 'J')
			++jokers;
		else {
			const auto new_count = ++counts[card_val(c)];
			if(cur_max >= 2 && new_count == 2)
				second_pair = true;
			else
				cur_max = max(cur_max, new_count);
		}
	}
	cur_max += jokers;
	if(cur_max == 5)
		return FIVE_OF_A_KIND;
	else if(cur_max == 4)
		return FOUR_OF_A_KIND;
	else if(cur_max == 3)
		return second_pair ? FULL_HOUSE : THREE_OF_A_KIND;
	else if(cur_max == 2)
		return second_pair ? TWO_PAIR : ONE_PAIR;
	return HIGH_CARD;
}

int main(void)
{
	vector<pair<string, uint32_t>> hands;
	string line, cards;
	uint32_t bet, winnings = 0;

	while(getline(cin, line)) {
		istringstream iss(line);
		iss >> cards >> bet;
		hands.emplace_back(cards, bet);
	}

	ranges::sort(hands, [](const auto& a, const auto& b) -> bool {
		const auto& a_cards = a.first,
		            b_cards = b.first;
		const auto a_type = hand_type(a_cards),
		           b_type = hand_type(b_cards);
		if(a_type != b_type)
			return a_type < b_type;
		for(string::size_type i = 0; i != 5; ++i)
			if(a_cards[i] != b_cards[i])
				return card_val(a_cards[i]) < card_val(b_cards[i]);
		return true;
	});

	for(size_t i = 0; const auto& hand : hands)
		winnings += hand.second * ++i;

	cout << winnings << '\n';
	return 0;
}

