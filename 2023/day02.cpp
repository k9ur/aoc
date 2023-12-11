#include <iostream>
#include <string>
#include <sstream>
#include <array>
#include <limits>
#include <cstdint>

using namespace std;

int main(void)
{
	constexpr array<const uint32_t, 3> limits = { 12, 13, 14 }; // r g b order
	string line;
	uint32_t ID_sum{}, ID = 1;

	while(getline(cin, line)) {
		string colour;
		uint32_t count;
		bool poss = true;
		istringstream iss(line);
		iss.ignore(numeric_limits<streamsize>::max(), ':');
	
		while(iss >> count >> colour) {
			const auto first_l = colour[0];
			const size_t lim_i = first_l == 'r' ? 0 : first_l == 'g' ? 1 : 2;
			if(count > limits[lim_i]) {
				poss = false;
				break;
			}
		}

		if(poss)
			ID_sum += ID;
		++ID;
	}

	cout << ID_sum << '\n';
	return 0;
}

