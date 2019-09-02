#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <fstream>
#include <list>
#include <string>

#include "hash.hpp"
#include "heap.hpp"

class Graph {
	public:
		Graph (long const & capacity = 500000); // don't really need to initialize anything

		bool applyDijkstra (std::string const & start);
		bool hasNode (std::string const & node);
		bool insert (std::string const & v1, std::string const & v2, long long dist);

		bool exportFile (std::ofstream & out); // just traverse the linked list of nodes
		bool importFile (std::ifstream & in); // take in each line and process properly
	private:
		long capacity;
		struct Node; // we need to put this here so the edge struct is happy

		typedef struct Edge {
			long long cost;
			Node * dest;
		} Edge;

		typedef struct Node {
				std::string name; // name of node (this may not be necessary)
				std::list<Edge> adj; // linked list of directed edges for given node

				// these are for Dijkstra's algorithm:
				bool known;
				long long dist;
				std::list<std::string> path;
		} Node;

		std::list<Node *> nodeList;
		HashTable * graphIndex; // pointers to the node (in list of nodes)
};

#endif
