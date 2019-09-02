#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include "graph.hpp"

using namespace std;
using namespace std::chrono;

int main (int argc, char ** argv) {
	string graphPath, vStart, outPath;
	ifstream inFile;
	ofstream outFile;
	Graph g;

	high_resolution_clock::time_point t1, t2;
	duration<double> diff;

	while (!inFile.is_open ()) {
		cout << "Enter a valid graph input file: ";
		cin >> graphPath;
		inFile.open (graphPath, ifstream::in);
	}

	g.importFile (inFile); // read in the graph input file
	inFile.close (); // close the input file
	
	while (!g.hasNode (vStart)) {
		cout << "Enter a valid starting vertex: ";
		cin >> vStart;
	}

	t1 = high_resolution_clock::now ();
	g.applyDijkstra (vStart); // apply Dijkstra's algorithm
	t2 = high_resolution_clock::now ();
	diff = duration_cast<duration<double>> (t2 - t1);
	
	cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << diff.count () << endl;

	while (!outFile.is_open ()) {
		cout << "Enter an output file: ";
		cin >> outPath;
		outFile.open (outPath, ofstream::out | ofstream::trunc);
	}

	g.exportFile (outFile); // output the shortest paths of all vertices
	outFile.close (); // close the output file

	return 0;
}
