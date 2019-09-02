# dijkstra

An implementation of a graph class that possesses inherent Dijkstra's algorithm
shortest path capabilities.

## Description

graph.cpp and graph.hpp implement a graph class that possesses as a function
Dijkstra's shortest path algorithm.  Graph nodes are implemented using
dynamic memory allocation and the custom hash table implementation.
Dijkstra's algorithm is implemented using the custom binary heap
implementation.  Adjacent nodes are represented as adjacency lists through
which Dijkstra's algorithm makes passes to greedily determine the shortest
path from one node to all other nodes in the graph.

The graph and Dijkstra's algorithm implementation are tested using a program
that takes in a graph file and outputs the shortest paths from a given node
to all other nodes in the graph (if there exists a path, with no negative cost
edges).


More details are given in the full problem set description within `doc/`.

## Dependencies

* g++ (with C++11 support)
* make

## Compilation

To build `dijkstra`, simply execute the following:

	make

## Usage

To run `dijkstra`, simply execute the following:

	make run

## Testing

Two files are provided for testing the example graph: `example.graph` and `example.test`.
To test the `dijkstra` utility, provide `example.graph` as the input graph.
Make the starting vertex `v1`.
Then, compare the output file `YOUR_OUTPUT_FILE` with `example.test` using `diff YOUR_OUTPUT_FILE example.test`.

## Graph File Structure

Graph files can be created using the following line structure:

```
VERTEX_X VERTEX_Y EDGE_COST
```

Each line creates a vertex `VERTEX_X` and a vertex `VERTEX_Y` (if they do not exist), as well as an edge of weight `EDGE_COST` among the two.  *Important: Dijkstra's algorithm does not accept negative edge costs.*
