#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <charconv>
#include <concepts>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <utility>
#include <iterator>
#include <execution>
#include <memory>
#include <cstdint>

using namespace std;

template<integral T, int base = 10>
T svto(const string_view& sv)
{
	T val;
	[[maybe_unused]] const errc ec = from_chars(sv.cbegin(), sv.cend(), val, base).ec;
	assert(ec == errc());
	return val;
}

struct Monkey
{
	vector<uint64_t> items;
	const size_t ind_if_t;
	const size_t ind_if_f;
	uint32_t inspections = 0;
	const uint8_t divisor;
	const uint8_t op_val;
	const uint8_t op : 2;

	constexpr Monkey(vector<uint64_t>& _items, size_t _ind_if_t, size_t _ind_if_f, uint8_t _divisor, uint8_t _op, uint8_t _op_val = 0)
	  : items(_items)
	  , ind_if_t(_ind_if_t)
	  , ind_if_f(_ind_if_f)
	  , divisor(_divisor)
	  , op_val(_op_val)
	  , op(_op)
	{}

	constexpr void add_inspections(void) noexcept
	{
		inspections += items.size();
	}
	constexpr void interpret_op(uint64_t& item) const noexcept
	{
		switch(op) {
			case 1:
				item += op_val;
				break;
			case 2:
				item *= op_val;
				break;
			case 3:
				item *= item;
				break;
		}
	}
};

int main(void)
{
	constexpr uint16_t rounds = 10'000;
	vector<unique_ptr<Monkey>> monkeys;
	string line;
	
	while(!cin.eof()) {
		getline(cin, line);

		vector<uint64_t> items;
		uint64_t val;
		getline(cin, line);
		istringstream is(line.substr(18));
		while(is >> val) {
			items.push_back(val);
			is.get();
		}

		getline(cin, line);
		uint8_t op, op_val{};
		if(line[23] == '*') {
			if(line[25] == 'o')
				op = 3;
			else {
				op = 2;
				op_val = svto<uint8_t>(string_view(line).substr(25));
			}
		} else {
			op = 1;
			op_val = svto<uint8_t>(string_view(line).substr(25));
		}

		getline(cin, line);
		const auto divisor = svto<uint8_t>(string_view(line).substr(21));
		getline(cin, line);
		const auto ind_if_t = svto<uint8_t>(string_view(line).substr(29));
		getline(cin, line);
		const auto ind_if_f = svto<uint8_t>(string_view(line).substr(30));

		if(!cin.eof())
			getline(cin, line);
		monkeys.push_back(make_unique<Monkey>(items, ind_if_t, ind_if_f, divisor, op, op_val));
	}

	const auto total_divisor = transform_reduce(execution::par_unseq, next(monkeys.cbegin()), monkeys.cend(), static_cast<uint64_t>(monkeys.front()->divisor), lcm<uint64_t, uint64_t>, [](const unique_ptr<Monkey>& b) {
		return b->divisor;
	});

	for(uint16_t r = 0; r != rounds; ++r)
		for(auto& monkey : monkeys) {
			monkey->add_inspections();
			for(auto item : monkey->items) {
				monkey->interpret_op(item);
				item %= total_divisor;
				monkeys[item % monkey->divisor ? monkey->ind_if_f : monkey->ind_if_t]->items.push_back(move(item));
			}
			monkey->items.clear();
		}

	sort(monkeys.begin(), monkeys.end(), [](const auto& a, const auto& b) {
		return a->inspections > b->inspections;
	});

	cout << static_cast<uint64_t>(monkeys[0]->inspections) * monkeys[1]->inspections << '\n';
	return 0;
}

