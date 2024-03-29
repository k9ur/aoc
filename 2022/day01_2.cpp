#include <iostream>
#include <string>
#include <numeric>
#include <algorithm>
#include <array>
#include <execution>
#include <cstdint>

using namespace std;

template<size_t N>
void update(uint32_t& cal, array<uint32_t, N>& max_cals)
{
	size_t i;
	for(i = N; i; --i)
		if(cal <= max_cals[i - 1])
			break;
	if(i != N) {
		move(next(max_cals.begin(), i), prev(max_cals.end()), next(max_cals.begin(), i + 1));
		max_cals[i] = cal;
	}
	cal = 0;
}

int main(void)
{
	constexpr size_t size = 3;
	string line;
	array<uint32_t, size> max_cals = { 0 };
	uint32_t cal = 0;

	while(getline(cin, line)) {
		if(line.empty())
			update(cal, max_cals);
		else
			cal += stoul(line);
	}
	update(cal, max_cals);

	cout << reduce(execution::par_unseq, max_cals.cbegin(), max_cals.cend()) << '\n';
	return 0;
}

