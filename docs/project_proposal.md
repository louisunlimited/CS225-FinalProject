# Project Proposal

## Leading Question

We have acquired a dataset on the San Fransico Road Network, and we are thinking about developing a project to help police cope with crime activities by providing efficient ways(routes) to chase criminals. Our project can be used to achive the following goals:

1. **Identify the importance of places in the city:** With the help of our project, police can identify the importance of places in the city, and can focus on the places that are more important. This can be achieved by using dijkstra's algorithm to find all shortes paths from a given node to all other nodes in the graph, and calculate the weight of how many times a node is visited. The corresponding weights represents the importance of places in the city.

2. **Optimum route for chasing criminals:** With the help of this function, police can find the best route to chase a target (a criminal), which can help them save time and increase the probability of catching criminals. After getting three parameters, the starting coordinate, the destination coordinate and the zoom factor, we will firstly use the k-d tree to find the nearest node to the two given location(the starting coordinate and the destination coordinate). Then we will apply the A* search algorithm to find the nodes that can form the shortest path between the two given nodes. In the end, the function will generate a map containing the path, which will be scaled by the third parameter, the zoom factor.

3. **Police Training Simulator:** Want to become better in chansing criminals? Use this simulator to try to simulate a criminal's running route and try to catch them. After getting the starting coordinates for the path, we would output a GIF showing the criminals running path. We will use DFS to similate this route.

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

    We are going to check whether the input is a `.csv` or a `.txt` and we will parse the data accordingly. We don't think there will be any major errors present in our dataset, but we will do some prelimilary checks for GPS Coordinates and else. For invalid data, we will check whether weight is larger than or equal to the absolute distance between two points. We will make sure a weight is present  and no two weight present for same set of nodes. Furthermore, we would also want to check whether the graph is connected. The input is only valid if there exists a connected compoents that contains more than half of the total number of nodes.

    For police station nodes, we will be approximating them to our map node for easier reference.

3. Data Storage:

    For our mapdata, we will be storing them in a graph with an Adjacency List, which would take O(E + V) space. And we might use KD-tree to find the nearest neighour, which would take O(Vlog(V)). We will be storing Police Stations in a vector than takes O(N), where N is the number of stations present.

## Graph Algorithms

1. Identify the importance of places in the city

    **Algorithm**: `Dijkstra's algorithm` will be applied to every node to find all shortest paths. Then for every point, we will calculate the betweeness centrality as its importance value.

    **Input**: RBG value(optional)

    **Output**:
     - A `PNG` containing the map of the entire San Francisco. All nodes are colored, according to user input or default, differently to match its betweeness centrality value.

    **Complexity**:
     - Time: $O((|V| + |E|)|V| log(|V|))$
     - Space: $O(|V|)$ (Note that the shortest paths are not stored simutaneously)

2. Optimum route for chasing criminals

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

3. Police Training Simulator

    **Algorithm:** Depth-First-Search will be used to select the path for the criminal through a graph. The nodes will be coordinates and the edges are corresponding to the length of the road between.

    **Input:**
    - Starting coordinate: `Coord(double, double)`

    **Output:**
    - A `GIF` showing a regular map that shows the movement of the criminal out of town.

    **Complexity:**
    - Time: $O(|V| + |E|)$
    - Space: $O(|V|)$

4. Finding the next best position for a new police station

    **Algorithm**: We will apply `Dijkstra's algorithm` to measure the distance of all nodes to the nearest police station. This can be achieved by initially pushing all police station nodes into the priority queue. Finally, the node with the largest distance will be selected as the best location.

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
