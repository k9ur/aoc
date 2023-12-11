#include <iostream>
#include <string>
#include <algorithm>
#include <cstdint>

using namespace std;

constexpr void update(uint32_t& cal, uint32_t& max_cal)
{
	max_cal = max(max_cal, cal);
	cal = 0;
}

int main(void)
{	
	string line;
	uint32_t cal{}, max_cal{};

	while(getline(cin, line)) {
		if(line.empty())
			update(cal, max_cal);
		else
			cal += stoul(line);
	}
	update(cal, max_cal);

	cout << max_cal << '\n';
	return 0;
}

