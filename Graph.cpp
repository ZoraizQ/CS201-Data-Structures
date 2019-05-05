#ifndef GRAPH__CPP
#define GRAPH__CPP
#include "Graph.h"

Graph::Graph(char* file, bool isUnitLength=false)
{
	//initialise the nodes and edges in the constructor after reading them from the file.
	//file's name is stored in file (char array)
	ifstream inFile;
	inFile.open(file);
		
	cout << "1. Parsing the dataset people and connections from file \'" << file << "\'." << endl;
	if (inFile.is_open())
	{
		string currPerson;
		inFile >> currPerson; //reads the "PEOPLE" header
		while (inFile >> currPerson) //every line has a person, read the next person
		{
			if (currPerson == "CONNECTIONS") // when connections header encountered, break this loop. 
				break;
			//cout << "Person found: " << currPerson << endl;
			node* newPerson = new node();
			newPerson->name = currPerson;
			newPerson->visit = 0;
			//vector edge will be clear for a new node formed
			people.push_back(newPerson);
		}
		
		cout << "Printing all node names found:\n";
		for (int i = 0; i < people.size(); i++){
			cout << people[i]->name << " ";
		}
		cout << endl;
		
		string conn1, conn2, weightStr;
		while (inFile >> conn1) //every line has a connection now, read the next connection
		{
			inFile >> conn2;
			inFile >> weightStr;
			stringstream converter;
			converter << weightStr;
			float weight;
			converter >> weight;
			
			if (isUnitLength == true){
				weight = 1;
			}
			//cout << "Connection found between " << conn1 << " and " << conn2 << " of weight " << weight << endl;

			edge newConn;
			
			for (int i = 0; i < people.size(); i++){
				if (people[i]->name == conn1)
					newConn.Origin = people[i];
				if (people[i]->name == conn2)
					newConn.Dest = people[i];
			}
			newConn.weight = weight;

			newConn.Origin->edges.push_back(newConn);
			edge reverseCopy; // reverse direction copy created, origin, dest flipped
			reverseCopy.Origin = newConn.Dest;
			reverseCopy.Dest = newConn.Origin;
			reverseCopy.weight = newConn.weight;
			newConn.Dest->edges.push_back(reverseCopy); //reverse copy pushed back into destination node's edges vect
			
			connections.push_back(newConn);
		}

		cout << endl;
		inFile.close();
	}
	
	cout << "Printing all connections details found:\n";
	for (int i = 0; i < connections.size(); i++){
		cout << "Connection found between " << connections[i].Origin->name << " and " << connections[i].Dest->name << " of weight " << connections[i].weight << endl;
	}

	/*for (int i = 0; i < people.size(); i++){
		for (int j = 0; j < people[i]->edges.size(); j++){
			cout << people[i]->name << " has an edge to " << people[i]->edges[j].Dest->name << endl;
		}
	}*/
}

void Graph::display(node* temp)
{
	if(temp->visit)
		return;
	
	cout << "Person: " << temp->name;
	temp->visit = 1;
	for(int i=0; i< temp->edges.size(); i++)
	{
		if(!temp->edges[i].Origin->visit || !temp->edges[i].Dest->visit)
		cout << "->" << temp->edges[i].weight << endl;
		display(temp->edges[i].Origin);
		display(temp->edges[i].Dest);
	}
}

void printStrVector(vector<string> v){
	for (int i = 0; i < v.size(); i++){
		cout << v[i];
		if (i != v.size()-1)
			cout <<" -> ";
	}
	cout << endl;
}

node* getNodeByName(vector<node*> nodesList, string name){
	for (int i = 0; i < nodesList.size(); i++){
		if (nodesList[i]->name == name){
			return nodesList[i];
		}
	}
	return NULL;
}

void resetAllVisited(vector<node*> nodesList){
	for (int i = 0; i < nodesList.size(); i++){
		nodesList[i]->visit = 0;
	}
}


bool Graph::Reachable(string start, string dest)
{
	resetAllVisited(people); // reset all visit attribute of every node in given node list to 0
	node* destNode = getNodeByName(people, dest); // get node from nodelist by given string name
	if (destNode == NULL){
		cout << "No such node exists for \'" << dest << "\'." << endl;
		return false;
	}

	node* startNode = getNodeByName(people, start); // get node from nodelist by given string name
	if (startNode == NULL){
		cout << "No such node exists for \'" << start << "\'." << endl;
		return false;
	}

	queue <node*> bfsQ;
	startNode->visit = 1; //the initial node is marked as visited
	bfsQ.push(startNode); // and pushed into the BFS queue


	while(bfsQ.size() != 0){
		node* front = bfsQ.front();
		if (front->name == dest){ // destination node reached
			return true;
			break;
		}
		//cout << front->name << '\t';
		bfsQ.pop();

		for (int i = 0; i < front->edges.size(); i++){
			if (front->edges[i].Origin->name == front->name){ // if the current front has any connected edges (Origin node == front node by name)
				//if not already visited, add edge's dest node to queue after it is marked as visited
				node* pushNode = front->edges[i].Dest; // pushNode == DEST
				if (pushNode->visit == 0){
					pushNode->visit = 1;
					bfsQ.push(pushNode);
				}
			}
		}
	}

	return false;
}

