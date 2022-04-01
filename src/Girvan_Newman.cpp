/******************************************************
* CS 3353 - PA 03
* Description: Implementation of the Girvan Newman algorithm
*******************************************************/

#include "Girvan_Newman.h"

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphml.hpp>
#include <iterator>
#include <fstream>
#include <string>
#include <vector>

using namespace boost;

//Creates an unweighted and undirected graph from the passed GraphML file
Girvan_Newman::Girvan_Newman(const std::string& file){

    std::ifstream read(file);
    if(read.is_open()){

        dynamic_properties dp(ignore_other_properties);
        dp.property("name", get(&VertexData::name, graph));
        dp.property("value", get(&VertexData::value, graph));
        read_graphml(read, graph, dp);

        //initialize the vertices' indexes
        int currentId = 0;
        for(auto vp = vertices(graph); vp.first != vp.second; vp.first++){

            graph[*vp.first].index = currentId++;
        }
    }
    else
        throw std::runtime_error("Unable to read the input data file");
}

//Outputs the graph's vertices and edges by their names to the console.
void Girvan_Newman::printGraph(){

    if(num_vertices(graph) == 0 || num_edges(graph) == 0)
        throw std::runtime_error("Graph is empty");
    else{

        std::cout << "Graph: " << num_vertices(graph) << " vertices, " << num_edges(graph)
                  << " edges" << std::endl;

//        print_graph(graph, get(&VertexData::name, graph)); //prints the vertices and edges by their names
        print_graph(graph, std::cout); //prints the vertices and edges by their indexes
    }
}

/* Helper function for biDirSearch. Returns true if the forward and backward searches
 * visit the same node.*/
int Girvan_Newman::isIntersecting(bool* s_visit, bool* t_visit) {

    for(int i = 0; i < num_vertices(graph); i++){

        if(s_visit[i] && t_visit[i])
            return i; //change to index of vertex that is intersected
    }
    return -1;
}

/* Helper function for biDirSearch. */
void Girvan_Newman::BFS(std::list<Graph::vertex_descriptor>* queue, Graph::vertex_descriptor* prev,
                        bool* visited){

    Graph::vertex_descriptor current = queue->front();
    queue->pop_front();

    for(auto adj: make_iterator_range(adjacent_vertices(current, graph))){

        if(!visited[adj]){

            prev[adj] = current;
            visited[adj] = true;
            queue->push_back(adj);
        }
    }
}

/* Searches from the source and target vertices to generate their shortest paths.
 * Referenced from GeeksforGeeks: https://www.geeksforgeeks.org/bidirectional-search/ */
void Girvan_Newman::biDirSearch(Graph::vertex_descriptor& source, Graph::vertex_descriptor& target){

    //keeps track of visited nodes
    bool s_visited[num_vertices(graph)], t_visited[num_vertices(graph)];
    Graph::vertex_descriptor s_prev[num_vertices(graph)], t_prev[num_vertices(graph)];
    for(int i = 0; i < num_vertices(graph); i++){
        s_visited[i] = false;
        t_visited[i] = false;
    }

    std::list<Graph::vertex_descriptor> s_queue, t_queue;
    s_queue.push_back(source);
    s_visited[source] = true;
    s_prev[source] = -1;    //vertex_descriptors are the vertex's index

    t_queue.push_back(target);
    std::cout << target << std::endl;
    t_visited[target] = true; //segfault here, the index probably exceeds the arrays -> yep, so how do we constrain it?
    t_prev[target] = -1;

    while(!s_queue.empty() && !t_queue.empty()){

        BFS(&s_queue, s_prev, s_visited);
        BFS(&t_queue, t_prev, t_visited);

        int intersectNode = isIntersecting(s_visited, t_visited);

        //If a path is found, store the path in a vector<vector>?
        if(intersectNode != -1){
            std::cout << "found a path" << std::endl;
        }
    }
}

/* Uses a bidirectional search to generate the shortest paths for all nodes.
 * Returns anything? */
void Girvan_Newman::findShortestPaths(){

    if(num_vertices(graph) == 0 || num_edges(graph) == 0)
        throw std::runtime_error("Graph is empty");

    //iterate through all pairs and save the shortest paths
    //avoid what has been visited previously
    for(auto vp = vertices(graph); vp.first != vp.second; vp.first++){

        Graph::vertex_descriptor s = *vp.first;
//        std::cout << get(vertex_index_t, graph, s);
        vp.first++;

        if(vp.first != vp.second){
            Graph::vertex_descriptor t = *vp.first;
            std::cout << " " << *vp.first << std::endl;
//            biDirSearch(s, t);
        }
    }
}

//function for the algo itself and output results to a file
void Girvan_Newman::computeGroups(){

    if(num_vertices(graph) == 0 || num_edges(graph) == 0)
        throw std::runtime_error("Graph is empty");

//    printGraph();
    findShortestPaths();
}
