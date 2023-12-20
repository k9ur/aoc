#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <array>
#include <utility>
#include <iterator>
#include <cstdint>

using namespace std;

using lens_t = pair<string, uint8_t>;

#ifdef gnu
[[using gnu : const]]
#endif
constexpr uint8_t hash_func(const string& s)
{
	uint16_t val = 0;
	for(const auto c : s) {
		val += c;
		val *= 17;
		val %= 256;
	}
	return val;
}

int main(void)
{
	array<vector<lens_t>, 256> boxes;
	string line, inst;

	getline(cin, line);
	istringstream iss(line);
	while(getline(iss, inst, ',')) {
		const auto lens_name = inst.substr(0, inst.size() - (inst.back() == '-' ? 1 : 2));
		auto& box = boxes[hash_func(lens_name)];
		const auto iter = ranges::find_if(box, [&lens_name](const auto& lens) {
			return lens.first == lens_name;
		});

		if(inst.back() == '-') {
			if(iter != box.end()) {
				shift_left(iter, box.end(), 1);
				box.pop_back();
			}
		} else {
			const auto focal_len = static_cast<uint8_t>(inst.back() - '0');
			if(iter == box.end())
				box.emplace_back(lens_name, focal_len);
			else
				(*iter).second = focal_len;
		}
	}

	uint32_t focus_pow = 0;
	for(size_t slot_i, box_i = 1; const auto& box : boxes) {
		for(slot_i = 1; const auto& lens : box) {
			focus_pow += box_i * slot_i * lens.second;
			++slot_i;
		}
		++box_i;
	}

	cout << focus_pow << '\n';
	return 0;
}

