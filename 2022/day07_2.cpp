#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <cstdint>

using namespace std;

struct Dir
{
	vector<unique_ptr<Dir>> sub_dirs;
	Dir* const parent_dir = nullptr;
	const string name;
	uint32_t size = 0;

	constexpr Dir() : name("/") {}
	constexpr Dir(Dir& _parent_dir, string&& _name)
	  : parent_dir(&_parent_dir)
	  , name(move(_name))
	{}

	constexpr void dfs_update_size() noexcept
	{
		for(auto& sub_dir : sub_dirs)
			sub_dir->dfs_update_size();

		if(parent_dir != nullptr)
			parent_dir->size += size;
	}
	constexpr void dfs_find_size(uint32_t& smallest, const uint32_t required) const noexcept
	{
		if(size < required) // All subdirs will be even smaller
			return;
		for(auto& sub_dir : sub_dirs)
			sub_dir->dfs_find_size(smallest, required);

		if(size < smallest)
			smallest = size;
	}
};

int main(void)
{
	constexpr uint32_t total_size = 70'000'000,
	                   required_size = 30'000'000;
	Dir root;
	Dir* cur_dir = nullptr;
	string line;

	while(getline(cin, line)) {
		if(line[0] == '$') { // Command

			if(line[2] == 'c') {
				if(line[5] == '.') { // Go back
					cur_dir = cur_dir->parent_dir;
					assert(cur_dir != nullptr);
				} else if(line[5] == '/') // Root
					cur_dir = &root;
				else {
					const auto new_dir_name = string_view(line).substr(5);
					for(auto& sub_dir : cur_dir->sub_dirs)
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

	root.dfs_update_size();

	assert(total_size > root.size);
	const auto unused_size = total_size - root.size;
	assert(required_size > unused_size);
	const auto ntf_size = required_size - unused_size; // Need to free size

	auto smallest = root.size;
	root.dfs_find_size(smallest, ntf_size);

	cout << smallest << '\n';
	return 0;
}

