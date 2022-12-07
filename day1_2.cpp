#include <iostream>
#include <string>
#include <numeric>
#include <cstdint>

using namespace std;

static inline constexpr void update(uint32_t& cal, uint32_t max_cals[3]) {
	if(cal <= max_cals[2]) [[likely]] {}
	else if(cal <= max_cals[1])
		max_cals[2] = cal;
	else if(cal <= max_cals[0]) {
		max_cals[2] = max_cals[1];
		max_cals[1] = cal;
	} else [[unlikely]] {
		max_cals[2] = max_cals[1];
		max_cals[1] = max_cals[0];
		max_cals[0] = cal;
	}
	cal = 0;
}

int main(void) {
	constexpr size_t size = 3;
	uint32_t cal = 0;
	uint32_t max_cals[size] = { 0 };
	string line;

	while(getline(cin, line)) {
		if(line.empty())
			update(cal, max_cals);
		else
			cal += stoi(line);
	}
	update(cal, max_cals);

	cout << accumulate(max_cals, max_cals + size, 0) << '\n';
	return 0;
}
