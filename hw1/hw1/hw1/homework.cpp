
#include<fstream>
#include<string>
#include<iostream>
#include <list>
#include<vector>
#include<algorithm>

#define INFINITY 32767   // max value of int
using namespace std;

struct Node_data{   //store vertice's state and f-value
	string state;
	int h_cost;
	int g_cost;   //distance from node v to goal
	int parent;
};
struct Arc_data{
	int value;
	int order;
};
//global variables;
ofstream output("output.txt");
string algo;   // name of algorithm

class Graph
{
	int V;    // No. of vertices
	Node_data *nodes;  //set of vertex
	Arc_data **arcs;  //2d set of edges, store g-value
public:
	Graph(int V, Node_data *nodes, Arc_data **arcs);  // Constructor
	int LocateString(string s);   // replace string by using thier location in vertex's set.
	void addEdge(int v, int w, int g);  // add an edge to graph
	int FirstVex(int s);  // find the first adjacent vertice of s
	int NextVex(int i, int j);   // find the next vertex of j, which j is the i's adjacent vertice
};

Graph::Graph(int V, Node_data *nodes, Arc_data **arcs)
{
	this->V = V;
	this->nodes = nodes;
	this->arcs = arcs;
}

int Graph::LocateString(string s){
	for(int i = 0; i < V; ++i){
		if(s == nodes[i].state)
			return i;
	}
	return -1;
}

void Graph::addEdge(int v, int w, int g)
{
	arcs[v][w].value = g;
}

int Graph::FirstVex(int s){
	for(int i = 0; i < V; ++i)
		if (arcs[s][i].value != INFINITY)
			return i;
	return -1;
}

int Graph::NextVex(int i, int j){
	for(int k = j + 1; k < V; ++k )
		if(arcs[i][k].value != INFINITY)
			return k;
	return -1;
}

class Search{
	int V;    
	Node_data *nodes;
	Arc_data **arcs;
	int startNum;
	int goalNum;
	list<int> open, closed;  // open:frontier queue, for BFS, UCS; closed-for all algorithm
	list<int> time;  //time queue
	int *f_cost;
	void UCSUtil(int v, int w, bool *visited);   //only used by UCS
	void AStarUtil(int v, int w, bool *visited);   //only used by AStar
	void Solution(int w, bool *visited);   // print out the solution
public:
	Search(int V, Node_data *nodes, Arc_data **arcs, int startNum, int goalNum);
	bool BFS(Graph g);	
	bool DFS(Graph g);
	bool UCS(Graph g);
	bool AStar(Graph g);
};
Search::Search(int V, Node_data *nodes, Arc_data **arcs, int startNum, int goalNum){
	this->V = V;
	this->nodes = nodes;
	this->arcs = arcs;
	this->startNum = startNum;
	this->goalNum = goalNum;
}

void Search::UCSUtil(int v, int w, bool *visited){	
	list<int>::iterator i = find(open.begin(), open.end(), w); 
	if((i == open.end()) && (!visited[w])){   // w is not in the open list, closed[w] = false;
		open.push_back(w);
		nodes[w].g_cost = nodes[v].g_cost + arcs[v][w].value;
		nodes[w].parent = v;
	}
	else if ((i != open.end()) && (!visited[w])){
		if((nodes[v].g_cost + arcs[v][w].value) < nodes[w].g_cost){
			nodes[w].g_cost = nodes[v].g_cost+ arcs[v][w].value;
			nodes[w].parent = v;
		}
	}
	else if ((i == open.end()) && visited[w]){
		if((nodes[v].g_cost + arcs[v][w].value) < nodes[w].g_cost){
			visited[w] = false;
			list<int>::iterator j = find(closed.begin(), closed.end(), w);
			closed.erase(j);
			open.push_back(w);
			nodes[w].g_cost = nodes[v].g_cost + arcs[v][w].value;
			nodes[w].parent = v;
		}
	}
}

