/******************************************************
* CS 3353 - PA 03
* Description: Header for the Girvan Newman algorithm
*******************************************************/

#ifndef INC_22S_PA03_GIRVAN_NEWMAN_H
#define INC_22S_PA03_GIRVAN_NEWMAN_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <string>
#include <list>

using namespace boost;

struct VertexData{
    std::string name;
    long value;
};

class Girvan_Newman {
    private:

            typedef adjacency_list<vecS, vecS, undirectedS, VertexData> Graph;
            Graph graph;

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
            Girvan_Newman(const std::string&);
            void printGraph();
            ////TO DO

            //helper functions for findShortestPaths
            int isIntersecting(bool*, bool*);
            void BFS(std::list<Graph::vertex_descriptor>*, Graph::vertex_descriptor*, bool*);
            void biDirSearch(Graph::vertex_descriptor&, Graph::vertex_descriptor&);

            //function to find all shortest paths <- use bidirectional search?
            void findShortestPaths();

            //what should we return?

            //function to calculate edge-betweenness: how many times is an edge used in a path

            //function to remove edges

            void computeGroups();
};


#endif //INC_22S_PA03_GIRVAN_NEWMAN_H
