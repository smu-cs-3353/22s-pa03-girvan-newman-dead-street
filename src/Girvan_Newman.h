/******************************************************
* CS 3353 - PA 03
* Description: Header for the Girvan Newman algorithm
*******************************************************/

#ifndef INC_22S_PA03_GIRVAN_NEWMAN_H
#define INC_22S_PA03_GIRVAN_NEWMAN_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <string>

struct VertexData{
    std::string Name;
};

class Girvan_Newman {
    private:

            typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                                          boost::property<boost::vertex_name_t, std::string>> Graph;
            Graph test;

            /* for clarification on the parameters in <>:
             * 1st boost::vecS = vertices are stored in a vector
             * 2nd boost::vecS = edges are stored in a vector
             * boost::undirectedS = means the graph is undirected
             * boost::property<boost::vertex_name_t, std::string> = vertices have a string variable
             *                                                      that represents their name
             *
             * Note: Can add two more parameters to define edges' and graph's properties.
             * We can also redefine the vertices if the GraphML defines them differently
             * */
    public:
            Girvan_Newman(std::string&);
            void printGraph();
            ////TO DO
            //function to calculate edge-betweeness
            //separate function to remove edges
            //function to cluster
            //function for the algo itself and output results
};


#endif //INC_22S_PA03_GIRVAN_NEWMAN_H
