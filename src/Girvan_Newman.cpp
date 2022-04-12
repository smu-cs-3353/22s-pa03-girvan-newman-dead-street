/******************************************************
* CS 3353 - PA 03
* Description: Implementation of the Girvan Newman algorithm
*******************************************************/

#include "Girvan_Newman.h"

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphml.hpp>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <string>
#include <iostream>

using namespace boost;

//Creates an unweighted and undirected graph from the passed GraphML file
Girvan_Newman::Girvan_Newman(const std::string& file){

    std::ifstream read(file);
    if(read.is_open()){

        dynamic_properties dp(ignore_other_properties);
        dp.property("name", get(&VertexData::name, graph));
        dp.property("value", get(&VertexData::value, graph));
        read_graphml(read, graph, dp);

        //initialize the vertices' indexes
        int currentId = 0;
        for(auto vp = vertices(graph); vp.first != vp.second; vp.first++){

            graph[*vp.first].index = currentId++;
        }
    }
    else
        throw std::runtime_error("Unable to read the input data file");
}

//Outputs the graph's vertices and edges by their names to the console.
void Girvan_Newman::printGraph(){

    if(num_vertices(graph) == 0 || num_edges(graph) == 0)
        throw std::runtime_error("Graph is empty");
    else{

        std::cout << "Graph: " << num_vertices(graph) << " vertices, " << num_edges(graph)
                  << " edges" << std::endl;

        print_graph(graph, std::cout); //prints the vertices and edges by their indexes
    }
}

/* Helper function for biDirSearch. Returns the index of the intersecting node
 * if the forward and backward searches have visited the same node.*/
int Girvan_Newman::isIntersecting(bool* s_visit, bool* t_visit) {

    for(int i = 0; i < num_vertices(graph); i++){

        if(s_visit[i] && t_visit[i])
            return i;
    }
    return -1;
}

/* Helper function for biDirSearch. Finds the vertices adjacent to the source vertex.*/
void Girvan_Newman::BFS(std::list<Graph::vertex_descriptor>* queue, Graph::vertex_descriptor* prev,
                        bool* visited){

    Graph::vertex_descriptor current = queue->front();
    queue->pop_front();

    Graph::adjacency_iterator adj, adjEnd;
    for(tie(adj, adjEnd) = adjacent_vertices(current, graph); adj != adjEnd; adj++){

        int adjIndex = get(&VertexData::index, graph, *adj);

        if(!visited[adjIndex]){

            prev[adjIndex] = current;
            visited[adjIndex] = true;
            queue->push_back(*adj);
        }
    }
}

/* Searches from the source and target vertices to generate their shortest path.
 * Referenced from GeeksforGeeks: https://www.geeksforgeeks.org/bidirectional-search/ */
