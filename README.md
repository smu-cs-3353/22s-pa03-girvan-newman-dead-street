# Community Detection | 22s-PA 03 | Team Dead Street

This repo's focus is to use the Girvan Newman algorithm to detect communities in dense networks/graphs.

For more information about our extension, which is also contained within this repo, please take a look at our [medium article series](https://medium.com/smucs/an-application-of-sparse-filtering-in-community-detection-part-1-c9ff782d109).

## Getting Started

Please note that this project will run Python 3 and C++ programs in the CLion IDE.

### Generating Graphs

The pythonThings directory contains python scripts that can convert or generate community graphs, which we will refer to as graphs. You can run these scripts in CLion's terminal.

The program will test its implementation of the Girvan Newman algorithm on graphs in the graphML format. Some graphs may be in the gml format instead, so we have to convert them to the graphML format. To do this, we use Python's NetworkX library. In convert.py, the script will to convert a gml file to a graphML file called "converted.graphml" in the data directory. Please note that convert.py will, by default, convert a gml file called "football.gml" in the the data directory. If you wish to test a different gml file, you must change the file path in convert.py so that the right file is converted.

We can also use the NetworkX library to generate random and undirected graphs with the script generation.py . This will generate a graphML file called "randomGraph.graphml" in the data directory. If you wish to use pre-existing graphs like Zachary's Karate Club, Les Misérables, or Florentine's Families, use the script pullOtherCommunities.py to generate the graphML files for those graphs, which can also be found in the data directory.


To convert gml to graphML:
```
python pythonThings/convert.py
```

To generate a random graph:
```
python pythonThings/generation.py
```

To generate an exisiting community graph:
```
python pythonThings/pullOtherCommunities.py
```

### Running Girvan-Newman

For more infomation on how the algorithm is implemented, see [the following section](#Implementation-of-the-Girvan-Newman-Algorithm).

The program will take the file path of the graph dataset as its input and program argument.

Here's an example of how to run the program:
```

./22s_PA03 ../data/converted.graphml
```


## Implementation of the Girvan-Newman Algorithm

Boost's [BGL library](https://www.boost.org/doc/libs/1_78_0/libs/graph/doc/index.html) is what we use to represent and manipulate graphs. All functions used for the algorithm can be found in the Girvan_Newman class (.h/.cpp).

### Creating the Graph

An adjacency list is used to represent an undirected and unweighted graph, which is filled with the contents of the input graphML file. Boost has functionality for reading graphML files thanks to the read_graphml function, and we use that when reading the graphML file.

### Finding the Shortest Path and Edge Betweenness Centrality

The first step in the Girvan-Newman algorithm is to find all the shortest paths. Graphs can become dense and complex with more vertices or edges. A single BFS searching from start to finish would become inefficient as the distance between the start and finish vertices grow, so we use a bidirectional search to find the shortest paths. With a bidirectional search, which is doing a BFS with the starting vertex and a BFS with the finishing vertex, the shortest path is found when both BFSs intersect. Thus, the time required to traverse the graph is shortened by half and gives a time complexity of O( b<sup>d/2</sup> ).

For every shortest path found, we assign edge weights to the edges used in that path. The edge weights are stored in a vector of pairs of the graph's edges and their values. This value is the edge betweenness centrality, which is defined by the edge weight (the total number of times an edge is used) divided by the total number of shortest paths in the graph. Once all paths are found and the edge betweenness values are assigned, the vector of pairs is sorted by the edge betweeness values in descending order. This makes it easier for us to find the edge with the highest edge betweenness centrality.

### Finding Communities

The algorithm continously removes edges with high edge betweenness centrality until it reaches a certain condition or metric. The metric we use to determine whether communities have been found is known as the modularity. A community is detected when the modularity is not equal to zero. Since there is no standardized quantity for modularity, we instead compare the modularity of the current graph to the previous modularity before edges were removed. If the current modularity is less than the previous modularity, that means all of the communities have been found and the algorithm will stop removing edges.

Modularity is calculated from this formula:
![1_-oCvsgi-QMkYnmkn-pJKgA](https://user-images.githubusercontent.com/79382708/162868619-475b38f9-7522-4e4c-911f-d5f4af80e0ec.png)

Once the communities are found, the communities are written to a graphml file in the data directory called "output.graphml". If you wish to see the original graph and the communities visualized, you can use the Python script checkOutput.py to display the results. Please note that when testing different graphs, you must change the file paths in checkOutput.py so that the right graphs are displayed.

To display graphs, go to CLion's terminal and run the following command:
```

python pythonThings\checkOutput.py
```


## Testing on the Football Teams Dataset

The Original Graph:

![football_origin](https://user-images.githubusercontent.com/79382708/162868712-a8a8244b-7291-4125-99cd-3612c828845f.png)

The Communities Found in the Graph:

![football_groups](https://user-images.githubusercontent.com/79382708/162868709-f4a567b8-048e-4ba6-b290-60861d6b82f8.png)

According to the second picture, the algorithm detected one large community and several smaller communities that are supposed to represent the different football conferences. While this shows that our implementation is capable of detecting communities, it is not as accurate as we would assume since there are communities only made up of one node/team. A reason for this could be because of the edge betweenness and modularity. Since the algorithm relies on these values, it is possible that our implementation/calculation for these values are not optiminal for large and dense graphs like the football teams graph. Although the algorithm may not work as effectively in a massive graph, perhaps it will perform better in a smaller graph.

## Testing on Random/Other Graphs

Florentine Families Graph

![Fams_origin](https://user-images.githubusercontent.com/79382708/162876725-5818dadc-e7ae-4764-99b0-5986348f45f0.png)

Communities Found In Florentine Families

![Fams_groups](https://user-images.githubusercontent.com/79382708/162876731-74dc7245-0e46-460d-95f0-3175a42e676c.png)

Random Graph:

![random_origin](https://user-images.githubusercontent.com/79382708/162873395-0764b7bd-b4e6-4fee-b136-a0fde2242db6.png)

Communities Found in Random

![random_groups](https://user-images.githubusercontent.com/79382708/162873393-e85c514e-21ef-4fec-bf37-5d963c9000f7.png)

Random Graph 2:

![random_origin2](https://user-images.githubusercontent.com/79382708/162876177-2c87555e-8f92-40ff-9bfd-f0cad50886fc.png)

Communities Found in Random 2:

![random_groups2](https://user-images.githubusercontent.com/79382708/162876168-a3343a79-ae0e-4f40-a859-54b1e93d5c56.png)

Just like with the football dataset, the algorithm also detected communities in smaller and randomly-generated graphs. These communities are more evenly distributed than the communities in the football dataset as they do not have a community of one node. To further check our implementation, we tested other smaller real-world graphs like Zachary's Karate Club and the Florentine Families. While the algorithm was able to show the communities of the Florentine Families, it could not do so for Karate Club. The reason for this could be because the modularity is not decreasing, which causes the algorithm to remove all of the edges. Therefore, our implementation of Girvan-Newman algorithm is capable of detecting communities, but only on sufficiently-sized graphs.

## Authors

* **Ashley Genato** - *@agenato*
* **Bryce Shurts** - *@S-Bryce*
* **Michael Amberg** - *@SaltyPeachcake*

## Medium Articles
Part 1: https://medium.com/smucs/an-application-of-sparse-filtering-in-community-detection-part-1-c9ff782d109

Part 2: https://medium.com/smucs/an-application-of-sparse-filtering-in-community-detection-part-2-methodology-757a829f94fd

Part 3: https://medium.com/smucs/an-application-of-deep-sparse-filtering-in-community-detection-part-3-3d197e86a045

## Acknowledgments

* **Dr. Mark Fontenot** - For his invaluable teachings and tolerance for the class' procrastination.
