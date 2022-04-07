/******************************************************
* CS 3353 - PA 03
* Description: Header for the Girvan Newman algorithm
*******************************************************/

#ifndef INC_22S_PA03_GIRVAN_NEWMAN_H
#define INC_22S_PA03_GIRVAN_NEWMAN_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <string>
#include <vector>
#include <list>

using namespace boost;

struct VertexData{
    std::string name;
    long value;
    int index;
};

class Girvan_Newman {
    private:

            typedef adjacency_list<vecS, vecS, undirectedS, VertexData> Graph;
            Graph graph;

            int isIntersecting(bool*, bool*);

            void BFS(std::list<Graph::vertex_descriptor>*, Graph::vertex_descriptor*, bool*);

            void biDirSearch(std::vector<std::vector<Graph::vertex_descriptor>>&,
                             Graph::vertex_descriptor&, Graph::vertex_descriptor&);

            void findShortestPaths(std::vector<std::vector<Graph::vertex_descriptor>>&);

            void calculateEdgeBetweeness(std::vector<std::vector<Graph::vertex_descriptor>>&,
                                         std::vector<std::pair<Graph::edge_descriptor, double>>&);

            void calculateModularity(std::vector<std::vector<Graph::vertex_descriptor>>&,
                                     double&, double&, int&);
    public:
            Girvan_Newman(const std::string&);
            void printGraph();

    ////TO DO////
            //function to calculate edge-betweenness: how many times is an edge used in a path
            void computeGroups();
};


#endif //INC_22S_PA03_GIRVAN_NEWMAN_H
