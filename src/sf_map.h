#pragma once

#include <algorithm>
#include <cmath>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <queue>
#include <limits.h>
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
        void addPoliceStation(const Coord& coord);

        /**
         * Return a map of the city of San Francisco. 5000 pixels represents 1 degree
         * The color pickers are closures that takes the node(s) index as input and output the
         * color as a rgbaColor object
         *
         * NOTE: if the alpha value of the rgbaColor object is 0, then the node/edge will not be
         * shown. Otherwise the alpha value does not have an effect.
         *
         * @param zoom The zoom factor between 1.0 and 20.0, inclusive
         * @param center The center point to zoom on
         * @param nodeColor The color picker for coloring nodes
         * @param edgeColor The color picker for coloring edges
         */
        PNG drawMap(double zoom, const Coord& center, function<rgbaColor(int)> nodeColor,
            function<rgbaColor(int, int)> edgeColor) const;

        /**
         * Return a map of San Francisco without zooming
         */
        PNG drawMap(function<rgbaColor(int)> nodeColor, function<rgbaColor(int, int)> edgeColor) const;


        // Four main functions

        /**
         * 1. Identify the importance of places in the city:
         *
         * Dijkstra's algorithm will be applied to every node to find all shortest paths. Then for
         * every point, we will calculate the betweenness centrality as its importance value.
         *
         * @param color RGB value of the output image
         * @return A colored PNG containing the map of the entire San Francisco
         */
        PNG importance(const rgbaColor& color);

        /**
         * Helper for 1.
         *
         * @return A list of importance of each node
         */
        vector<double> importanceAsVec();

        /**
         * 2. Emergency Contact Access Point:
         *
         * @return A colored PNG containing the map of the entire San Francisco
         */
        PNG accessPoint() const;

        /**
         * Potential Helper for 2?
         *
         * @return 
         */

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
        Animation escapeRoute(const Coord& start, double minDist) const;

        /**
         * Helper for 3.
         *
         * @return A list of nodes (including both ends) representing the escape route
         */
        vector<int> escapeRouteAsVec(const Coord& start, double minDist) const;

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
        PNG nextPoliceStation(double zoom) const;

        /**
         * Helper for 4.
         *
         * @return A integer representing the index of the best node for the new police station
         */
        int nextPoliceStationAsIndex(Coord start) const;


        // Functions used exclusively for testing

        /**
         * Set the "constants" for drawing the map
         */
        void setScale(double scale);
        void setMargin(double margin);
        void setRadius(double radius);
        void setLineWidth(double lineWidth);

        /**
         * For testing
         *
         * @return A vector of parents between start and all other nodes
         *  e.g. result[i] = the node before the ith node on the shortest path between start and
         *  the ith nodes.
         */
        vector<int> getParents(int start) const;

        /**
         * For testing
         *
         * @return A vector of distance between start and all other nodes
         *  e.g. result[i] = distance of shortest path between start and the ith nodes.
         */
        vector<double> getDistances(int start) const;

        /**
         * For testing
         * [start = new police station]
         * @return A pair of distances between all points and police stations and an 
         *         integer representing the index of the point for the furtherest police station.
         */
        pair<vector<double>, int> getEccentricity(int start) const;

    private:
        /* Coordinates */
        vector<MapNode> _nodes;
        /* Adjacency list */
        vector<vector<MapNode*>> _neighbors;
        /* Police stations */
        vector<MapNode*> _police;
        /* Metric for measuring distance */
        function<double(const Coord&, const Coord&)> _dist;
        /* K-d tree */
        KDTree tree;
        /* Map range */
        double _minLat;
        double _maxLat;
        double _minLong;
        double _maxLong;

        // CONSTANTS FOR DRAW MAP
        // NOTE: some const qualifiers are removed so that they can be adjusted for
        //       better visualization of certain testcases
        /* Pixels per degree */
        double SCALE = 1500;
        /* Maximum zoom factor */
        const double MAX_ZOOM = 15;
        /* Map margin (in degree) */
        double MARGIN = 0.01;
        /* Radius of node */
        double RADIUS = 1.2;
        /* Width of edge */
        double LINE_WIDTH = 1;

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
        vector<bool> getValidSubset() const;

        /**
         * Helper for the above helper.
         * Given a connected graph, remove the least number of nodes such that
         *  1) remaining graph is still connected
         *  2) all nodes lies in a circle of diameter 1000 km
         */
        void getValidSubsetHelper(vector<bool>& validPoints) const;

        /**
         * Cleans up the data according to the return value of `getValidSubset`. It removes
         * all invalid nodes and the edges connected to them.
         *
         * @param validPoints A vector of boolean values representing whether each node is valid
         */
        void cleanData(const vector<bool>& validPoints);

        /**
         * Helpers for drawMap.
         */
        Coord coord2Pixel(const Coord& coord, const Coord& lowerLeft, double zoom) const;
        void drawCircle(PNG& image, const Coord& center, double radius,
            const rgbaColor& color) const;
        void drawLine(PNG& image, const Coord& start, const Coord& end, double width,
            const rgbaColor& color) const;
        void colorPixel(HSLAPixel& pixel, const rgbaColor& color, double percentage) const;

        /**
         * Helper for 3.
         */
        bool findRoute(vector<int>& currNodes, double remainDist, vector<bool>& visited) const;
};