void Girvan_Newman::biDirSearch(std::vector<std::pair<Graph::edge_descriptor, double>>& edgeValues,
                                std::vector<std::vector<Graph::vertex_descriptor>>& paths,
                                Graph::vertex_descriptor& source, Graph::vertex_descriptor& target){

    if(num_vertices(graph) == 0 || num_edges(graph) == 0)
        throw std::runtime_error("Graph is empty");

    else if(edge(source, target, graph).second){ //if an edge between source and target exist

        //store the path as a vector
        std::vector<Graph::vertex_descriptor> path;
        path.emplace_back(source);
        path.emplace_back(target);

        //add path to paths
        auto dupePath = std::find(paths.begin(), paths.end(), path);
        if(dupePath == paths.end()){
            setEdgeBetweeness(path, edgeValues);
            paths.emplace_back(path);
        }
    }
    else{

        //keeps track of visited nodes
        bool s_visited[num_vertices(graph)], t_visited[num_vertices(graph)];
        Graph::vertex_descriptor s_prev[num_vertices(graph)], t_prev[num_vertices(graph)];

        //initialize s_visited and t_visited
        for(int i = 0; i < num_vertices(graph); i++){
            s_visited[i] = false;
            t_visited[i] = false;
        }

        int s_index = get(&VertexData::index, graph, source);
        int t_index = get(&VertexData::index, graph, target);

        //initialize the forward and backward searches s_queue and t_queue
        std::list<Graph::vertex_descriptor> s_queue, t_queue;
        s_queue.push_back(source);
        s_visited[s_index] = true;
        s_prev[s_index] = -1;

        t_queue.push_back(target);
        t_visited[t_index] = true;
        t_prev[t_index] = -1;

        while(!s_queue.empty() && !t_queue.empty()){

            BFS(&s_queue, s_prev, s_visited);
            BFS(&t_queue, t_prev, t_visited);

            int intersectNode = isIntersecting(s_visited, t_visited);

            if(intersectNode != -1){ //means a path exists between source and target vertices

                //store the path as a vector
                std::vector<Graph::vertex_descriptor> path;
                int i = intersectNode;

                if (i == s_index)
                    path.emplace_back(source);

                while(i != s_index){
                    path.emplace_back(s_prev[i]);
                    i = get(&VertexData::index, graph, s_prev[i]);
                }
                std::reverse(path.begin(), path.end());

                i = intersectNode;
                while(i != t_index){
                    path.emplace_back(t_prev[i]);
                    i = get(&VertexData::index, graph, t_prev[i]);
                }

                //add path to paths
                auto dupePath = std::find(paths.begin(), paths.end(), path);
                if(dupePath == paths.end()){
                    setEdgeBetweeness(path, edgeValues);
                    paths.emplace_back(path);
                    break;
                }
            }
        }
    }
}

/* Uses a bidirectional search to generate the shortest paths for all nodes. */
void Girvan_Newman::findShortestPaths(std::vector<std::pair<Graph::edge_descriptor, double>>& edgeValues,
                                      std::vector<std::vector<Graph::vertex_descriptor>>& paths){

    if(num_vertices(graph) == 0 || num_edges(graph) == 0)
        throw std::runtime_error("Graph is empty");
    else{

        //iterate through all pairs and save the shortest paths
        for(auto vp = vertices(graph); vp.first != vp.second; vp.first++){

            Graph::vertex_descriptor s = *vp.first;

            if(degree(s, graph) != 0){

                for(auto vp2 = vertices(graph); vp2.first != vp2.second; vp2.first++){

                    if(vp2.first > vp.first){
                        Graph::vertex_descriptor t = *vp2.first;

                        if(degree(t, graph) != 0)
                            biDirSearch(edgeValues, paths,s, t);
                    }
                }
            }
        }
    }
}

/* Takes in a vector containing the shortest path and a vector containing the edges.
 * Assigns edge weights for each edge used in the path.*/
void Girvan_Newman::setEdgeBetweeness(std::vector<Graph::vertex_descriptor>& path,
                                      std::vector<std::pair<Graph::edge_descriptor, double>>& edgesBetweenValues){

    auto pathIter = path.begin();
    auto source_vertex = *pathIter;
    auto target_vertex = *(pathIter + 1);
    pathIter++;

    //find each edge used in path
    while(pathIter != path.end()){

        auto ed = edge(source_vertex, target_vertex, graph);
        for(auto& ed_value: edgesBetweenValues){

            bool check = source(ed.first, graph) == source(ed_value.first, graph);
            bool check2 = target(ed.first, graph) == target(ed_value.first, graph);

            if(check && check2){
                ed_value.second += 1.0;
                break;
            }
        }

        auto temp = target_vertex;
        target_vertex = *(pathIter + 1);
        source_vertex = temp;
        pathIter++;
    }
}

