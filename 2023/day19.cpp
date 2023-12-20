#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <cassert>
#include <cstdint>

using namespace std;

struct Part
{
	uint16_t x, m, a, s;

	constexpr Part() {}
	constexpr Part(uint16_t _x, uint16_t _m, uint16_t _a, uint16_t _s)
	  : x(_x)
	  , m(_m)
	  , a(_a)
	  , s(_s)
	{}
};

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

	constexpr bool compare(const Part& part) const noexcept
	{
		if(letter == '\0')
			return true;
		const auto part_num = letter == 'x' ? part.x : letter == 'm' ? part.m : letter == 'a' ? part.a : part.s;
		return gt ? part_num > num : part_num < num;
	}
};

bool is_accepted(const unordered_map<string, vector<WorkflowRule>>& workflows, const Part& part)
{
	auto workflow_name = "in"s;
	do {
		assert(workflows.contains(workflow_name));
		const auto& rules = workflows.at(workflow_name);
		for(const auto& rule : rules)
			if(rule.compare(part)) {
				workflow_name = rule.dest;
				break;
			}
	} while(workflow_name != "A"s && workflow_name != "R"s);
	return workflow_name == "A"s;
}

int main(void)
{
	unordered_map<string, vector<WorkflowRule>> workflows;
	string line;
	Part part;
	uint32_t accepted_sum = 0;

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
	while(getline(cin, line)) {
		istringstream iss(line);
		iss.ignore(iss.str().size(), '=');
		iss >> part.x;
		iss.ignore(iss.str().size(), '=');
		iss >> part.m;
		iss.ignore(iss.str().size(), '=');
		iss >> part.a;
		iss.ignore(iss.str().size(), '=');
		iss >> part.s;
		if(is_accepted(workflows, part))
			accepted_sum += part.x + part.m + part.a + part.s;
	}

	cout << accepted_sum << '\n';
	return 0;
}

