/******************************************************
* CS 3353 - PA 03
* Description: Header for the Girvan Newman algorithm
*******************************************************/

#ifndef INC_22S_PA03_GIRVAN_NEWMAN_H
#define INC_22S_PA03_GIRVAN_NEWMAN_H

#include <boost/graph/adjacency_list.hpp>

class Girvan_Newman {
    private:
            boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> test;
            //^this seems to be class rather than an actual container

            /* for clarification on the parameters in <>:
             * 1st boost::vecS = vertices are stored in a vector
             * 2nd boost::vecS = edges are stored in a vector
             * boost::undirectedS = means the graph is undirected
             * the last 2 (not listed) are vertex & edge properties, create a struct above for vertex data?
             * */
    public:
            Girvan_Newman(); //do we need a default constructor?
            //constructor that will take in a dataset and create the graph
};


#endif //INC_22S_PA03_GIRVAN_NEWMAN_H
