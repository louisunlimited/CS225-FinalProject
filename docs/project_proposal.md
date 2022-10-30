# Project Proposal

## Leading Question

We have acquired a dataset on the San Fransico Road Network, and we are thinking about developing a project to help police cope with crime activities by providing efficient ways(routes) to chase criminals. Our project can be used to achive the following goals:
THis isin BFS
1. Identify Busiest Road in San Fransico (or best 10):

2. Optimum route for chasing criminals:

3. Optimum route for searching a specific area:

4. Finding the next best position for a police station: (KD-Tree?)

## Dataset Acquisition

1. Data Format:

    We are using [San Francisco Road Network Data](https://www.cs.utah.edu/~lifeifei/SpatialDataset.htm) and working with the following two datasets:

    - [San Francisco Road Network's Nodes](https://www.cs.utah.edu/~lifeifei/research/tpq/SF.cnode)

    - [San Francisco Road Network's Edges](https://www.cs.utah.edu/~lifeifei/research/tpq/SF.cedge)

    Additionally, we added [San Francisco Police Station data](https://data.sfgov.org/Public-Safety/Map-of-Police-Stations-2011-/a9xm-jdem) to our existing dataset for reference.

    For the first two dataset on `San Fransico Road Network`, the dataset is in `.txt` file so we can directly read from them.

    For the `police station data`, the dataset is in `.csv` and we can directly read as well.

    The data we have on road network in rather large, with approximatly 170,000 Nodes and 220,000 Edges. Whilest we only have roughly 10 police department data. As for now, we do plan to use all data acquired.

2. Data Correction:

    We are going to check whether the input is a `.csv` or a `.txt` and we will parse the data accordingly. We don't think there will be any major errors present in our dataset, but we will do some prelimilary checks for GPS Coordinates and else. (weight >= absolute distance between two points) (makesure a weight is present) (no two weight present for same set of nodes)

3. Data Storage:

    For our mapdata, we will be storing them in a graph wiht an Adjacency List. Which would take O(E + V) space. And we might use KD-tree to find the nearest neighour, which would take O(Nlog(N)), where N is the number of nodes.

## Graph Algorithms

1. _____ for Betweeness Centrality

    Input: A graph of Mapdata | None

    Output: A PNG containing the marked position. (or all position with different values(colors) for each point)

    Function Effiency: "Floyd Marshall": O(|V|^3) or "Johnson's" : O(|V|^2 log(|V|) + |V||E|)

2. A* Path Finding

    Input: current \ destination coordinate | None | (zoom factor)

    Output: A PNG containing the marked path. (possibly zoomed)

    Function Effiency: Probably approaching O(V)

3. BFS

    Input: A graph of Mapdata + current coordinate | None | (zoom factor)

    Output: A PNG containing all the possible points availavble to reach. (possibly zoomed)

    Function Effiency: O(V)

4. _____ for finding police station (Dijkstra's?)

    Input: A graph of Mapdata | None

    Output: A PNG with marked positions for the next possible police station.

    Function Effiency: O(V)

## Timeline

    11.4 - 11.14~18 - Dec 8

    10.31 - 11.3: Finish project proposal
    11.4 - 11.7: Finish data handling methods, populate data stucture.
    11.7 - 11.13: Finish algo for one task (without graph).
    11.14 - Dec.8: Finish everything according to tasks. Each taking about 1.2 week.
