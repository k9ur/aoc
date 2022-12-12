#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <cstdint>

using namespace std;

struct Dir {
	vector<Dir*> sub_dirs;
	Dir* const parent_dir = nullptr;
	const string name;
	uint32_t size = 0;

	Dir(Dir* const _parent_dir, const string&& _name) : parent_dir(_parent_dir), name(move(_name)) {}

	constexpr void dfs_update_size() noexcept {
		for(Dir* const sub_dir : sub_dirs)
			sub_dir->dfs_update_size();

		if(parent_dir != nullptr)
			parent_dir->size += size;
	}
	constexpr void dfs_find_size(uint32_t& smallest, const uint32_t required) const noexcept {
		if(size < required) // All subdirs will be even smaller
			return;
		for(const Dir* const sub_dir : sub_dirs)
			sub_dir->dfs_find_size(smallest, required);

		if(size < smallest)
			smallest = size;
	}
};

int main(void) {
	constexpr uint32_t total_size = 70'000'000,
	                   required_size = 30'000'000;
	string line;
	Dir root = Dir(nullptr, "/");
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
					const string new_dir_name = line.substr(5);
					for(Dir* const sub_dir : cur_dir->sub_dirs)
						if(sub_dir->name == new_dir_name) {
							cur_dir = sub_dir;
							goto repeat;
						}
					exit(EXIT_FAILURE);
				}
			}

		} else if(line[0] == 'd') // New dir
			cur_dir->sub_dirs.push_back(new Dir(cur_dir, line.substr(4)));
		else // File size
			cur_dir->size += stoul(line);
repeat:
		;
	}

	root.dfs_update_size();

	assert(total_size > root.size);
	const uint32_t unused_size = total_size - root.size;
	assert(required_size > unused_size);
	const uint32_t ntf_size = required_size - unused_size; // Need to free size

	uint32_t smallest = root.size;
	root.dfs_find_size(smallest, ntf_size);

	cout << smallest << '\n';
	return 0;
}
