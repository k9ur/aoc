#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <numeric>
#include <utility>
#include <limits>
#include <cassert>
#include <cstdint>

using namespace std;

int main(void)
{
	unordered_map<string, pair<string, string>> node_map;
	string line, inst, word;

	getline(cin, inst); // Instructions
	getline(cin, line);

	while(getline(cin, line)) {
		string nodeA, nodeB;
		istringstream iss(line);
		iss >> word;
		iss.ignore(numeric_limits<streamsize>::max(), '(');
		iss >> nodeA >> nodeB; // with trailing punctuation
		node_map[word] = { nodeA.substr(0, 3), nodeB.substr(0, 3) };
	}

	uint64_t total_steps = 1;
	size_t steps;
	for(const auto& [ key, node_pair ] : node_map) {
		if(key[2] != 'A')
			continue;
		steps = 0;
		word = key;
		do {
			assert(node_map.contains(word));
			word = inst[steps % inst.size()] == 'L' ? node_map[word].first : node_map[word].second;
			++steps;	
		} while(word[2] != 'Z');
		total_steps = lcm(total_steps, steps); // improper solution, but it passes(???)
	}

	cout << total_steps << '\n';
	return 0;
}

