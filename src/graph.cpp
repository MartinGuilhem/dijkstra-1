#include <climits>
#include <iterator>
#include "graph.hpp"

Graph::Graph (long const & capacity) {
	this->graphIndex = new HashTable (capacity * 2);
	this->capacity = 0;
}

bool Graph::applyDijkstra (std::string const & begin) {
	int i;
	Node * start = (Node *) this->graphIndex->getPointer (begin);
	start->dist = 0; // then look at all adjacent stuff
	start->path.push_back (begin);
	
	BinaryHeap helper (this->capacity);
	helper.insert (begin, start->dist, start); // insert source vertex first
	for (std::list<Node *>::const_iterator it = this->nodeList.begin (), end = this->nodeList.end (); it != end; ++it) {
		if ((*it)->name != begin) {
			helper.insert ((*it)->name, (*it)->dist, *it);
		}
	} // the binary heap is now initialized

	Node * temp;

	for (i = 0; i < this->capacity; ++i) {
		helper.deleteMin (nullptr, nullptr, &temp);
		for (std::list<Edge>::const_iterator it = temp->adj.begin (), end = temp->adj.end (); it != end && temp->dist != LLONG_MAX; ++it) {
			if (!it->dest->known && it->dest->dist > (it->cost + temp->dist)) {
				// update the path leading to the node
				it->dest->path.clear ();
				it->dest->path.insert (it->dest->path.begin (), temp->path.begin (), temp->path.end ());
				it->dest->path.push_back (it->dest->name);

				// update the node distance and key in the priority queue
				it->dest->dist = (it->cost + temp->dist);
				helper.setKey (it->dest->name, (it->cost + temp->dist));
			}
		}
		temp->known = true;
	}
	
	return true;
}

bool Graph::hasNode (std::string const & node) {
	return this->graphIndex->contains (node);
}

bool Graph::insert (std::string const & v1, std::string const & v2, long long dist) {
	Node * temp1, * temp2;
	Edge e;

	if (!this->graphIndex->contains (v1)) {
		temp1 = new Node; // allocate memory for new node
		temp1->name = v1;
		temp1->known = false;
		temp1->dist = LLONG_MAX; // treat as infinity
		this->nodeList.push_back (temp1); // insert node at end of nodeList
		this->graphIndex->insert (v1, temp1); // insert node into graphIndex hash table
		++this->capacity;
	} else { // it's already in the graph, so let's get the pointer
		temp1 = (Node *) this->graphIndex->getPointer (v1);
	}

	if (!this->graphIndex->contains (v2)) {
		temp2 = new Node; // allocate memory for new node
		temp2->name = v2;
		temp2->known = false;
		temp2->dist = LLONG_MAX; // treat as infinity
		this->nodeList.push_back (temp2); // insert node at end of nodeList
		this->graphIndex->insert (v2, temp2); // insert node into graphIndex hash table
		++this->capacity;
	} else { // it's already in the graph, so let's get the pointer
		temp2 = (Node *) this->graphIndex->getPointer (v2);
	}

	e.cost = dist;
	e.dest = temp2;
	temp1->adj.push_back (e); // put edge in adjacency list

	return true;
}

bool Graph::exportFile (std::ofstream & out) {
	// assume file to be opened and ready to go
	// just traverse the nodeList
	// print out the node name

	for (std::list<Node *>::const_iterator it = this->nodeList.begin (), end = this->nodeList.end (); it != end; ++it) {
		out << (*it)->name << ": ";

		if ((*it)->dist == LLONG_MAX) {
			out << "NO PATH" << std::endl;
		} else {
			out << (*it)->dist << " [";
			for (std::list<std::string>::const_iterator ju = (*it)->path.begin(), foe = (*it)->path.end (); ju != foe; ++ju) {
				out << *ju;
				if (std::next (ju, 1) != foe) out << ", "; // only print commas if not final node
			}
			out << "]" << std::endl;
		}
	}
	
	return true;
}

bool Graph::importFile (std::ifstream & in) {
	std::string v1, v2;
	long dist;

	// assume file to be opened and ready to go
	while (in >> v1 >> v2 >> dist) {
		this->insert (v1, v2, dist);
	}

	return true;
}
