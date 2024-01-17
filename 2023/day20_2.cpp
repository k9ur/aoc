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
#include <functional>
#include <iterator>
#include <execution>
#include <cassert>
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

	constexpr bool parse_pulse(queue<pulse_t>& q, const string& from, const bool high) noexcept
	{ // Returns the pulse sent, or low if none
		bool cond;
		if(type == '%') {
			if(high)
				return false;
			on = !on;
			cond = on;
		} else if(type == '&') {
			inputs[from] = high;
			cond = !ranges::all_of(inputs, static_cast<const bool& (*)(const pair<const string, bool>&)>(get<1>));
		} else
			cond = high;
		for(const auto& dest : dests)
			q.emplace(dest, name, cond);
		return cond;
	}
};

int main(void)
{
	unordered_map<string, Module> modules;
	string line;

	vector<string> rx_inputs;
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
			if(name == "rx"s)
				rx_inputs.push_back(mod.name);
			if(!iss.eof())
				iss.get(); // Skip space
		}
		if(type == '%')
			mod.on = false;
	}
	assert(rx_inputs.size());

	for(auto& [ name, mod ] : modules)
		if(mod.type == '&') // Get all inputs
			for(const auto& [ input_name, input_mod ] : modules)
				if(ranges::find(input_mod.dests, name) != input_mod.dests.cend())
					mod.inputs[input_name] = false;

	
	const auto og_inputs = rx_inputs.size();
	for(size_t input_i = 0; input_i != og_inputs; ++input_i) {
		const auto& input_name = rx_inputs.front();
		const auto& mod = modules[input_name];
		assert(mod.type == '&'); // Solution only works with these
		for(const auto& [ k, v ] : mod.inputs) {
			const auto& input_mod = modules[k];
			assert(input_mod.type == '&'); // Solution only works with these
			rx_inputs.emplace_back(k);
		}
		shift_left(rx_inputs.begin(), rx_inputs.end(), 1);
		rx_inputs.pop_back();
	}

	vector<uint32_t> puh(rx_inputs.size(), 0); // Button presses it took for one of the &->&->rx modules to emit a high pulse

	queue<pulse_t> pulse_q;
	uint64_t button_i = 0;
	do {
		++button_i;
		pulse_q.emplace("broadcaster"s, ""s, false); // Missing the b since it was used in type

		do {
			const auto [ name, from, high ] = pulse_q.front();
			pulse_q.pop();

			if(!modules.contains(name))
				continue;
			const auto emitted_high = modules[name].parse_pulse(pulse_q, from, high);
			const vector<string>::const_iterator iter = ranges::find(rx_inputs, name);
			if(emitted_high && iter != rx_inputs.cend()) { // Emitted high to the &->rx module
				auto& presses = puh[distance(rx_inputs.cbegin(), iter)];
				if(presses == 0)
					presses = button_i;
			}
		} while(!pulse_q.empty());
	} while(!ranges::all_of(puh, identity()));

	cout << reduce(execution::par_unseq, next(puh.cbegin()), puh.cend(), static_cast<uint64_t>(puh.front()), lcm<uint64_t, uint64_t>) << '\n';
	return 0;
}

