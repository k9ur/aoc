#include <iostream>
#include <string>
#include <numeric>
#include <array>
#include <cstdint>

using namespace std;

template<size_t N> // FIXME
inline constexpr void update(uint32_t& cal, array<uint32_t, N>& max_cals) {
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
	array<uint32_t, size> max_cals = { 0 };
	string line;

	while(getline(cin, line)) {
		if(line.empty())
			update(cal, max_cals);
		else
			cal += stoul(line);
	}
	update(cal, max_cals);

	cout << reduce(max_cals.cbegin(), max_cals.cend(), 0) << '\n';
	return 0;
}
