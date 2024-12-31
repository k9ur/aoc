#include <iostream>
#include <string>
#include <sstream>
#include <vector>
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
	void move(const int64_t times) noexcept
	{
		int64_t x_int = x,
		        y_int = y;
		x = positive_mod(x_int + times * v_x, width);
		y = positive_mod(y_int + times * v_y, height);
	}
};

int main(void)
{
	constexpr uint32_t width = 101,
	                   height = 103,
					   seconds = 100;

	vector<Robot> robots;

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

	for (auto& robot : robots)
		robot.move<width, height>(seconds);

	size_t q1{}, q2{}, q3{}, q4{};

	for (const auto& robot : robots)
		if (robot.x < (width >> 1)) {
			if (robot.y < (height >> 1))
				++q1;
			else if (robot.y > (height >> 1))
				++q3;
		} else if (robot.x > (width >> 1)) {
			if (robot.y < (height >> 1))
				++q2;
			else if (robot.y > (height >> 1))
				++q4;
		}

	cout << q1 * q2 * q3 * q4 << '\n';
	return 0;
}

