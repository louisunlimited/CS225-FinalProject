#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "cs225/PNG.h"
#include "cs225/RGB_HSL.h"
#include "animation.h"
#include "coord.h"
#include "kdtree.h"


using namespace std;

class SFMap {
    private:
        struct MapNode {
            int index;
            Coord coord;
            bool isPoliceStation;
            MapNode(int index, Coord coord, bool isPoliceStation) {
                this->index = index;
                this->coord = coord;
                this->isPoliceStation = isPoliceStation;
            }
        };

    public:
        /**
         * Constructor. Checks if the given data is valid. An error is raised if at least half
         * of the nodes are invalid.
         *
         * @param nodes The node data
         * @param edges The edge data
         */
        SFMap(const vector<Coord>& nodes, const vector<pair<int, int>>& edges);

        /**
         * Constructor. Same as above but contains police station data.
         *
         * @param nodes The node data
         * @param edges The edge data
         * @param police Coordinates of the police station
         */
        SFMap(const vector<Coord>& nodes, const vector<pair<int, int>>& edges, const vector<Coord>& police);

        /**
         * Find the number of nodes in the graph
         *
         * @return The number of nodes
         */
        int size() const;

        /**
         * Add a new police station. Snap to nearest node. Ignore if it is at the same location
         * with an existing one.
         *
         * @param coord The coordinate of the new police station
         */
        void addPoliceStation(Coord coord);

        /**
         * Return a map of the city of San Francisco. 5000 pixels represents 1 degree
         */
        cs225::PNG drawMap(double zoom);

        /**
         * 1. Identify the importance of places in the city:
         *
         * Dijkstra's algorithm will be applied to every node to find all shortest paths. Then for
         * every point, we will calculate the betweenness centrality as its importance value.
         *
         * @param color RGB value of the output image
         * @return A colored PNG containing the map of the entire San Francisco
         */
        cs225::PNG importance(cs225::rgbaColor color);

        /**
         * Helper for 1.
         * @return A list of importance of each node
         */
        vector<double> importanceAsVec();

        /**
         * 2. Optimum route for chasing criminals:
         *
         * First, k-d tree is used to find the nearest node to the two given locations. Then
         * we apply A* search algorithm to find the shortest path between the two nodes.
         *
         * @param start starting coordinate
         * @param end destination coordinate
         * @param zoom zoom factor of the PNG
         * @return A colored PNG containing the map of the entire San Francisco
         */
        cs225::PNG shortestPath(Coord start, Coord end, double zoom);

        /**
         * Helper for 2.
         * @return A list of nodes (including both ends) representing the path
         */
        vector<MapNode*> shortestPathAsVec(Coord start, Coord end);

        /**
         * 3. Police training simulator
         *
         * Depth-First-Search will be used to select the path for the criminal
         * through a graph. The nodes will be coordinates, and the edges correspond
         * to the length of the road between them.
         *
         * @param start starting coordinate
         * @param minDist minimum distance the criminal escapes (in km)
         * @return An animation of the escaping criminal
         */
        Animation escapeRoute(const Coord& start, double minDist);

        /**
         * Helper for 3.
         * @return A list of nodes (including both ends) representing the escape route
         */
        vector<MapNode*> escapeRouteAsVec(const Coord& start, double minDist);

        /**
         * 4. Finding the next best position for a new police station
         *
         * We will traverse all the node one by one and regard each as a potential police
         * station, then calculate the maximum distance for the police to reach any node on
         * the graph using Dijkstra's algorithm. We aim to find the best solution that gives
         * the minimum of such distance.
         *
         * @param start starting coordinate
         * @param zoom zoom factor of the PNG
         * @return A PNG with highlighted location for the next possible police station
         */
        cs225::PNG nextPoliceStation(double zoom);

        /**
         * Helper for 4.
         * @return A node pointer representing the next best location for the new police station
         */
        MapNode* nextPoliceStationAsCoord(Coord start);

    private:
        /* Coordinates */
        vector<MapNode> _nodes;
        /* Adjacency list */
        vector<vector<MapNode*>> _neighbors;
        /* Police stations */
        vector<MapNode*> _police;
        /* K-d tree */
        KDTree tree;
        /* Map range */
        double _min_lat;
        double _max_lat;
        double _min_long;
        double _max_long;

        // CONSTANTS
        /* Map margin (in pixel) */
        const double MARGIN = 80;
        /* Radius of node */
        const double RADIUS = 5;
        /* Width of edge */
        const double WIDTH = 5;

        // HELPER FUNCTIONS
        /**
         * The current data is valid if there exists a subset of the nodes such that
         *  1) has size at least 90% of the total nodes
         *  2) all nodes lies in a circle of diameter 1000 km
         *  3) is a connected graph
         * The subset with the largest size will be returned as a valid subset.
         *
         * @return A vector of boolean values representing whether each node is valid
         */
        vector<bool> getValidSubset();

        /**
         * Helper for the above helper.
         * Given a connected graph, remove the least number of nodes such that
         *  1) remaining graph is still connected
         *  2) all nodes lies in a circle of diameter 1000 km
         */
        void getValidSubsetHelper(vector<bool>& validPoints);

        /**
         * Cleans up the data according to the return value of `getValidSubset`. It removes
         * all invalid nodes and the edges connected to them.
         *
         * @param validPoints A vector of boolean values representing whether each node is valid
         */
        void cleanData(const vector<bool>& validPoints);

        // helper for 3
        bool findRoute(vector<SFMap::MapNode*>& currNodes, double remainDist, vector<bool>& visited);
};
