#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <cassert>
#include <cstdint>

using namespace std;

struct WorkflowRule
{
	string dest;
	uint16_t num;
	char letter;
	bool gt;

	constexpr WorkflowRule() {}
	constexpr WorkflowRule(string&& _dest)
	  : dest(move(_dest))
	  , letter('\0')
	{}
	constexpr WorkflowRule(string&& _dest, uint16_t _num, char _letter, bool _gt)
	  : dest(move(_dest))
	  , num(_num)
	  , letter(_letter)
	  , gt(_gt)
	{}
};

struct PartRanges
{
	using value_type = decltype(WorkflowRule::num);
	using range_type = pair<value_type, value_type>;

	range_type x, m, a, s;

	constexpr PartRanges() {}
	constexpr PartRanges(value_type _min, value_type _max)
	  : x(_min, _max)
	  , m(_min, _max)
	  , a(_min, _max)
	  , s(_min, _max)
	{}
	constexpr PartRanges(range_type _x, range_type _m, range_type _a, range_type _s)
	  : x(_x)
	  , m(_m)
	  , a(_a)
	  , s(_s)
	{}

	constexpr bool invalid(void) const noexcept
	{
		return x.second < x.first || m.second < m.first || a.second < a.first || s.second < s.first;
	}
	constexpr uint64_t combinations(void) const noexcept
	{
		return static_cast<uint64_t>(x.second - x.first + 1) * (m.second - m.first + 1) * (a.second - a.first + 1) * (s.second - s.first + 1);
	}
	template<bool using_condition = true>
	PartRanges merge_with(const WorkflowRule& rule) const noexcept
	{ // May return an invalid range. If using_condition is false, returns the merged ranges as if the condition was flipped
		assert(rule.letter != '\0');
		auto new_ranges = *this;
		auto& range = rule.letter == 'x' ? new_ranges.x : rule.letter == 'm' ? new_ranges.m : rule.letter == 'a' ? new_ranges.a : new_ranges.s;
		if(rule.gt ^ using_condition) // Act as lt
			range.second = min(range.second, using_condition ? static_cast<value_type>(rule.num - 1) : rule.num);
		else // Act as gt
			range.first = max(range.first, using_condition ? static_cast<value_type>(rule.num + 1) : rule.num);
		return new_ranges;
	}
};

uint64_t dfs_possible(const unordered_map<string, vector<WorkflowRule>>& workflows, const PartRanges& ranges, const string& workflow_name, const size_t rule_idx)
{
	if(ranges.invalid() || workflow_name == "R"s)
		return 0;
	if(workflow_name == "A"s)
		return ranges.combinations();
	assert(workflows.contains(workflow_name));
	const auto& rules = workflows.at(workflow_name);
	const auto& rule = rules[rule_idx];
	if(rule.letter == '\0')
		return dfs_possible(workflows, ranges, rule.dest, 0);
	return dfs_possible(workflows, ranges.merge_with<true>(rule), rule.dest, 0)
		+ dfs_possible(workflows, ranges.merge_with<false>(rule), workflow_name, rule_idx + 1);
}

int main(void)
{
	unordered_map<string, vector<WorkflowRule>> workflows;
	string line;

	while(getline(cin, line) && !line.empty()) {
		vector<WorkflowRule> rules;
		string name, rule, dest;
		uint16_t num;
		char letter, gt;
		istringstream iss(line);

		getline(iss, name, '{');
		while(getline(iss, rule, ',')) {
			if(rule.back() == '}')
				rule = rule.substr(0, rule.size() - 1);
			istringstream rule_iss(rule);
			if(rule.size() < 2 || (rule[1] != '<' && rule[1] != '>')) {
				assert(rule != name);
				rules.emplace_back(move(rule));
			} else {
				rule_iss >> letter >> gt >> num;
				rule_iss.get(); // Skip :
				rule_iss >> dest;
				assert(dest != name);
				rules.emplace_back(move(dest), num, letter, gt == '>');
			}
		}
		workflows[name] = rules;
	}
	while(getline(cin, line));

	cout << dfs_possible(workflows, PartRanges(1, 4000), "in"s, 0) << '\n';
	return 0;
}

