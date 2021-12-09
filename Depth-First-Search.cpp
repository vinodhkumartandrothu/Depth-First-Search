#include <list>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <regex>
#include <numeric>

using namespace std::chrono;
using namespace std;


class Graph {
    public:
        int nodes;
        list<int> *edges;
        bool *visited;

        //constructor of the graph
        Graph(int vertices) {
            nodes = vertices;//nodes
            edges = new list<int>[vertices];//edges list
            visited = new bool[vertices];//visited bool list
        }
        //to add edge to the graph
        void addEdge(int src, int dest){
            //pushing the edge to the graph
            edges[src].push_front(dest);
        }

        //to display nodes of graph
        void display_nodes(){
            cout<<"Nodes "<<endl;
            for (int i = 1; i < nodes; i++)
            {
                cout<<i<<" ";
            }
            cout<<endl;
        }

        //to traverse the graph in DFS order
        void graph_dfs(int vertex){
            visited[vertex] = true;
            list<int> edg = edges[vertex];
            cout << vertex << " ";

            list<int>::iterator i;
            for (i = edg.begin(); i != edg.end(); ++i){
                if (!visited[*i]){
                    graph_dfs(*i);
                }
            }
        }
};


vector<Graph> graphs;


void load_graphs(string filename){
    //file data parameters
	vector<string> row;
	string line, word, ed;
    list<int> *edgs;

    //file stream reading file
    fstream freader(filename, ios::in);
    if (freader.is_open()){
        while(getline(freader, line)){
            row.clear();
            stringstream s (line);

            // reading whole row
            while(getline(s , word, ',')){
                row.push_back(word);
            }
            
            //segrating no of node
            int node = stoi(row[0]) + 1;
            Graph g(node);

            //removing the no of nodes from the row
            row.erase(row.begin());
            
            //parsing edges of the graph
            for (int i = 0; i < row.size(); i++)
            {
                //poping out the braces of the edges
                row[i].erase(0,1);
                row[i].pop_back();

                //parsing edge with regex to split the string and find nodess
                string edg(row[i]);
                std::regex regex("\\-");
                vector<string> out (std::sregex_token_iterator(edg.begin(), edg.end(), regex, -1), std::sregex_token_iterator());

                //adding edge to graph
                int source = stoi(out[0]);
                int dest = stoi(out[1]);

                //adding edge to graph
                g.addEdge(source, dest);
            }
            graphs.push_back(g);
        }
    } else {
        cout << "Can't read the specidied file"<<endl;
    }

    cout<<"[+] Graphs are loaded from csv"<<endl;
};

void print_avg_time(vector<int> time){
    if(time.empty()){
        cout<<"No times are measured"<<endl;
    } else {
        // int count = time.size();
        auto avg = 1.0 * std::accumulate(time.begin(), time.end(), 0LL) / time.size();
        cout<<"Average time in milliseconds: "<< avg<< " ms"<<endl;
    }
}

int main() {
    
    // reading csv
    string filename = "data2.csv";

    load_graphs(filename);

    vector<int> times;

    //to display nodes and traverse through the nodes
    for (int i = 0; i < graphs.size(); i++)
    {
        cout<<"\tGraph: "<<i+1<<endl;
        graphs[i].display_nodes();
        int node;
        cout<<"Enter a node : ";
        cin >> node;
        if (node < 1 || node > graphs[i].nodes){
            cout<<"Node not exists in graph\n Sorry :( I will quit"<<endl;
            print_avg_time(times);
            exit(1);
        } else {
            auto start = high_resolution_clock::now();
            cout<<"DFS Traversal: ";
            graphs[i].graph_dfs(node);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            cout <<endl <<"Elapsed time in milliseconds: "<< duration.count()<< " ms" << endl;
            times.push_back(duration.count());
            cout<<endl<<endl;

        }
    }
    print_avg_time(times);

    return 0;
}
