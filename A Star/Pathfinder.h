#pragma once

#include "Definitions.h"
#include "Node.h"

namespace Simplex
{

	class Node; // forward declaration

	class Pathfinder
	{
	public:

		uint maxFCost = 999;
		std::vector<Node*> initialNodes; // nodes that will be searched for best path
		std::vector<Node*> workingPath; // store of nodes that are possible
		std::vector<Node*> finalPath; // store of nodes in best path

		Pathfinder();
		~Pathfinder();
		void Run();
		std::vector<Node*> FindPath(Node* startNode);
		Node* FindNode(String name);
		Node* FindStartNode();
		Node* FindLowestCostNode();
		void RemoveNodeFromWorking(Node* currentNode);
		bool isInWorking(Node* node);
		bool isInFinal(Node* node);
		void Initialize();
	};

}