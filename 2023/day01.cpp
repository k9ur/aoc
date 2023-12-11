#include <iostream>
#include <string>
#include <ranges>
#include <cstdint>
#include <cctype>

using namespace std;

int main(void)
{
	string line;
	uint32_t num_sum = 0;

	while(getline(cin, line)) {
		for(const auto c : line)
			if(isdigit(c)) {
				num_sum += (c - '0') * 10;
				break;
			}
		for(const auto c : line | views::reverse)
			if(isdigit(c)) {
				num_sum += c - '0';
				break;
			}
	}

	cout << num_sum << '\n';
	return 0;
}

