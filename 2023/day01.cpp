#include <iostream>
#include <string>
#include <cstdint>
#include <cctype>

using namespace std;

int main(void)
{
	string line;
	uint32_t num_sum = 0;

	while(getline(cin, line)) {
		for(auto i = line.cbegin(); i != line.cend(); ++i)
			if(isdigit(*i)) {
				num_sum += (*i - '0') * 10;
				break;
			}
		for(auto i = line.crbegin(); i != line.crend(); ++i)
			if(isdigit(*i)) {
				num_sum += *i - '0';
				break;
			}
	}

	cout << num_sum << '\n';
	return 0;
}

