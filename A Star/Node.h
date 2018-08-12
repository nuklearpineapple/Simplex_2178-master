#pragma once

#include "Definitions.h"
#include "Connection.h"

namespace Simplex
{

	class Connection; // forward declaration

	class Node
	{
	public:

		uint fCost = 0; // distance to finish node + distance from current node
		uint distance; // distance from finish node
		String name; // name of node
		std::vector<Connection*> myConnections; 
		Node* parentNode;
		bool isFinal = false; // boolean for last node
		bool isStart = false; // boolean for first node

		// each node has its own unique location
		vector3 m_coordinates = ZERO_V3;

		Node();
		Node(String thisName, uint thisDistance);
		~Node();
		void AddConnection(Connection* connection);
		String GetName();
		void SetFinal(bool ind);
		void SetStart(bool ind);
		bool isFinalNode();
		Connection* GetConnectionToNeighbor(String name);
		std::vector<Connection*> GetConnections();
		std::vector<String> GetNeighborNames();
		vector3 GetCoordinates();
		void SetCoordinates(vector3 coordinates);
	};

}