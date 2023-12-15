#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <cassert>

using namespace std;

#ifdef gnu
[[using gnu : const]]
#endif
constexpr bool can_be_working(const char c)
{
	return c != '#';
}
#ifdef gnu
[[using gnu : const]]
#endif
constexpr bool can_be_damaged(const char c)
{
	return c != '.';
}

int main(void)
{
	vector<vector<vector<int32_t>>> dp; // [s_i][n_i][cur_len] = cache
	vector<uint16_t> nums;
	string line, records;
	uint32_t count_sum = 0;

	function<uint32_t(const string::size_type, const size_t, const uint16_t)> dp_solve = [&records, &nums, &dp, &dp_solve](const string::size_type s_i, const size_t n_i, const uint16_t cur_len) -> uint32_t {
		if(s_i >= records.size()) // Can sometimes add multiple so check >= not ==
			return n_i == nums.size(); // Finished (= 1) only if last number was also finished, otherwise (= 0) solution doesn't work
		auto& cache = dp[s_i][n_i][cur_len];
		if(cache != -1)
			return cache;
		cache = 0; // Now working with it, unset from -1

		if(can_be_working(records[s_i]) && cur_len == 0)
			cache += dp_solve(s_i + 1, n_i, 0); // Adding a .
		if(can_be_damaged(records[s_i]) && n_i < nums.size()) {
			if(cur_len != nums[n_i] - 1) // Not yet at the last #
				cache += dp_solve(s_i + 1, n_i, cur_len + 1); // Add another #
			else if(s_i == records.size() - 1 || can_be_working(records[s_i + 1])) // Can't add last # if the next (if existent) is #
				cache += dp_solve(s_i + 2, n_i + 1, 0); // Adding last #, as well as mandatory following .
		}
		return cache;
	};

	while(getline(cin, line)) {
		uint16_t num, max_num = 0;
		istringstream iss(line);
		iss >> records;
		while(iss >> num) {
			nums.push_back(num);
			max_num = max(max_num, num);
			if(!iss.eof())
				iss.get();
		}
		assert(nums.size());

		// Need nums+1 to continue to end once finished, need max_num+1 to include both 0 and max_num
		dp = vector<vector<vector<int32_t>>>(records.size(), vector<vector<int32_t>>(nums.size() + 1, vector<int32_t>(max_num + 1, -1))); // Set all to -1

		count_sum += dp_solve(0, 0, 0);
		nums.clear();
	}

	cout << count_sum << '\n';
	return 0;
}

