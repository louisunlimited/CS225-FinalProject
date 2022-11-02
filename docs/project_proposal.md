# Project Proposal

## Leading Question

We have acquired a dataset on the San Fransico Road Network, and we are thinking about developing a project to help police cope with crime activities by providing efficient ways(routes) to chase criminals. Our project can be used to achive the following goals:

1. **Identify the importance of places in the city:** With the help of our project, police can identify the importance of places in the city, and can focus on the places that are more important. This can be achieved by using dijkstra's algorithm to find all shortes paths from a given node to all other nodes in the graph, and calculate the weight of how many times a node is visited. The corresponding weights represents the importance of places in the city.

2. **Optimum route for chasing criminals:**

3. **Optimum route for searching a specific area:**

4. **Finding the next best position for a new police station:** We would also want to help the police to find the next best position for the new police station with the dataset we have. This can be achieved by using the same method as the first goal, but this time we will use a kd-tree to populate the graph.

## Dataset Acquisition and Processing

1. Data Format:

    We are using [San Francisco Road Network Data](https://www.cs.utah.edu/~lifeifei/SpatialDataset.htm) and working with the following two datasets:

    - [San Francisco Road Network's Nodes](https://www.cs.utah.edu/~lifeifei/research/tpq/SF.cnode)

    - [San Francisco Road Network's Edges](https://www.cs.utah.edu/~lifeifei/research/tpq/SF.cedge)

    Additionally, we added [San Francisco Police Station data](https://data.sfgov.org/Public-Safety/Map-of-Police-Stations-2011-/a9xm-jdem) to our existing dataset for reference.

    For the first two dataset on `San Fransico Road Network`, the dataset is in `.txt` file so we can directly read from them.

    For the `police station data`, the dataset is in `.csv` and we can directly read as well.

    The data we have on road network in rather large, with approximatly 170,000 Nodes and 220,000 Edges. Whilest we only have roughly 10 police department data. As for now, we do plan to use all data acquired.

2. Data Correction:

    We are going to check whether the input is a `.csv` or a `.txt` and we will parse the data accordingly. We don't think there will be any major errors present in our dataset, but we will do some prelimilary checks for GPS Coordinates and else. For invalid data, we will check whether weight is larger than or equal to the absolute distance between two points. We will make sure a weight is present  and no two weight present for same set of nodes.

3. Data Storage:

    For our mapdata, we will be storing them in a graph wiht an Adjacency List, which would take O(E + V) space. And we might use KD-tree to find the nearest neighour, which would take O(Vlog(V)).

## Graph Algorithms

1. Find importance of all nodes

    **Algorithm**: `Dijkstra's algorithm` will be applied to every node to find all shortest paths. Then for every point, we will calculate the betweeness centrality as its importance value.

    **Input**: (empty)

    **Output**:
     - A `PNG` containing the map of the entire San Francisco. All nodes are colored differently to match its betweeness centrality value.

    **Complexity**:
     - Time: $O((|V| + |E|)|V|log(|V|))$
     - Space: $O(|V|)$ (Note that the shortest paths are not stored simutaneously)

2. Path finding between any two location

    **Algorithm**: First, `k-d tree` is used to find the nearest node to the two given locations. Then we apply `A* search algorithm` for finding the shortest path between the two nodes.

    **Input**:
     - Starting coordinate: `Coord(double, double)`
     - Destination coordinate: `Coord(double, double)`
     - Zoom factor: `double`

    **Output**:
     - A `PNG` containing the map showing the path from the starting node to the target node. The image is scaled by the given factor.

    **Complexity**:
     - Time: $O((|V| + |E|)log(|V|))$
     - Space: $O(|V|)$

3. Finding all nodes within distance

    **Algorithm**: The list of nodes can be obtained by going through the procedure as specified by `Dijkestra's algorithm` and record all nodes visited. Nodes outside the range will be dropped immediately without appending to the queue.

    **Input**:
     - Starting coordinate: `Coord(double, double)`
     - Zoom factor: `double`

    **Output**:
     - A `PNG` showing a zoomed map that highlights all possible nodes availavble to reach. The image is scaled by the given factor.

    **Complexity**:
     - Time: $O((|V| + |E|)log(|V|))$
     - Space: $O(|V|)$

4. Finding best location for new police station

    **Algorithm**: We will use `k-d tree` to map the current police stations into nodes. Then we apply `Dijkstra's algorithm` to measure the distance of all nodes to the nearest police station. This can be achieved by initally pushing all police station nodes into the priority queue. Finally, the node with the largest distance will be selected as the best location.

    **Input**:
     - Zoom factor: `double`

    **Output**:
     - A `PNG` with highlighted location for the next possible police station. The image is scaled by the given factor.

    **Complexity**:
     - Time: $O((|V| + |E|)log(|V|))$
     - Space: $O(|V|)$

## Timeline

    11.4 - 11.14~18 - Dec 8

    10.31 - 11.3: Finish project proposal
    11.4 - 11.7: Finish data handling methods, populate data stucture.
    11.7 - 11.13: Finish algo for one task (without graph).
    11.14 - Dec.8: Finish everything according to tasks. Each taking about 1.2 week.
