/***************************************
* Team: Dead Street
* Partners: Ashley Genato, Bryce Shurts, & Michael Amberg
* CS 3353 - PA 03
***************************************/

#include <iostream>
#include <boost/graph/adjacency_list.hpp>

int main(int argc, char **argv) {

    //type here to begin
    std::cout << "We can do this!" << std::endl;
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> test;
    std::cout << boost::num_vertices(test) << std::endl;

    return EXIT_SUCCESS;
}
