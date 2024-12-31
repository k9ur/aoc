#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <cstdint>

using namespace std;

uint64_t dp_solve(vector<uint64_t>& dp, const vector<string_view>& patterns, const string_view& design, const size_t pos)
{
	if (dp[pos])
		return dp[pos];
	else if (pos == design.size())
		return 1;

	uint64_t res = 0;
	for (const auto& pattern : patterns) {
		const auto next_pos = pos + pattern.size();
		if (design.substr(pos, pattern.size()) == pattern)
			res += dp_solve(dp, patterns, design, next_pos);
	}

	dp[pos] = res;
	return res;
}

int main(void)
{
	vector<string_view> patterns;
	string pattern_list, design;
	uint64_t possible_designs = 0;

	{
		getline(cin, pattern_list);

		const string_view list_sv = pattern_list;
		size_t start_idx{}, end_idx = list_sv.find(", "sv);

		while (end_idx != string_view::npos) {
			patterns.push_back(list_sv.substr(start_idx, end_idx - start_idx));
			start_idx = end_idx + 2;
			end_idx = list_sv.find(", "sv, start_idx);
		}
		patterns.push_back(list_sv.substr(start_idx));
	}

	getline(cin, design);
	while (getline(cin, design)) {
		const string_view design_view = design;
		vector<uint64_t> dp(design_view.size() + 1);
		possible_designs += dp_solve(dp, patterns, design_view, 0);
	}

	cout << possible_designs << '\n';
	return 0;
}

