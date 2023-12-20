#include <iostream>
#include <string>
#include <sstream>
#include <array>
#include <algorithm>
#include <functional>
#include <iterator>
#include <execution>
#include <cstdint>

using namespace std;

int main(void)
{
	array<uint32_t, 3> min_needed = { 0 };
	string line;
	uint32_t pow_sum = 0;

	while(getline(cin, line)) {
		string colour;
		uint32_t count;
		istringstream iss(line);
		iss.ignore(iss.str().size(), ':');

		while(iss >> count >> colour) {
			const auto first_l = colour[0];
			const size_t arr_i = first_l == 'r' ? 0 : first_l == 'g' ? 1 : 2;
			min_needed[arr_i] = max(min_needed[arr_i], count);
		}

		pow_sum += reduce(execution::par_unseq, next(min_needed.cbegin()), min_needed.cend(), min_needed.front(), multiplies());
		min_needed.fill(0);
	}

	cout << pow_sum << '\n';
	return 0;
}

