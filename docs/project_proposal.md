# Project Proposal

## Leading Question

We have acquired a dataset on the San Fransico Road Network, and we are thinking about developing a project to help police cope with crime activities by providing efficient ways(routes) to chase criminals. Our project can be used to achieve the following goals:

1. **Identify the importance of places in the city:** With the help of our project, police can identify the importance of places in the city and can focus on the places that are more important. This can be achieved by using Dijkstra's Algorithm to find all shortest paths from a given node to all other nodes in the graph and calculate the weight of how many times a node is visited. The corresponding weights represent the importance of places in the city.

2. **Optimum route for chasing criminals:** With the help of this function, police can find the best route to chase a target (a criminal), which can help them save time and increase the probability of catching criminals. After getting three parameters, the starting coordinate, the destination coordinate, and the zoom factor, we will first use the k-d tree to find the nearest node to the two given locations (the starting coordinate and the destination coordinate). Then we will apply the A* search algorithm to find the nodes that can form the shortest path between the two given nodes. In the end, the function will generate a map containing the path, which will be scaled by the third parameter, the zoom factor.

3. **Police Training Simulator:** Want to become better at chasing criminals? Use this simulator to try to simulate a criminal's running route and try to catch them. After getting the starting coordinates for the path, we would output a GIF showing the criminals' running path. We will use DFS to simulate this route.

4. **Finding the next best position for a new police station:** We would also want to help the police to find the next best position for the new police station with the dataset we have. This can be achieved by using a similar approach as the first goal. We will traverse all the node one by one and, regarding each node as a potential police station, then calculate the maximum distance for the police to reach any node on the graph. Then we would try to find the node (potential position) that minimizes this distance and take this node as our next possible police station.

## Dataset Acquisition and Processing

1. Data Format:

 We are using [San Francisco Road Network Data](https://www.cs.utah.edu/~lifeifei/SpatialDataset.htm) and working with the following two datasets:

 - [San Francisco Road Network's Nodes](https://www.cs.utah.edu/~lifeifei/research/tpq/SF.cnode)

 - [San Francisco Road Network's Edges](https://www.cs.utah.edu/~lifeifei/research/tpq/SF.cedge)

 Additionally, we added [San Francisco Police Station data](https://data.sfgov.org/Public-Safety/Map-of-Police-Stations-2011-/a9xm-jdem) to our existing dataset for reference.

 For the first two datasets on `San Fransico Road Network`, the dataset is in `.txt` file so we can directly read from them.

 For the `police station data`, the dataset is in `.csv` and we can directly read as well.

 The data we have on the road network is rather large, with approximately 170,000 Nodes and 220,000 Edges. While we only have roughly ten police department data. As for now, we do plan to use all data acquired.

2. Data Correction:

 We are going to check whether the input is a `.csv` or a `.txt`, and we will parse the data accordingly. We don't think there will be any major errors present in our dataset, but we will do some preliminary checks for GPS Coordinates and else. For invalid data, we will check whether the weight is larger than or equal to the absolute distance between two points. We will make sure a distinct weight value is present for a certain set of nodes. Furthermore, we would also want to check whether the graph is connected. The input is only valid if there exists a connected component that contains more than half of the total number of nodes.

 For police station nodes, we will be approximating them to our map node for easier reference.

3. Data Storage:

 For our map data, we will be storing them in a graph with an Adjacency List, which would take O(E + V) space. And we might use KD-tree to find the nearest neighbor, which would take O(Vlog(V)). We will be storing Police Stations in a vector than takes O(N), where N is the number of stations present.

## Graph Algorithms

1. Identify the importance of places in the city

 **Algorithm**: `Dijkstra's algorithm` will be applied to every node to find all shortest paths. Then for every point, we will calculate the betweenness centrality as its importance value.

 **Input**: RBG value(optional)

 **Output**:
 - A `PNG` containing the map of the entire San Francisco. All nodes are colored, according to user input or default, differently to match its betweenness centrality value.

 **Complexity**:
 - Time: $O((|V| + |E|)|V| log(|V|))$
 - Space: $O(|V|)$ (Note that the shortest paths are not stored simultaneously)

2. Optimum route for chasing criminals

 **Algorithm**: First, `k-d tree` is used to find the nearest node to the two given locations. Then we apply `A* search algorithm` to find the shortest path between the two nodes.

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

 **Algorithm:** Depth-First-Search will be used to select the path for the criminal through a graph. The nodes will be coordinates, and the edges correspond to the length of the road between them.

 **input:**
 - Starting coordinate: `Coord(double, double)`

 **Output:**
 - A `GIF` showing a regular map that shows the movement of the criminal out of town.

 **Complexity:**
 - Time: $O(|V| + |E|)$
 - Space: $O(|V|)$

4. Finding the next best position for a new police station

 **Algorithm**: We will traverse all the node one by one and regard each as a potential police station, then calculate the maximum distance for the police to reach any node on the graph using `Dijkstra's algorithm`. We aim to find the best solution that gives the minimum of such distance.

 **Input**:
 - Zoom factor: `double`

 **Output**:
 - A `PNG` with highlighted location for the next possible police station. The image is scaled by the given factor.

 **Complexity**:
 - Time: $O((|V| + |E|)|V|log(|V|))$
 - Space: $O(|V|)$

## Timeline

### **Week 1 (Oct 30th - Nov 5th)**

1. Development env setup

2. Finishing documents

### **Week 2 (Nov 6th - Nov 12th)**

1. Finish data handling & populate data structure

2. Decide function signatures

3. Implement Police Training Simulator with DFS

4. Setup test case for data handling and DFS

### **Week 3 (Nov 13th - Nov 19th)**

Note: Mid-Project Checkin (Nov 14-18)

1. Test case for DFS

2. Optimum route for chasing criminals

3. Identify the importance of places in the city

4. Implement PNG output

### **Week 4 (Nov 20th - Nov 26th)**

1. Test case for the importance of places

2. Finding the next best position for a new police station

3. Implement GIF output

4. Integrate image output into existing features

### **Week 5 (Nov 27th - Dec 3rd)**

1. Finish any leftovers from previous weeks

2. Record final presentation video

### **Week 6 (Dec 4th - Dec 8th)**

1. Finalize README.md

2. Final lint/other checks

