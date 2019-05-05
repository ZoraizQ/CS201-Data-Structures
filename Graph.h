#ifndef GRAPH__H
#define GRAPH__H
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <set>
#include <algorithm>
#include <queue>
#include <sstream>
// You may include more libraries.

using namespace std;
class node;

class edge //E -> (Origin, Dest), weight
{
	// You may add more variables and functions in this class
	public:
		node* Origin; // start node
		node* Dest; // end node
		float weight; // weight == 1 if isUnitLenght is true, weight == from file if false 
		bool used;
};
class node //N -> (Name, Edge vector), visit
{
	// You may add more variables and functions in this class
	public:
		int visit; // flag to check if the node has been already visited
		string name; // of person being represented
		vector<edge> edges; // edges connected to node
		int distance;
		node* p; // previous pointer
		int ID;

};


class Graph
{
	public:
		vector<node*> Mst; // minimum spanning tree
		vector<node*> people;
		vector<edge> connections;
		Graph(char* filename, bool isUnitLenght);
		void display(node* temp);// displays the graph

		bool Reachable(string start, string dest);
		vector<node*> Prims();
		vector<node*> Kruskal(); //runs kruskal algorithm and returns the Minimum spanning tree
		vector<string> Dijkstra(string start, string dest, float &d);
};

#endif

