#pragma once

#include "Definitions.h"

namespace Simplex
{

	class Connection
	{
	public:

		uint myCost; // size of the connection
		String nodeOne; // from node 
		String nodeTwo; // to node

		Connection();
		Connection(String aNode, String bNode, uint cost);
		~Connection();
	};

}