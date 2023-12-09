#include <iostream>
#include <ios>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cstdint>
#include <cctype>

using namespace std;

int main(void)
{
	vector<uint32_t> needed;
	string line;
	uint32_t total_points = 0;

	while(getline(cin, line)) {
		uint32_t num, points = 1;
		istringstream iss(line);
		iss.ignore(numeric_limits<streamsize>::max(), ':');
		while(iss >> num)
			needed.push_back(num);

		iss = istringstream(line);
		iss.ignore(numeric_limits<streamsize>::max(), '|');
		while(iss >> num)
			if(find(needed.cbegin(), needed.cend(), num) != needed.cend())
				points <<= 1;

		needed.clear();
		total_points += points >> 1;
	}

	cout << total_points << '\n';
	return 0;
}

