/******************************************************
* CS 3353 - PA 03
* Description: Implementation of the Girvan Newman algorithm
*******************************************************/

#include "Girvan_Newman.h"

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphml.hpp>
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
    }
    else
        throw std::runtime_error("Unable to read the input data file");
}

//Outputs the graph's vertices and edges to the console
void Girvan_Newman::printGraph(){

    if(num_vertices(graph) == 0 || num_edges(graph) == 0)
        throw std::runtime_error("Graph is empty");
    else{

        std::cout << "Graph: " << num_vertices(graph) << " vertices, " << num_edges(graph)
                  << " edges" << std::endl;

//        std::cout << graph[num_vertices(graph)-1].name << std::endl;
        print_graph(graph, get(&VertexData::name, graph));
        //print the graph to a file
    }
}

/* Uses a bidirectional search to generate the shortest paths for all nodes.
 * Returns anything? */
void Girvan_Newman::findShortestPaths(){

    if(num_vertices(graph) == 0 || num_edges(graph) == 0)
        throw std::runtime_error("Graph is empty");

    //need to avoid duplicate paths ex: (A, B) & (B, A) are the same
//    std::vector<std::pair<Graph::vertex_descriptor, bool>> visited(num_vertices(graph));
//    for(auto vp = boost::vertices(graph); vp.first != vp.second; vp.first++){
//        visited.emplace_back(std::make);
//    }
}