void Search::AStarUtil(int v, int w, bool *visited){
	list<int>::iterator i = find(open.begin(), open.end(), w); 
	if((i == open.end()) && (!visited[w])){   // w is not in the open list, closed[w] = false;
		open.push_back(w);
		nodes[w].g_cost = nodes[v].g_cost+ arcs[v][w].value;
		f_cost[w] = nodes[w].h_cost + nodes[w].g_cost;
		nodes[w].parent = v;
	}
	else if ((i != open.end()) && (!visited[w])){
		if((nodes[w].h_cost + nodes[v].g_cost + arcs[v][w].value) < f_cost[w]){
			nodes[w].g_cost = nodes[v].g_cost+ arcs[v][w].value;
			f_cost[w] = nodes[w].h_cost + nodes[w].g_cost;
			nodes[w].parent = v;
		}
	}
	else if ((i == open.end()) && visited[w]){
		if((nodes[w].h_cost + nodes[v].g_cost + arcs[v][w].value) < f_cost[w]){
			visited[w] = false;
			list<int>::iterator j = find(closed.begin(), closed.end(), w);
			closed.erase(j);
			open.push_back(w);
			nodes[w].g_cost = nodes[v].g_cost+ arcs[v][w].value;
			f_cost[w] = nodes[w].h_cost + nodes[w].g_cost;
			nodes[w].parent = v;
		}
	}
}
void Search::Solution(int w, bool *visited){
	int temp = INFINITY;
	int size = closed.size();
	list<string> solution;
	solution.push_front(nodes[w].state);
	if(algo == "BFS" || algo == "DFS"){
		int accTime = 0;
		time.push_back(accTime);
		//ensure tiebreak
		while(!closed.empty()){
			int i = closed.back();
			if(nodes[w].parent == i){
				solution.push_front(nodes[i].state);
				++accTime;
				time.push_back(accTime);
				w = i;
			}
			closed.pop_back();
		}
	}
	else if(algo == "UCS" || algo == "A*"){
		time.push_front(nodes[w].g_cost);
		closed.pop_back();
		while(!closed.empty()){
			int i = closed.back();
			if(nodes[w].parent == i){
				int i = nodes[w].parent;
				solution.push_front(nodes[i].state);
				time.push_front(nodes[i].g_cost);
				w = i;
			}
			closed.pop_back();
		}
	}
	while (!solution.empty() && !time.empty()){
		output << solution.front() << " " << time.front() << endl;
		solution.pop_front();
		time.pop_front();
	}
}

bool Search::BFS(Graph g)
{
	if (startNum == goalNum){
		output << nodes[startNum].state << " 0" << endl;
		return true;
	}
	bool *visited = new bool[V];
	for(int i = 0; i < V; ++i)
		visited[i] = false;
	visited[startNum] = true;
	open.push_back(startNum);

	int *order_temp = new int[V];
	for(int i = 0; i < V; ++i)
		order_temp[i] = INFINITY;
	int ord;
	while(!open.empty())
	{
		ord = 0;
		startNum = open.front();
		open.pop_front();
		closed.push_back(startNum);
		//ensure enqueue nodes with the live traffic's order.
		for(int w = g.FirstVex(startNum); w >= 0 ; w = g.NextVex(startNum, w)){
			order_temp[ord] = w;
			++ord;
		}
		for(int i = 0; i < V-1; ++i){
			for(int j = i+1; j < V; ++j){
				if(order_temp[i] != INFINITY && order_temp[j] != INFINITY){
					int v = order_temp[i];
					int w = order_temp[j];
					if(arcs[startNum][v].order > arcs[startNum][w].order){
						order_temp[i] = w;
						order_temp[j] = v;
					}
				}
			}
		}
		for(int i = 0; i < V; ++i){
			int w = order_temp[i];
			if(w != INFINITY && !visited[w]){ 
				if(w == goalNum){
					nodes[w].parent = startNum;
					Solution(w, visited);
					return true;
				}
				visited[w]=true;
				nodes[w].parent = startNum;
				open.push_back(w);
			}
		}
	}
	return false;
}

bool Search::DFS(Graph g){
	if (startNum == goalNum){
		output << nodes[startNum].state << " 0" << endl;
		return true;
	}
	bool *visited = new bool[V];
	for (int i = 0; i < V; i++)
		visited[i] = false;

	visited[startNum] = true;
	open.push_front(startNum);

	list<int> temp;
	int *order_temp = new int[V];
	for(int i = 0; i < V; ++i)
		order_temp[i] = INFINITY;
	int ord;
	while(!open.empty())
	{
		ord = 0;
		startNum = open.front();
		open.pop_front();
		closed.push_back(startNum);
		//ensure enqueue nodes with the live traffic's order.
		for(int w = g.FirstVex(startNum); w >= 0 ; w = g.NextVex(startNum, w)){
			if(arcs[startNum][w].order != INFINITY){
				order_temp[ord] = w;
				++ord;
			}
		}
		for(int i = 0; i < V-1; ++i){
			for(int j = i+1; j < V; ++j){
				if(order_temp[i] != INFINITY && order_temp[j] != INFINITY){
					int v = order_temp[i];
					int w = order_temp[j];
					if(arcs[startNum][v].order > arcs[startNum][w].order){
						order_temp[i] = w;
						order_temp[j] = v;
					}
				}
			}
		}
		for(int i = 0; i < V; ++i){
			int w = order_temp[i];
			if(w != INFINITY && !visited[w]){ 
				if(w == goalNum){	
					nodes[w].parent = startNum;
					Solution(w, visited);
					return true;
				}
				visited[w]=true;
				nodes[w].parent = startNum;
				temp.push_front(w);
			}
		}
		while(!temp.empty()){
			int t = temp.front();
			temp.pop_front();
			open.push_front(t);
		}
	}
	return false;
}

