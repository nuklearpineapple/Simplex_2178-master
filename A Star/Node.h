#pragma once

#include "Definitions.h"

namespace Simplex
{

	class Node
	{
	public:

		uint fCost;
		uint distance; // distance from home node
		String name; // name of node
		std::vector<uint> connections; 

		Node();
		~Node();
	};

}