#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <limits>
#include <cassert>
#include <cstdint>

using namespace std;

int main(void)
{
	unordered_map<string, pair<string, string>> node_map;
	string line, inst, word;
	size_t steps = 0;

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

	word = "AAA"s;
	do {
		assert(node_map.contains(word));
		word = inst[steps % inst.size()] == 'L' ? node_map[word].first : node_map[word].second;
		++steps;
	} while(word != "ZZZ"s);

	cout << steps << '\n';
	return 0;
}

