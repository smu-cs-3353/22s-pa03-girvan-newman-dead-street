# Community Detection | 22s-PA 03 | Team Dead Street

This repo's focus is to use the Girvan Newman algorithm to detect communities in dense networks/graphs.

## Getting Started

Please note that this project will run Python 3 and C++ programs in the CLion IDE.

### Generating Graphs

The pythonThings directory contains python scripts that can convert or generate community graphs, which we will refer to as graphs. You can run these scripts in
CLion's terminal.

The program will test its implementation of the Girvan Newman algorithm on graphs in the graphML format. Some graphs may be in the gml format instead, so
we have to convert them to the graphML format. To do this, we use Python's NetworkX library. In convert.py, the script will to convert a gml file to a graphML file
called "converted.graphml" in the data directory. Please note that convert.py will, by default, convert a gml file called "football.gml" in the the data
directory. If you wish to test a different gml file, you must change the file path in convert.py so that the right file is converted.

We can also use the NetworkX library to generate random and undirected graphs with the script generation.py . This will generate a graphML file called
"randomGraph.graphml" in the data directory. If you wish to use pre-existing graphs like Zachary's Karate Club, Les Misérables, or Florentine's Families, use the
script pullOtherCommunities.py to generate the graphML files for those graphs, which can also be found in the data directory.


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


## Implementation of the Girvan Newman Algorithm

Boost's [BGL library](https://www.boost.org/doc/libs/1_78_0/libs/graph/doc/index.html) is what we use to represent and manipulate graphs. All functions used for
the algorithm can be found in the Girvan_Newman class (.h/.cpp).

### Creating the Graph

An adjacency list is used to represent an undirected and unweighted graph, which is filled with the contents of the input graphML file. Boost has functionality for reading graphML files thanks to the read_graphml function, and we use that when reading the graphML file.

### Finding the Shortest Path and Edge Betweeness

The first step to Girvan Newman is to find all the shortest paths and determine their edge betweenness (more on that later). Graphs can easily become dense and complex with more vertices or edges, so a singular BFS searching from start to finish would become inefficient as the distance between the starting and finishing vertices grow. Shortest paths are found through bidirectional search. Doing 2 BFS that will search from the source and target vertices, which would give O(b ^ d/2).

We assign edge weights based on the shortest paths. With every shortest path found, we update the edges that were used in that path. Once all the paths
are generated, we divide it by the total shortest paths. Lastly, to make the next step of the alogirthm easier, we sort the edge values in descending order.

### Finding Communities

Removing edges based on the edge betweeness

Modularity formula and finding a community

Outputting the communities


## Testing on the Football Dataset

GRAPH OUTPUT

DETERMINE ITS ACCURACY


## Testing on Random/Other Graphs

GRAPH OUTPUT

DETERMINE ITS ACCURACY


## Link to our blog posts?


## Authors

* **Ashley Genato** - *@agenato*
* **Bryce Shurts** - *@S-Bryce*
* **Michael Amberg** - *@SaltyPeachcake*


## Acknowledgments

* **Dr. Mark Fontenot** - For his invaluable teachings and tolerance of the class' procrastination.
