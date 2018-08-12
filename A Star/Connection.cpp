#include "Connection.h"


using namespace Simplex;

Simplex::Connection::Connection()
{
}

Simplex::Connection::Connection(String aNode, String bNode, uint cost)
{
	nodeOne = aNode;
	nodeTwo = bNode;
	myCost = cost;
}


Simplex::Connection::~Connection()
{
}