edge getMinUnvisEdge(vector<edge> edgeList){
	float minWeight = INT32_MAX;
	edge minEdge;
	minEdge.weight = -1; // for NULL checks
	for(int i = 0; i < edgeList.size(); i++){
		if (edgeList[i].Dest->visit == 0 && edgeList[i].weight < minWeight){
			minWeight = edgeList[i].weight;
			minEdge = edgeList[i];
		}
	}
	return minEdge;
}

void resetAllDistancesINF(vector<node*> nodesList){
	for (int i = 0; i < nodesList.size(); i++){
		nodesList[i]->distance = INT32_MAX; // changed in memory, so pass by value of the nodesList was enough
		nodesList[i]->p = NULL; // also resets all prev ptrs
	}
}


node* getMinDistanceNode(vector<node*> nodesList){
	float minDistance = INT32_MAX;
	node* minDistNode;
	minDistNode->distance = -1; //NULL check, possible to be returned when all visited
	for (int i = 0; i < nodesList.size(); i++){
		node* currNode = nodesList[i];
		if (currNode->distance < minDistance && currNode->visit == 0){
			minDistance = currNode->distance;
			minDistNode = currNode;
		}
	}
	return minDistNode;
}

float getWeightBetween(vector<edge> connections, string n1, string n2){
	for (int i = 0; i < connections.size(); i++){
		//if origin == n1 and dest == n2 OR origin == n2 and dest == n1, since undirected
		string oName = connections[i].Origin->name;
		string dName = connections[i].Dest->name;
		if ((oName == n1 && dName == n2) || (oName == n2 && dName == n1)){ // there is no reverseCopy in connections
			return connections[i].weight;
		}
	}
	return 0; // otherwise 0 returned if no such edge found
}

int getPathSum(vector<edge> connections, vector<string> shortestPath){
	int pathSum = 0;
	for (int i = 0; i < shortestPath.size()-1; i++){ //print the path sum as well, by adding all the weights of the edges on the shortest path calculated
		pathSum += getWeightBetween(connections, shortestPath[i],shortestPath[i+1]);
	}
	return pathSum;
}

struct Comparator
{
	bool operator()(const node* a, const node* b) const
	{
		return a->distance > b->distance;
	}
};

void showpq(priority_queue <node*, vector<node*>, Comparator> gq) 
{ 
    priority_queue <node*, vector<node*>, Comparator> g = gq; 
    while (!g.empty()) 
    { 
        cout << '\t' << g.top()->name << " " << g.top()->distance; 
        g.pop(); 
    } 
    cout << '\n'; 
} 

vector<node*> Graph::Prims()
{
	resetAllVisited(people);
	resetAllDistancesINF(people);
	vector<node*> tree;
	priority_queue <node*, vector<node*>, Comparator> minq;
	node* start = getMinUnvisEdge(connections).Origin;
	start->distance = 0;
	for (int i = 0; i < people.size(); i++){
		minq.push(people[i]);
	} //heap ready
	
	while (!minq.empty()){
		node* topNode = minq.top();
		tree.push_back(topNode);
		topNode->visit = 1; // make visited (added to minq)
		minq.pop();
		for (int i = 0; i < topNode->edges.size(); i++){
			node* connNode = topNode->edges[i].Dest; //connected destination node by this edge
			float currCost = topNode->edges[i].weight; // current cost of this edge (from node to connection)
			if (connNode->visit == 0 && currCost < connNode->distance){
				connNode->p = topNode; // assign predecessor
				connNode->distance = currCost; // fix cost 
				vector<node*> copier;
				while (!minq.empty()) 
			    { 
			        copier.push_back(minq.top());
			        minq.pop();
			    } 
			    for (int c= 0; c < copier.size(); c++){
			    	minq.push(copier[c]);
			    }
			    
			}
		}
	}

	resetAllVisited(people);

	
	for (int i = 0; i < tree.size(); i++){
		cout << tree[i]->name << " ";
	}
	cout << endl;
}

