/******************************************************
* CS 3353 - PA 03
* Description: Implementation of the Girvan Newman algorithm
*******************************************************/

#include "Girvan_Newman.h"

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphml.hpp>
#include <fstream>
#include <string>

//Creates an unweighted and undirected graph from the passed GraphML file
Girvan_Newman::Girvan_Newman(std::string& file){

    std::ifstream read(file);
    if(read.is_open()){

        boost::dynamic_properties dp; //has to be defined but I'm not quite sure what we need
//        boost::read_graphml(read, test, dp); //currently gives an error
    }
    else
        throw std::runtime_error("Unable to read the input data file");
}

//Outputs the graph's vertices and edges
void Girvan_Newman::printGraph(){

    if(boost::num_vertices(test) == 0)
        std::cout << "Graph is empty" << std::endl;
    else
        boost::print_graph(test, boost::get(boost::vertex_name, test));
}