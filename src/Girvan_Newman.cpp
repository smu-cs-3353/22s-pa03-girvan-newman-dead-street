/******************************************************
* CS 3353 - PA 03
* Description: Implementation of the Girvan Newman algorithm
*******************************************************/

#include "Girvan_Newman.h"

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/property_map/property_map.hpp>
#include <fstream>
#include <string>

//Creates an unweighted and undirected graph from the passed GraphML file
Girvan_Newman::Girvan_Newman(std::string& file){

    std::ifstream read(file);
    if(read.is_open()){

//        boost::dynamic_properties dp;
//        dp.property("name", boost::get(&VertexData::name, graph));
//        boost::read_graphml(read, graph, dp); //Creates an error though I can't figure out why
    }
    else
        throw std::runtime_error("Unable to read the input data file");
}

//Outputs the graph's vertices and edges
void Girvan_Newman::printGraph(){

    if(boost::num_vertices(graph) == 0)
        std::cout << "Graph is empty" << std::endl;
    else{

        std::cout << "Graph: " << boost::num_vertices(graph) << " vertices, " << boost::num_edges(graph)
                  << " edges" << std::endl;
        boost::print_graph(graph, boost::get(&VertexData::name, graph));
    }
}