//Calculates the total modularity from the passed arguments.
double Girvan_Newman::calculateModularity(std::vector<std::vector<Graph::vertex_descriptor>>& paths, int original_edges){

    double sum = 0.0;
    double modularity = 0.0;
    //Sums the number of edges in the graph - the number of expected edges in the graph
    for(auto vp = vertices(graph); vp.first != vp.second; vp.first++){ // just runs through all the vertices

        Graph::vertex_descriptor firstNode = *vp.first;

        for(auto vp2 = vertices(graph); vp2.first != vp2.second; vp2.first++){

            Graph::vertex_descriptor secondNode = *vp2.first;

            double delta = 0.0;

            double degree_s_t = degree(firstNode, graph) * degree(secondNode, graph); // used in finding what expected edges should be

            double nodeA = 0.0;
            if(edge(firstNode, secondNode, graph).second) //if the vertices are directly connected
                nodeA = 1.0;
            else if(firstNode == secondNode)
                continue;

            //if a path between the vertices exist, find delta = number of edges in-between
            for(auto& path: paths){
                if(path.front() == firstNode && path.back() == secondNode){
                    delta = 2;
                    break;
                }
            }

            sum += (nodeA - (degree_s_t / (2.0 * original_edges))) * delta;
        }

    }

    modularity =  sum / (2.0 * original_edges);
    return modularity;
}



//function for the algo itself and output results to a file
void Girvan_Newman::computeGroups(){

    if(num_vertices(graph) == 0 || num_edges(graph) == 0)
        throw std::runtime_error("Graph is empty");

//    printGraph(); //for debugging purposes, it helps me see the vertices and edges

    std::vector<std::vector<Graph::vertex_descriptor>> communities;
    std::vector<std::pair<Graph::edge_descriptor, double>> edgesBetweenValues;
    std::vector<std::vector<Graph::vertex_descriptor>> paths;

    // initalize the vector that will contain the edges and their betweenness value
    for(auto ed: make_iterator_range(edges(graph))){
        edgesBetweenValues.emplace_back(std::make_pair(ed, 0.0));
    }

    // generate all the shortest paths & calculate edge betweeness
    findShortestPaths(edgesBetweenValues, paths);

    int original_edges = num_edges(graph); //used to calculate the modularity

    /* The metric used to signal that communities have been found. If total_modularity < 0.7, then
     * the algorithm will stop. Otherwise, continue to remove edges. */
    double total_modularity = 0;
    double prev_modularity = 0;
    bool cont = true;

    //Girvan Newman Algorithm
    do{

        std::cout << "Edges: " << num_edges(graph) << " ... Paths: " << paths.size() << std::endl;

        // Calculate betweeness centrality for each edge
        for(auto ed: edgesBetweenValues){
            ed.second /= (double)paths.size();
        }

        //Sort edgesBetweenessValues in descending order by edge betweeness value
        std::sort(edgesBetweenValues.begin(), edgesBetweenValues.end(), [](auto& left, auto& right){
            return left.second > right.second;
        });

        // Find and remove the edge(s) with the highest edge
        // betweenness value from the graph
        double highestEdge = edgesBetweenValues.begin()->second;
        while(!edgesBetweenValues.empty()){

            auto edgeToRemove = edgesBetweenValues.begin()->first;
            remove_edge(source(edgeToRemove, graph), target(edgeToRemove, graph), graph);
            edgesBetweenValues.erase(edgesBetweenValues.begin());

            std::cout << "With a value of " << highestEdge << ", remove this edge: " << edgeToRemove << std::endl;

            if(edgesBetweenValues.begin()->second != highestEdge)
                break;
        }

        //update paths and edgesBetweenValues
        for(auto& ed: edgesBetweenValues){
            ed.second = 0.0;
        }
        paths.clear();
        findShortestPaths(edgesBetweenValues, paths);

        prev_modularity = total_modularity - 0.002; //helps approach a better optimization

        //calculate total_modularity
        total_modularity = calculateModularity(paths, original_edges);

        cont = total_modularity > prev_modularity;

        std::cout << "Modularity = " << total_modularity << std::endl;

    }while(cont);

    std::cout << "Communities have been found!" << std::endl;

    //fill in the communities & output it to a file
    dynamic_properties dp(ignore_other_properties);
    dp.property("name", get(&VertexData::name, graph));
    dp.property("value", get(&VertexData::value, graph));

    std::filebuf fb;
    fb.open ("../data/output.graphml",std::ios::out);
    std::ostream fileOutput(&fb);

    write_graphml(fileOutput, graph, dp, true);
    fb.close();

}




