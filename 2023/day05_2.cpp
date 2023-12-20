#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <sstream>
#include <utility>
#include <algorithm>
#include <cassert>
#include <cstdint>

using namespace std;

int main(void)
{
	vector<pair<uint64_t, uint64_t>> seed_ranges, new_ranges; // [start, end)
	vector<tuple<uint64_t, uint64_t, uint64_t>> mappings; // dest, [start, end)
	string line;

	getline(cin, line);
	uint64_t start, range;
	istringstream iss(line.substr(7));
	while(iss >> start >> range)
		seed_ranges.emplace_back(start, start + range);

	getline(cin, line); // Skip empty line
	while(!cin.eof()) {
		getline(cin, line); // Skip map name
		getline(cin, line);
		do {
			uint64_t dest, src, len;
			iss = istringstream(line);
			iss >> dest >> src >> len;

			mappings.emplace_back(dest, src, src + len);
			getline(cin, line);
		} while(!cin.eof() && !line.empty());

		for(size_t i = 0; i != seed_ranges.size(); ++i) {
			const auto& r = seed_ranges[i];
			const auto [ r_start, r_end ] = r;
			bool done = false;
			for(const auto& m : mappings) {
				const auto [ m_dest, m_start, m_end ] = m;
				if(m_start <= r_start && r_end <= m_end) { // Covers whole range: [r_start, r_end)
					new_ranges.emplace_back(r_start - m_start + m_dest, r_end - m_start + m_dest);
					done = true;
				} else if(m_start <= r_start && r_start < m_end && m_end < r_end) { // Covers bottom part of range: [r_start, m_end)
					new_ranges.emplace_back(r_start - m_start + m_dest, m_end - m_start + m_dest);
					seed_ranges.emplace_back(m_end, r_end);
					done = true;
				} else if(r_start < m_start && m_start < r_end && r_end <= m_end) { // Covers top part of range: [m_start, r_end)
					new_ranges.emplace_back(/* m_start - m_start + */ m_dest, r_end - m_start + m_dest);
					seed_ranges.emplace_back(r_start, m_start);
					done = true;
				} else if(r_start < m_start && m_end < r_end) { // In middle of range: [m_start, m_end)
					new_ranges.emplace_back(/* m_start - m_start + */ m_dest, m_end - m_start + m_dest);
					seed_ranges.emplace_back(r_start, m_start);
					seed_ranges.emplace_back(m_end, r_end);
					done = true;
				}
				if(done)
					break;
			}
			if(!done)
				new_ranges.push_back(r);
		}

		mappings.clear();
		seed_ranges = move(new_ranges);
		new_ranges.clear();
	}

	const auto min_loc = ranges::min_element(seed_ranges, [](const auto& a, const auto& b) {
		return a.first < b.first;
	});
	assert(min_loc != seed_ranges.cend());
	cout << (*min_loc).first << '\n';
	return 0;
}

