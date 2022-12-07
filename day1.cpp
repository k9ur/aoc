#include <iostream>
#include <string>
#include <cstdint>

using namespace std;

static inline constexpr void update(uint32_t& cal, uint32_t& max_cal) {
	if(cal > max_cal)
		max_cal = cal;
	cal = 0;
}

int main(void) {
	uint32_t cal = 0,
	         max_cal = 0;
	string line;

	while(getline(cin, line)) {
		if(line.empty())
			update(cal, max_cal);
		else
			cal += stoi(line);
	}
	update(cal, max_cal);

	cout << max_cal << '\n';
	return 0;
}
