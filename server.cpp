// Server program
#include "wdigraph.h"
// #include "dijkstra.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include "dijkstra.cpp"
using namespace std;

struct Point {
	long long lat;	// latitude of point
	long long lon;	// longitude of point
};

long long manhattan(const Point& pt1, const Point& pt2) {
	// Return the Manhattan distance between the two given points
	// dist = |x1-x2| + |y1-y1|
	// x = lat, y = lon

	long long dist = abs(pt1.lat - pt2.lat) + abs(pt1.lon - pt2.lon);
	return dist;
}

void readGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points) {
	/*
	Read the edmonton map data from the provided file and
	load it into the given WDigraph object.
	Store vertex coordinates in Points struct and map each
	vertex to its corresponding Point struct.

	PARAMETERS:
	filename: name of the file describing a road network
	graph: an instance of the weighted directed graph class
	points: a mapping between vertex identifiers and their
	coordinates (unordered map of ints and Point structs)

	*/
	// store vertex coordinates in a hash table (points) in order to 
	// display the least cost parth on the map
	// directed graph so do not add both edges

	// create ifstream object to open/read file
	ifstream file;
	// initialize string object line to store each line read
	string line;
	// open file
	file.open(filename);

	int commaPos, commaPos2, commaPos3;
	int ID, start, end;
	double latIn, lonIn;

	// from WE5
	if (file.is_open()) {
		// check if file opened correctly
		while (!file.eof()) {
			getline(file, line);
			// loop while there are more lines to read
			// read in a line
			// get type (V or E)
			string V = "V";
			string E = "E";
			// gets the type of input (V or E) from the line and stores as type
			string type = line.substr(0, 1);
			if (type.compare(V) == 0) {
				// type is V
				// find position of second comma
				commaPos = line.find(',', 2);
				commaPos2 = line.find(',', commaPos + 1);
				int len = line.length();

				// get ID as int
				ID = stoi(line.substr(2, commaPos - 2));
				latIn = stod(line.substr(commaPos + 1,commaPos2 - commaPos));
				lonIn = stod(line.substr(commaPos2 + 1, len - commaPos2));

				// store coords in 100000th degree
				long long newLat, newLon;
				newLat = static_cast<long long> (latIn*100000);
				newLon = static_cast<long long> (lonIn*100000);

				// create temporary Point struct to add to points map
				Point tempPoint;
				tempPoint.lat = newLat;
				tempPoint.lon = newLon;

				graph.addVertex(ID);

				// store lat and lon in points map for later use
				pair<int, Point> tempPair (ID, tempPoint);
				points.insert(tempPair);
			} else if (type.compare(E) == 0) {
				// type is E
				// find position of second comma
				commaPos = line.find(',', 2);
				// get first ID to connect
				start = stoi(line.substr(2, commaPos - 2));
				// get position of third comma
				commaPos2 = line.find(',', commaPos + 1);
				// get second ID to connect
				end = stoi(line.substr(commaPos + 1, commaPos2 - commaPos - 1));
				// add edge start to end only
				long long cost = manhattan(points.at(start), points.at(end));
				graph.addEdge(start, end, cost);
			}
		}
	}
	// close file
	file.close();
}

int main() {
	// call readGraph
	// call dijkstra

	WDigraph graph;
	unordered_map<int, Point> points;
	unordered_map<int, PIL> tree;
	// read and create the graph
	readGraph("edmonton-roads-2.0.1.txt", graph, points);

	dijkstra(graph, 29577354, tree);

	for (auto iter = tree.begin(); iter != tree.end(); ++iter) {
		cout << iter->first << ": " << iter->second.first << " time " << 
		iter->second.second << endl;
	}
    return 0;
}