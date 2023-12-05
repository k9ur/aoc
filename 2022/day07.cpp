#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <cstdint>

using namespace std;

struct Dir {
	vector<unique_ptr<Dir>> sub_dirs;
	Dir* const parent_dir = nullptr;
	const string name;
	uint32_t size = 0;

	static constexpr uint32_t max_size = 100'000;

	Dir() : name("/") {}
	Dir(Dir& _parent_dir, string&& _name)
	  : parent_dir(&_parent_dir)
	  , name(move(_name))
	{}

	constexpr void dfs_update(uint32_t& sum) noexcept {
		for(unique_ptr<Dir>& sub_dir : sub_dirs)
			sub_dir->dfs_update(sum);

		if(parent_dir != nullptr)
			parent_dir->size += size;
		if(size <= max_size)
			sum += size;
	}
};

int main(void) {
	string line;
	Dir root;
	Dir* cur_dir = nullptr;

	while(getline(cin, line)) {
		if(line[0] == '$') { // Command

			if(line[2] == 'c') {
				if(line[5] == '.') { // Go back
					cur_dir = cur_dir->parent_dir;
					assert(cur_dir != nullptr);
				} else if(line[5] == '/') // Root
					cur_dir = &root;
				else {
					const string_view new_dir_name = string_view(line).substr(5);
					for(unique_ptr<Dir>& sub_dir : cur_dir->sub_dirs)
						if(sub_dir->name == new_dir_name) {
							cur_dir = sub_dir.get();
							goto repeat;
						}
					exit(EXIT_FAILURE);
				}
			}

		} else if(line[0] == 'd') // New dir
			cur_dir->sub_dirs.push_back(make_unique<Dir>(*cur_dir, line.substr(4)));
		else // File size
			cur_dir->size += stoul(line);
repeat:
		;
	}

	uint32_t sum = 0;
	root.dfs_update(sum);

	cout << sum << '\n';
	return 0;
}
