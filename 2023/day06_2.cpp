#include <iostream>
#include <string>
#include <sstream>
#include <concepts>
#include <cmath>
#include <cstdint>

using namespace std;

#ifdef gnu
[[using gnu : const]]
#endif
template<unsigned_integral T1, unsigned_integral T2, unsigned base = 10>
constexpr uint64_t concat_ints(const T1 x, const T2 y)
{
    uint64_t pow = base;
    while(y >= pow)
        pow *= base;
    return pow * x + y;
}

int main(void)
{
	string line;
	uint64_t t{}, d{};
	uint32_t num;

	getline(cin, line);
	istringstream iss(line.substr(10));
	while(iss >> num)
		t = concat_ints(t, num);

	getline(cin, line);
	iss = istringstream(line.substr(10));
	while(iss >> num)
		d = concat_ints(d, num);

	++d;
	if(t * t < d * 4)
		cout << 0 << '\n';
	else {
		const double sqrt_val = sqrt(t * t - d * 4);
		const uint64_t lower = ceil((-sqrt_val - t) / 2),
		               upper = floor((sqrt_val - t) / 2);
		cout << (upper < lower ? 0 : upper - lower + 1) << '\n';
	}

	return 0;
}

