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

/* Searches from the source and target vertices to generate their shortest paths.
 * Referenced from GeeksforGeeks: https://www.geeksforgeeks.org/bidirectional-search/ */
void Girvan_Newman::biDirSearch(std::vector<std::vector<Graph::vertex_descriptor>>& paths,
                                Graph::vertex_descriptor& source, Graph::vertex_descriptor& target){

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
            if(dupePath == paths.end())
                paths.emplace_back(path);
        }
    }
}

/* Uses a bidirectional search to generate the shortest paths for all nodes.
 * pass a start vertex and end vertex?*/
void Girvan_Newman::findShortestPaths(std::vector<std::vector<Graph::vertex_descriptor>>& paths){

    if(num_vertices(graph) == 0 || num_edges(graph) == 0)
        throw std::runtime_error("Graph is empty");

    //iterate through all pairs and save the shortest paths
    for(auto vp = vertices(graph); vp.first != vp.second; vp.first++){

        Graph::vertex_descriptor s = *vp.first;

        for(auto vp2 = vertices(graph); vp2.first != vp2.second; vp2.first++){

            if(vp2.first > vp.first){
                Graph::vertex_descriptor t = *vp2.first;
                biDirSearch(paths,s, t);
            }
        }
    }
}

/* Takes in a vector containing the shortest paths and a vector containing the edges.
 * Calculates the edge betweeness centrality for each edge.*/
void Girvan_Newman::calculateEdgeBetweeness(std::vector<std::vector<Graph::vertex_descriptor>>& paths,
                             std::vector<std::pair<Graph::edge_descriptor, double>>& edgesBetweenValues){

    //assigns edge weights based on the shortest paths
    for(auto it: paths){

        std::vector<Graph::vertex_descriptor> path = it;

        auto it2 = path.begin();
        auto source_vertex = *it2;
        auto target_vertex = *(it2+1);
        it2++;

        //find each edge used in path
        while(it2 != path.end()){

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
            target_vertex = *(it2+1);
            source_vertex = temp;
            it2++;
        }
    }

    //calculate betweeness centrality for each edge
    for(auto ed: edgesBetweenValues){
        ed.second /= (double)paths.size();
        std::cout << ed.second << std::endl;
    }

    ////TO DO sort edgesBetweenessValues in descending order by the double
}

//function for the algo itself and output results to a file
void Girvan_Newman::computeGroups(){

    if(num_vertices(graph) == 0 || num_edges(graph) == 0)
        throw std::runtime_error("Graph is empty");

    //printGraph(); //for debugging purposes, it helps me see the vertices and edges

    std::vector<std::vector<Graph::vertex_descriptor>> communities;
    std::vector<std::pair<Graph::edge_descriptor, double>> edgesBetweenValues;
    std::vector<std::vector<Graph::vertex_descriptor>> paths;
    int original_edges = num_edges(graph); //used to calculate the modularity

    //initalize the vector that will contain the edges and their betweenness value
    for(auto ed: make_iterator_range(edges(graph))){
        edgesBetweenValues.emplace_back(std::make_pair(ed, 0.0));
    }

    /* The metric used to signal that communities have been found. If -1 < modularity < 1, then the
     * algorithm will stop. Otherwise, continue to remove edges. */
    double total_modularity = 0;

    //Girvan Newman Algorithm
    do{
        //use source(ed, graph) & target(ed, graph) to get the vertex descriptors

        //generate the shortest paths
        findShortestPaths(paths);

        //calculate edge betweenness
        calculateEdgeBetweeness(paths, edgesBetweenValues);

        //Find the edge(s) with the highest edge betweenness values
        //could use <algorithm> to order the vector by the edge value
        double max = edgesBetweenValues.at(0).second;
        for(auto it: edgesBetweenValues){

            if(it.second > max)
                max = it.second;
        }

        //remove edges from graph and edgesBetweenValues
        int i = 0;
        for(auto it: edgesBetweenValues){

            if(it.second == max){

                Graph::vertex_descriptor v1 = source(it.first, graph);
                Graph::vertex_descriptor v2 = target(it.first, graph);
                remove_edge(v1, v2, graph);
                break;
            }
            else{
                i++;
                it.second = 0;
            }
        }
        edgesBetweenValues.erase(edgesBetweenValues.begin()+i);
        paths.clear();

        //calculate total_modularity
        total_modularity--;
    }while(total_modularity > 1);

    std::cout << "Modularity = " << total_modularity << std::endl;

    //generate all shortest paths
    //calculate their edge betweeness, be sure to divide by 2 at the end since its undirected
    //remove the edges with highest betweeness
    //recalculate paths* and edge betweeness
    //check if the -1 < total modularity < 1, which is the stopping point

    ////TO DO:
    // Girvan Newman: repeatedly call findShortestPaths, calculate edge betweenness, & remove edges
    // until the groups/communities are found
    // when is a good time to stop? look more info on modularity, that's the metric
    // https://www.youtube.com/watch?v=LtQoPEKKRYM
    // https://medium.com/analytics-vidhya/girvan-newman-the-clustering-technique-in-network-analysis-27fe6d665c92
    // ^ links for girvan newman help

    // edge betweeness: an edge descriptor & its value: (a, b) => degree of a/degree of b +
    // output groups/communities in a file, what kind of file?

    /* How to remove an edge:
     * reqs: 2 vertex_descriptors & the graph
     * example:
     *          vertex_descriptor rip = something;
     *          vertex_descriptor sleep = something2; //The 2 vertex_descriptors are what's connected by the edge
     *                                                //that you want to remove. Removing an edge will not remove
     *                                                //the vertices.
     *          remove_edge(rip, sleep, graph);
     *
     * How to find an edge in the graph:
     * reqs: same as ^
     * example:
     *          boost::edge(hm, huh, test).first; //Remember that an edge is a pair<vertex_descriptor, bool>*/
}
