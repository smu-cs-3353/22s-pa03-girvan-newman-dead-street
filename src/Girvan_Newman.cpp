/******************************************************
* CS 3353 - PA 03
* Description: Implementation of the Girvan Newman algorithm
*******************************************************/

#include "Girvan_Newman.h"

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphml.hpp>
#include <fstream>
#include <string>

using namespace boost;

//Creates an unweighted and undirected graph from the passed GraphML file
Girvan_Newman::Girvan_Newman(const std::string& file){

    std::ifstream read(file);
    if(read.is_open()){

        dynamic_properties dp(ignore_other_properties);
        dp.property("value", get(&VertexData::value, graph));
        read_graphml(read, graph, dp);
    }
    else
        throw std::runtime_error("Unable to read the input data file");
}

//Outputs the graph's vertices and edges to the console
void Girvan_Newman::printGraph(){

    if(num_vertices(graph) == 0)
        std::cout << "Graph is empty" << std::endl;
    else{

        std::cout << "Graph: " << num_vertices(graph) << " vertices, " << num_edges(graph)
                  << " edges" << std::endl;

        print_graph(graph, get(&VertexData::value, graph));
    }
}

/* Uses a bidirectional search to find the shortest paths for all nodes.
 * Returns anything? */
void Girvan_Newman::findShortestPaths(VertexData& start, VertexData& end){
    //need to avoid duplicate paths ex: (A, B) & (B, A) are the same
    //
}