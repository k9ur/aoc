#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cassert>
#include <cctype>

using namespace std;

int main(void)
{
	vector<uint32_t> needed;
	string line;
	uint32_t total_points = 0;
	string::size_type i, bar_idx = 0;

	while(getline(cin, line)) {
		uint32_t num, points = 1;

		if(!bar_idx)
			bar_idx = line.find_first_of('|');
		else
			assert(line[bar_idx] == '|');
		for(i = 6; line[i] != ':'; ++i);
		i += 2;

		istringstream iss(line.substr(i));
		while(iss >> num)
			needed.push_back(num);

		iss = istringstream(line.substr(bar_idx + 1));
		while(iss >> num)
			if(find(needed.cbegin(), needed.cend(), num) != needed.cend())
				points <<= 1;

		needed.clear();
		total_points += points >> 1;
	}

	cout << total_points << '\n';
	return 0;
}

