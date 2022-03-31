/***************************************
* Team: Dead Street
* Partners: Ashley Genato, Bryce Shurts, & Michael Amberg
* CS 3353 - PA 03
***************************************/

#include <iostream>
#include "Girvan_Newman.h"

int main(int argc, char **argv) {

    //Testing Girvan Newman Algorithm
    Girvan_Newman test(argv[1]);
    test.computeGroups();

    return EXIT_SUCCESS;
}
