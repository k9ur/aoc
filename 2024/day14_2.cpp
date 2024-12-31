#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <execution>
#include <cstdint>

using namespace std;

constexpr uint32_t positive_mod(const int64_t n, const uint32_t mod)
{
	return (n % mod + mod) % mod;
}

struct Robot
{
	uint32_t x, y;
	int32_t v_x, v_y;

	constexpr Robot() {}
	constexpr Robot(const uint32_t _x, const uint32_t _y, const int32_t _v_x, const int32_t _v_y)
	  : x(_x)
	  , y(_y)
	  , v_x(_v_x)
	  , v_y(_v_y)
	{}

	template<uint32_t width, uint32_t height>
	void move(void) noexcept
	{
		int64_t x_int = x,
		        y_int = y;
		x = positive_mod(x_int + v_x, width);
		y = positive_mod(y_int + v_y, height);
	}
};

int main(void)
{
	constexpr uint32_t width = 101,
	                   height = 103;

	vector<Robot> robots, robots_sorted;
	uint32_t seconds = 0;

	{
		string s;
		while (getline(cin, s)) {
			istringstream iss(s);
			uint32_t x, y;
			int32_t v_x, v_y;

			iss.ignore(2);
			iss >> x;
			iss.ignore(1);
			iss >> y;
			iss.ignore(3);
			iss >> v_x;
			iss.ignore(1);
			iss >> v_y;

			robots.emplace_back(x, y, v_x, v_y);
		}
	}

	// this part is stupid

	vector<Robot>::const_iterator adj_find;
	do {
		for (auto& robot : robots)
			robot.move<width, height>();
		++seconds;

		robots_sorted = robots;
		sort(execution::par_unseq, robots_sorted.begin(), robots_sorted.end(), [](const auto& a, const auto& b) {
			return a.x < b.x || a.y < b.y;
		});
		adj_find = adjacent_find(execution::par_unseq, robots_sorted.cbegin(), robots_sorted.cend(), [](const auto& a, const auto& b) {
			return a.x == b.x && a.y == b.y;
		});
	} while (adj_find != robots_sorted.cend());

	cout << seconds << '\n';
	return 0;
}

