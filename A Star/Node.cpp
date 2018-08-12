#include "Node.h"


using namespace Simplex;

Simplex::Node::Node()
{
}

Simplex::Node::Node(String thisName, uint thisDistance)
{
	name = thisName;
	distance = thisDistance;
}

Simplex::Node::~Node()
{
}

void Simplex::Node::AddConnection(Connection * connection)
{
	myConnections.push_back(connection);
}

String Simplex::Node::GetName()
{
	return name;
}

void Simplex::Node::SetFinal(bool ind)
{
	isFinal = ind;
}

void Simplex::Node::SetStart(bool ind)
{
	isStart = ind;
}

bool Simplex::Node::isFinalNode()
{
	return isFinal;
}

Connection * Simplex::Node::GetConnectionToNeighbor(String name)
{
	for (Connection* connection : myConnections) {
		String nameOne = connection->nodeOne;
		String nameTwo = connection->nodeTwo;

		if (nameOne == name) { return connection; }
		if (nameTwo == name) { return connection; }
	}

	return nullptr;
}

std::vector<Connection*> Simplex::Node::GetConnections()
{
	return myConnections;
}

std::vector<String> Simplex::Node::GetNeighborNames()
{
	std::vector<String> neighbors;

	for (Connection* connect : myConnections) {
		
		String nameOne = connect->nodeOne;
		String nameTwo = connect->nodeTwo;
		if (nameOne != name)
			neighbors.push_back(nameOne);
		if (nameTwo != name)
			neighbors.push_back(nameTwo);
	}
	return neighbors;
}

vector3 Simplex::Node::GetCoordinates()
{
	return m_coordinates;
}

void Simplex::Node::SetCoordinates(vector3 coordinates)
{
	m_coordinates = coordinates;
}