bool Search::UCS(Graph g){
	if (startNum == goalNum){
		output << nodes[startNum].state << " 0" << endl;
		return true;
	}

	bool *visited = new bool[V];
	for(int i = 0; i < V; ++i)
		visited[i] = false;

	int curr;  // current node

	open.push_back(startNum);
	nodes[startNum].g_cost = 0;

	while(!open.empty()){
		curr = open.front();
		open.pop_front();
		visited[curr] = true;
		closed.push_back(curr);
		if(curr == goalNum){
			Solution(curr, visited);
			return true;
		}
		for(int w = g.FirstVex(curr); w >= 0 ; w = g.NextVex(curr, w)){
			UCSUtil(curr, w, visited);
		}	
		//sort;
		int size = open.size();
		int *sort_open = new int[size];
		for(int i = 0; !open.empty(); ++i){
			sort_open[i] = open.front();
			open.pop_front();
		}
		for(int i = 0; i < size-1; ++i){
			for(int j = i+1; j < size; ++j){
				int v = sort_open[i];
				int w = sort_open[j];
				if(nodes[v].g_cost > nodes[w].g_cost){
					sort_open[i] = w;
					sort_open[j] = v;
				}
			}
		}
		for(int i = 0; i < size; ++i){
			open.push_back(sort_open[i]);
		}
	}
	return false;
}

bool Search::AStar(Graph g){
	if (startNum == goalNum){
		output << nodes[startNum].state << " 0" << endl;
		return true;
	}

	bool *visited = new bool[V];
	for(int i = 0; i < V; ++i)
		visited[i] = false;

	int curr;  // current node

	f_cost = new int[V];
	for(int i = 0; i < V; ++i)
		f_cost[i] = INFINITY; 
	open.push_back(startNum);
	nodes[startNum].g_cost = 0; 
	f_cost[startNum] = nodes[startNum].g_cost + nodes[startNum].h_cost;

	while(!open.empty()){
		curr = open.front();		
		open.pop_front();
		visited[curr] = true;
		closed.push_back(curr);
		if(curr == goalNum){
			Solution(curr, visited);
			return true;
		}
		for(int w = g.FirstVex(curr); w >= 0 ; w = g.NextVex(curr, w)){
			AStarUtil(curr, w, visited);
		}
		//sort;
		int size = open.size();
		int *sort_open = new int[size];
		for(int i = 0; !open.empty(); ++i){
			sort_open[i] = open.front();
			open.pop_front();
		}
		for(int i = 0; i < size - 1; ++i){
			for(int j = i+1; j < size; ++j){
				int v = sort_open[i];
				int w = sort_open[j];
				if(f_cost[v] > f_cost[w]){
					sort_open[i] = w;
					sort_open[j] = v;
				}
			}
		}
		for(int i = 0; i < size; ++i){
			open.push_back(sort_open[i]);
		}
	}
	return false;
}

int main()
{
	// get data from input.txt
	ifstream input("input.txt");
	input >> algo; //get algorithm
	string startState; //get start state
	input >> startState;
	string goalState;
	input >> goalState; //get goal state

	int numLiveTraffic;
	input >> numLiveTraffic;  //get number of live traffic
	vector<string> b_path;
	vector<string> e_path ;
	vector<int> path_cost;
	string temp;
	int tnum;
	for(int i = 0; i < numLiveTraffic; ++i){   // get all information of live traffic
		input >> temp;
		b_path.push_back(temp);
		input >> temp;
		e_path.push_back(temp);
		input >> tnum;
		path_cost.push_back(tnum);
	}

	int numSundayTraffic;
	input >> numSundayTraffic;  // get number of sunday traffic, a.k.a number of vertex
	int numVertex = numSundayTraffic; 
	Node_data* nodes = new Node_data[numVertex];
	for(int i = 0; i < numVertex; ++i){   // get all estimated cost and state of all vertex
		input >> temp;
		nodes[i].state = temp;
		input >> tnum;
		nodes[i].h_cost = tnum;
		nodes[i].g_cost = INFINITY;
		nodes[i].parent = -1;
	}

	// create a graph
	Arc_data **arcs = new Arc_data*[numVertex];
	for(int i = 0; i < numVertex; ++i){
		arcs[i] = new Arc_data[numVertex];
		for(int j = 0; j < numVertex ; ++j){
			arcs[i][j].value = INFINITY;
			arcs[i][j].order = INFINITY;
		}
	}
	Graph g(numVertex, nodes, arcs);
	int ord = 0;
	for(int i = 0; i < numLiveTraffic; ++i ){
		int v, w;
		v = g.LocateString(b_path[i]);
		w = g.LocateString(e_path[i]);
		g.addEdge(v,w, path_cost[i]);
		++ord;
		arcs[v][w].order = ord;
	}

	// search
	int startNum = g.LocateString(startState);
	int goalNum = g.LocateString(goalState);

	Search s(numVertex, nodes, arcs,startNum, goalNum);
	if(algo == "BFS")
		s.BFS(g);
	else if(algo == "DFS")
		s.DFS(g);
	else if (algo == "UCS")
		s.UCS(g);
	else if (algo == "A*")
		s.AStar(g);

	return 0;
}