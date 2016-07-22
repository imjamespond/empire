#pragma once

#include <unordered_map>

namespace codechiev
{
	class AStar
	{
	public:
		AStar();

	private:
		typedef std::unordered_map<std::string, int> coord_map;
		coord_map openMap_;
		coord_map closeMap_;

		char coords_[10][10];
	};

}

