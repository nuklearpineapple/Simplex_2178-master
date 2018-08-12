#include "Pathfinder.h"

using namespace Simplex;

Simplex::Pathfinder::Pathfinder()
{
}

Simplex::Pathfinder::~Pathfinder()
{
}

void Simplex::Pathfinder::Run()
{
	Initialize();
	Node* startNode = FindStartNode();
	std::vector<Node*> path = FindPath(startNode);
}

std::vector<Node*> Simplex::Pathfinder::FindPath(Node* startNode)
{
	// add start node to our working path
	workingPath.push_back(startNode);

	bool foundFinal = false;

	while(!foundFinal){
		Node* currentNode = FindLowestCostNode();
		//remove current from workingPath
		RemoveNodeFromWorking(currentNode);
		//push back current to finalPath (in this example, final path becomes S,A,C,F)
		finalPath.push_back(currentNode);

		//if current is the target node, path has been found
		if (currentNode->isFinalNode()) {
			foundFinal = true;
			break;
		}

		uint workingCost = maxFCost;

		std::vector<String> neighbors = currentNode->GetNeighborNames();

		//foreach connection of the current node
		for (String neighborName : neighbors) {
			Node* neighbor = FindNode(neighborName);
		
			if (isInFinal(neighbor)) {
				//skip to the next neighbor
				continue;
			}

			//neighbor cost = calculate f cost of neighbor
			Connection* connection = currentNode->GetConnectionToNeighbor(neighborName);
			uint connectionCost = connection->myCost;
			uint calcCost = connectionCost + neighbor->distance;

			//if(neighbor cost is less than working cost OR neighbor is not in workingPath)
			if (neighbor->fCost < workingCost || !(isInWorking(neighbor))) {
				neighbor->fCost = calcCost;
				neighbor->parentNode = currentNode;

				//if(neighbor cost is less than working cost)
				if (neighbor->fCost < workingCost) {
					workingCost = neighbor->fCost;
				}

				if (!(isInWorking(neighbor))) {
					workingPath.push_back(neighbor);
				}
			}
		}
	}

	return finalPath; 
}

Node * Simplex::Pathfinder::FindNode(String name)
{
	for (Node* node : initialNodes) {
		if (node->name == name) {
			return node;
		}
	}
	return nullptr;
}

Node * Simplex::Pathfinder::FindStartNode()
{
	for (Node* node : initialNodes) {
		if (node->isStart)
			return node;
	}
	return nullptr;
}

Node * Simplex::Pathfinder::FindLowestCostNode()
{
	uint lowestCost = maxFCost;
	Node* lowestNode = nullptr;

	for (Node* node : workingPath) {
		uint cost = node->fCost;
		if (cost < lowestCost) {
			lowestCost = cost;
			lowestNode = node;
		}
	}
	return lowestNode;
}

void Simplex::Pathfinder::RemoveNodeFromWorking(Node * currentNode)
{
	for (uint i = 0; i < workingPath.size(); i++) {
		Node* node = workingPath[i];
		if (node->GetName() == currentNode->GetName()) {
			workingPath.erase(workingPath.begin() + i);
			return;
		}
	}

}

bool Simplex::Pathfinder::isInWorking(Node * node)
{
	for (Node* pathNode : workingPath) {
		if (pathNode->GetName() == node->GetName()) {
			return true;
		}
	}
	return false;
}

bool Simplex::Pathfinder::isInFinal(Node * node)
{
	for (Node* pathNode : finalPath) {
		if (pathNode->GetName() == node->GetName()) {
			return true;
		}
	}
	return false;
}


void Simplex::Pathfinder::Initialize()
{
	// load all my starting nodes : nodes are named S, A, B, C, D, G, F

	// S node
	Node* nd = new Node("s", 29);
	nd->SetCoordinates(vector3(ZERO_V3));
	nd->SetStart(true);
	Connection* aConnection = new Connection("s", "a", 10);
	nd->AddConnection(aConnection);

	aConnection = new Connection("s", "b", 15);
	nd->AddConnection(aConnection);

	initialNodes.push_back(nd);

	// A node
	nd = new Node("a", 19);
	nd->SetCoordinates(vector3(2.0f,0.0f,0.0f));
	aConnection = new Connection("a", "c", 7);
	nd->AddConnection(aConnection);

	aConnection = new Connection("a", "g", 5);
	nd->AddConnection(aConnection);

	aConnection = new Connection("a", "s", 10);
	nd->AddConnection(aConnection);

	initialNodes.push_back(nd);

	// B node
	nd = new Node("b", 15);
	nd->SetCoordinates(vector3(1.0f,3.5f,0.0f));
	aConnection = new Connection("b", "c", 3);
	nd->AddConnection(aConnection);

	aConnection = new Connection("b", "d", 15);
	nd->AddConnection(aConnection);

	aConnection = new Connection("b", "g", 8);
	nd->AddConnection(aConnection);

	initialNodes.push_back(nd);

	// C node
	nd = new Node("c", 12);
	nd->SetCoordinates(vector3(2.5f,2.0f,0.0f));
	aConnection = new Connection("c", "a", 7);
	nd->AddConnection(aConnection);

	aConnection = new Connection("c", "g", 5);
	nd->AddConnection(aConnection);

	aConnection = new Connection("c", "b", 3);
	nd->AddConnection(aConnection);

	aConnection = new Connection("c", "f", 12);
	nd->AddConnection(aConnection);

	initialNodes.push_back(nd);

	// D node
	nd = new Node("d", 30);
	nd->SetCoordinates(vector3(1.0f,6.5f,0.0f));
	aConnection = new Connection("d", "b", 15);
	nd->AddConnection(aConnection);

	aConnection = new Connection("d", "f", 30);
	nd->AddConnection(aConnection);

	initialNodes.push_back(nd);

	// G node
	nd = new Node("g", 17);
	nd->SetCoordinates(vector3(1.5f,1.0f,0.0f));
	aConnection = new Connection("g", "b", 8);
	nd->AddConnection(aConnection);

	aConnection = new Connection("g", "a", 5);
	nd->AddConnection(aConnection);

	aConnection = new Connection("g", "c", 5);
	nd->AddConnection(aConnection);

	initialNodes.push_back(nd);

	// F node
	nd = new Node("f", 0);
	nd->SetCoordinates(vector3(3.5f,4.0f,0.0f));
	nd->SetFinal(true);
	aConnection = new Connection("f", "d", 30);
	nd->AddConnection(aConnection);

	aConnection = new Connection("f", "c", 12);
	nd->AddConnection(aConnection);

	initialNodes.push_back(nd);
}
