#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include <utility>
#include <concepts>
#include <cmath>
#include <cassert>
#include <cstdint>

using namespace std;

using vert_t = pair<int64_t, int64_t>;

template<class T>
concept string_like = requires(T s)
{
	basic_string_view{s};
};

template<int base = 16, string_like T>
constexpr uint64_t base_to_dec(const T& sv)
{
	static_assert(base > 0);
	uint64_t dec = 0;
	for(const auto c : sv) {
		dec *= base;
		dec += c > '9' ? c - 'a' + 10 : c - '0';
	}
	return dec;
}

constexpr uint64_t shoelace_area(const vector<vert_t>& verts)
{
	int64_t area = 0;
	for(size_t i = 0; i < verts.size() - 1; ++i)
		area += verts[i].first * verts[i + 1].second - verts[i + 1].first * verts[i].second;
	return static_cast<uint64_t>(abs(area)) >> 1;
}

template<unsigned_integral T1, unsigned_integral T2>
constexpr uint64_t picks_theorem_all_points(const T1 A, const T2 b)
{ // A = i + b/2 - 1; i + b = A + b/2 + 1
	return static_cast<uint64_t>(A) + (b >> 1) + 1;
}

int main(void)
{
	vector<vert_t> verts;
	string line, hexa;
	int64_t x{}, y{};
	uint64_t perimeter = 0;

	while(getline(cin, line)) {
		char dir;
		uint32_t steps;
		istringstream iss(line);
		iss >> dir >> steps >> hexa;
		steps = base_to_dec<16>(string_view(hexa).substr(2, 5));
		dir = hexa[7];

		perimeter += steps;
		dir == '0' ? x += steps : dir == '1' ? y += steps : dir == '2' ? x -= steps : y -= steps;
		verts.emplace_back(x, y);
	}
	assert(y == 0 && x == 0);

	// Want the number of all interior and exterior points
	cout << picks_theorem_all_points(shoelace_area(verts), perimeter) << '\n';
	return 0;
}

