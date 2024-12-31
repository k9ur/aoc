#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <stack>
#include <cstdint>

using namespace std;

bool possible(const vector<string_view>& patterns, const string_view& design)
{
	stack<size_t> st;
	vector<bool> visited(design.size() + 1);

	st.push(0);
	visited[0] = true;
	do {
		const auto pos = st.top();
		st.pop();

		if (pos == design.size())
			return true;
		for (const auto& pattern : patterns) {
			const auto next_pos = pos + pattern.size();
			if (!visited[next_pos] && design.substr(pos, pattern.size()) == pattern) {
				st.push(next_pos);
				visited[next_pos] = true;
			}
		}
	} while (!st.empty());

	return false;
}

int main(void)
{
	vector<string_view> patterns;
	string pattern_list, design;
	uint32_t possible_designs = 0;

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
	while (getline(cin, design))
		if (possible(patterns, string_view{ design }))
			++possible_designs;

	cout << possible_designs << '\n';
	return 0;
}

