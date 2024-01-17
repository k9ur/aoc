#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <tuple>
#include <utility>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <execution>
#include <cstdint>

using namespace std;

using pulse_t = tuple<string, string, bool>; // name of current module, where the pulse came from, whether it was a high pulse

struct Module
{
	string name;
	vector<string> dests;
	unordered_map<string, bool> inputs; // Only for &
	bool on; // Only for %
	char type; // Either %, &, or b (broadcaster)

	constexpr Module() {}
	constexpr Module(string&& _name, vector<string>&& _dests, char _type, bool _on)
	  : name(move(_name))
	  , dests(move(_dests))
	  , on(_on)
	  , type(_type)
	{}
	constexpr Module(string&& _name, vector<string>&& _dests, char _type)
	  : name(move(_name))
	  , dests(move(_dests))
	  , inputs()
	  , type(_type)
	{}

	constexpr void parse_pulse(queue<pulse_t>& q, const string& from, const bool high) noexcept
	{
		bool cond;
		if(type == '%') {
			if(high)
				return;
			on = !on;
			cond = on;
		} else if(type == '&') {
			inputs[from] = high;
			cond = !ranges::all_of(inputs, static_cast<const bool& (*)(const pair<const string, bool>&)>(get<1>));
		} else
			cond = high;
		for(const auto& dest : dests)
			q.emplace(dest, name, cond);
	}
};

constexpr bool end_early(const unordered_map<string, Module>& modules)
{
	for(const auto& [ name, mod ] : modules) {
		if(mod.type == '%' && mod.on)
			return false;
		else if(mod.type == '&')
			for(const auto& [ input_name, input_high ] : mod.inputs)
				if(input_high)
					return false;
	}
	return true;
}

int main(void)
{
	constexpr uint16_t presses = 1000;
	unordered_map<string, Module> modules;
	string line;

	while(getline(cin, line)) {
		string name;
		char type;
		istringstream iss(line);

		iss >> type; // %, &, or b
		iss >> name;
		iss.get();
		iss.ignore(3, ' '); // Skip ->
		auto& mod = modules[type == 'b' ? "broadcaster"s : name];
		mod.name = name;
		mod.type = type;

		while(getline(iss, name, ',')) {
			mod.dests.push_back(name);
			if(!iss.eof())
				iss.get(); // Skip space
		}
		if(type == '%')
			mod.on = false;
	}

	for(auto& [ name, mod ] : modules)
		if(mod.type == '&') // Get all inputs
			for(const auto& [ input_name, input_mod ] : modules)
				if(ranges::find(input_mod.dests, name) != input_mod.dests.cend())
					mod.inputs[input_name] = false;

	queue<pulse_t> pulse_q;
	vector<pair<uint32_t, uint32_t>> counts; // Cache to find loops and prevent it having to execute all 1000 button presses
	// Ended up not being useful for the input
	pair<uint32_t, uint32_t> total_counts;

	uint16_t button_i = 0;
	do {
		++button_i;
		uint32_t l_count{}, h_count{};
		pulse_q.emplace("broadcaster"s, ""s, false); // Missing the b since it was used in type

		do {
			const auto [ name, from, high ] = pulse_q.front();
			pulse_q.pop();

			high ? ++h_count : ++l_count;
			if(modules.contains(name))
				modules[name].parse_pulse(pulse_q, from, high);
		} while(!pulse_q.empty());

		total_counts.first += l_count;
		total_counts.second += h_count;
		counts.emplace_back(l_count, h_count);
		if(end_early(modules))
			break;
	} while(button_i != presses);

	const auto cycles_leftover = presses % button_i;
	const uint16_t cycles = presses / button_i;
	total_counts.first *= cycles;
	total_counts.second *= cycles;
	if(cycles_leftover) { // Add the rest
		const auto& [ lows, highs ] = reduce(execution::par_unseq, next(counts.cbegin()), next(counts.cbegin(), cycles_leftover), counts.front(), [](const auto& a, const auto& b) {
			return pair(a.first + b.first, a.second + b.second);
		});
		total_counts.first += lows;
		total_counts.second += highs;
	}

	cout << static_cast<uint64_t>(total_counts.first) * total_counts.second << '\n';
	return 0;
}

