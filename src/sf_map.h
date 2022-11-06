#pragma once;

#include <string>
#include <tuple>
#include <vector>
#include "cs225/PNG.h"
#include "cs225/RGB_HSL.h"
#include "kdtree.h"

using namespace std;

class SFMap {
    public:
        /**
         * Constructor. Checks if the given data is valid. An error is raised if at least half
         * of the nodes are invalid.
         *
         * @param nodes The node data
         * @param edges The edge data
         */
        SFMap(vector<Coord> nodes, vector<tuple<int, int, double>> edges);

        /**
         * Constructor. Same as above but contains police station data.
         *
         * @param nodes The node data
         * @param edges The edge data
         * @param police Coordinates of the police station
         */
        SFMap(vector<Coord> nodes, vector<tuple<int, int, double>> edges, vector<Coord> police);

        /**
         * Add a new police station. Snap to nearest node. Ignore if it is at the same location
         * with an existing one.
         *
         * @param coord The coordinate of the new police station
         */
        Coord addPoliceStation(Coord coord);

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
        std::vector<double> importanceAsVec();

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
        std::vector<int> shortestPathAsVec(Coord start, Coord end);

        /**
         * 3. Police training simulator
         *
         * Depth-First-Search will be used to select the path for the criminal
         * through a graph. The nodes will be coordinates, and the edges correspond
         * to the length of the road between them.
         *
         * @param start starting coordinate
         * @return An animation of the escaping criminal
         */
        Animation escapeRoute(Coord start);

        /**
         * Helper for 3.
         * @return A list of nodes (including both ends) representing the escape route
         */
        std::vector<int> escapeRouteAsVec(Coord start);

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
         * @return A nodes representing the next best location for the new police station
         */
        Coord nextPoliceStationAsCoord(Coord start);

    private:
        /* Coordinates */
        vector<Coord> _coords;
        /* Adjacency list */
        vector<vector<int>> _neighbors;
        /* K-d tree */
        // KDTree tree;
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
};
