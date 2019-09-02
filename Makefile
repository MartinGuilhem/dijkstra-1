.PHONY: all clean debug run

all: dijkstra

dijkstra: dijkstra.o hash.o heap.o graph.o
	@echo "Building 'dijkstra'..."
	@g++ -std=c++11 dijkstra.o hash.o heap.o graph.o -o dijkstra

dijkstra.o: src/dijkstra.cpp
	@echo "Building 'dijkstra.o'..."
	@g++ -std=c++11 -c src/dijkstra.cpp

hash.o: src/hash.cpp src/hash.hpp
	@echo "Building 'hash.o'..."
	@g++ -std=c++11 -c src/hash.cpp src/hash.hpp

heap.o: src/heap.cpp src/heap.hpp
	@echo "Building 'heap.o'..."
	@g++ -std=c++11 -c src/heap.cpp src/heap.hpp

graph.o: src/graph.cpp src/graph.hpp
	@echo "Building 'graph.o'..."
	@g++ -std=c++11 -c src/graph.cpp src/graph.hpp

clean:
	@echo "Cleaning all built files..."
	@rm -f src/*.gch *.o dijkstra

debug:
	@echo "Building dijkstra with debugging capabilities..."
	@g++ -std=c++11 -g src/dijkstra.cpp src/hash.cpp src/heap.cpp -o dijkstra

run: dijkstra
	@./dijkstra
