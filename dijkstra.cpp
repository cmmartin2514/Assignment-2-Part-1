// implementation of Dijkstra's algorithm using binary heap
#include <cmath>
#include <algorithm>
#include <utility>
#include "heap.h"
#include "dijkstra.h"
#include "wdigraph.h"

void dijkstra(const WDigraph& graph, int startVertex,
	unordered_map<int, PIL>& tree) {
	/*
	Compute least cost paths that start from a given vertex
	Use a binary heap to efficiently retrieve an unexplored vertex
	that has the minimum distance from the start vertex at every iteration
	PIL is an alias for "pair<int, long long>" type

	PARAMETERS:
	WDigraph: an instance of WDigraph class
	startVertex: a vertex in this graph which serves as the root of the search tree
	tree: a search tree used to construct the least cost path to some vertex
	*/

	// should be O(mlogm) time where m is number of edges in graph
	// keys are predecessor vertices and times (int, long long)
	// items are ID nums (int)

	BinaryHeap<int, PIL> events;
	long long time;
	int u, v;
	// int is the vertex to be found
	// PIL is <int, long long> where int is in format (u, d)
	// PIPIL is <int, <int, long long>> in format (v, (u, d))
	// fire burns from u to v in d time

	events.insert(startVertex, PIL(-1,0));
	// starting at startVertex, no predecessor (-1) and time 0

	while (events.size() > 0) {
		// heap stores all neighbours of current vertex and time to each
		HeapItem<int, PIL> currMin;
		currMin = events.min();
		events.popMin();

		// currMin.item is the current ID, = v
		// currMin.key is the time, = time
		// u is the predecessor
		v = currMin.item;
		u = currMin.key.first;
		time = currMin.key.second;
		if (tree.find(v) == tree.end()) {
			// if v is not reached yet
			tree[v] = PIL(u, time);

			for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
            	int nbr = *iter;

            	// the fire starts at v at time d and will reach nbr
	            // at time d + (length of v->nbr edge)
	            int burn = time + graph.getCost(v, nbr);
	            events.insert(nbr, PIL(v, burn));
            }
		}
	}

}