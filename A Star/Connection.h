#pragma once

#include "Definitions.h"

namespace Simplex
{

	class Connection
	{
	public:

		uint cost; // size of the connection
		// from node 1
		// to node 2

		Connection();
		~Connection();
	};

}