vector<node*> Graph::Kruskal()
{

}



vector<string> Graph::Dijkstra(string start, string dest, float &d)
{
	if (!Reachable(start,dest)){
		vector<string> empt;
		return empt;
	}

	vector<string> shortestPath; // empty init
	set<string> checked;
	// initialise all distance values as INT_MAX (+inf)
	resetAllVisited(people);
	resetAllDistancesINF(people);

	node* srcNode = getNodeByName(people, start);
	srcNode->distance = 0; // source node has distance value 0 since we're already here (start)

	for (int x = 0; x < people.size(); x++){ // all are unknown
		node* mdNode = getMinDistanceNode(people);
		if (mdNode->distance == -1){ //out of unvisited
			break;
		}
		//cout << "Minimum distance node: " << mdNode->name << endl;
		mdNode->visit = 1;

		checked.insert(mdNode->name);
		//for every connected/adjacent node, update their distance values
		for (int i = 0; i < mdNode->edges.size(); i++){
			node* an = mdNode->edges[i].Dest; //current adjacent node
			an->p = mdNode; //update previous pointer to mdNode
			cout << an->name << "'s prev " << an->p->name << endl;
			
			if (an->name == dest){
				vector<string> reversePath;
				node* DN = getNodeByName(people,dest);
				while (DN->name != start){
					reversePath.push_back(DN->name);
					DN = DN->p;	
				}
				reversePath.push_back(DN->name);
					
				for (int i = reversePath.size()-1; i >= 0;i--){
					shortestPath.push_back(reversePath[i]);
				}

				cout << "Path Sum: " << getPathSum(connections, shortestPath) << endl;
				return shortestPath;
			}
			
			if (an->visit == 0){ 
				float sumD = mdNode->distance + mdNode->edges[i].weight; //get total distance up to this node with the addition of the edge weight to this node
				if (sumD < an->distance){ // if this new path has a distance less than current distance value of this node
					an->distance = sumD; //then update distance value to this sum
				}
			}
		}
		
	}

	
	vector<string> reversePath;
	node* DN = getNodeByName(people,dest);
	while (DN->name != start){
		reversePath.push_back(DN->name);
		DN = DN->p;	
	}
	reversePath.push_back(DN->name);
		
	for (int i = reversePath.size()-1; i >= 0;i--){
		shortestPath.push_back(reversePath[i]);
	}

	cout << "Path Sum: " << getPathSum(connections, shortestPath) << endl;
	
	return shortestPath; //empty vector object returned
}


int main()
{
	//cout<<"Initialize your graph in main!"<<endl;
	char friendsFile[] = "friends.txt";
	bool unitWeight = false;
	Graph G(friendsFile, unitWeight);
	cout << "\nInitialized graph." << endl;
	// write code for part 6 here
	cout << "Reachability for Mannan, Ali: " << G.Reachable("Mannan","Ali") << endl;
	cout << "Reachability for Mannan, Trump: " << G.Reachable("Mannan","Trump") << endl;
	cout << "Reachability for Ali, Trump: " << G.Reachable("Ali","Trump") << endl;
	//All reachability tests give true as a result, so this subgraph is cyclic, and complete connected.

	//Dijkstra: For a graph with unit length weights, how many hops is Ali away from Trump?
	float f = 0.0;
	string a = "Ali", b = "Trump";
	cout << "Printing shortest path by Dijkstra from " << a << " to " << b << ": "<< endl;
	vector<string> v = G.Dijkstra(a,b,f);
	printStrVector(v);
	cout << a << " is " << v.size()-1 << " hops away from " << b; 
	cout << endl << endl;

	//What about Mannan and Trump?
	a = "Trump"; b = "Mannan";
	cout << "Printing shortest path by Dijkstra from " << a << " to " << b << ": "<< endl;
	v = G.Dijkstra(a,b,f);
	if (v.empty()){
		cout << "EMPTY shortestPath vector." << endl;
	}
	else{
	printStrVector(v);
	cout << a << " is " << v.size()-1 << " hops away from " << b;
	}
	cout << endl;	

	//Who has smaller value of path in terms of index of friendship?
	if (!unitWeight)
		cout << "As seen from the printed shortest path sums, Mannan has the smaller value of\npath when isUnitLenght == false, and so a higher index of friendship with Trump." << endl;
	

	G.Prims();
	/*
	Run the MST on both unit weight graph and weighted graph. Could there
	exist more than one MST for one of the graphs? What implications can you
	draw from result of both runs? What benefit can a social media website
	have from the MSTs you have produced? Can you think of other
	applications of MST in terms of social connection graphs?
	*/
}

#endif
