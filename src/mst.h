#pragma once

#include <functional>
#include <iostream>
#include <vector>
#include <queue>
#include "coord.h"

class MST {
    private:
        struct MSTNode {
            int index;
            Coord coord;
            MSTNode* parent;
            MSTNode(int index, Coord coord) {
                this->index = index;
                this->coord = coord;
            }
        };
    public:
        /**
         * Empty constructor. Do not use it.
         */
        MST();
        /**
         * @brief Construct a new MST object
         * 
         * @param coords The vector of points to build your MST off of.
         * @param dist The distance function to use.
         */
        MST(const vector<pair<Coord, int>>& coords, const vector<pair<int, int>>& edges, function<double(const Coord&, const Coord&)> dist);

        /**
         * @brief Finds the minimum edge for a node
         * 
         * @param node The node to find the minimum edge for
         * @return pair<MSTNode*, double> The minimum edge and its weight
        */
        MSTNode* findMinEdge(MSTNode* node);

        /**
         * @brief Make MST using Prim's algorithm
         * 
         * @param start The starting node
         * 
         * @return vector<pair<int, int>> The edges of the MST
         */
        vector<pair<int, int>> primMST(int start);

        /**
         * @brief Helper for MST to search destination node given a direction as a node
         * 
         * @param startNode as the starting node
         * @param dirNode as the first node on a path
         * 
         * @return vector<MSTNode*> as all the nodes along the way
         */
        vector<MSTNode*> findValidNode(MSTNode* startNode, MSTNode* dirNode);

        /**
         * @brief Helper for calculating dist between start and end of a route
         * 
         * @param startRoute as the node we are going to
         * 
         * @return double as all the directions 
         */
        double findDistance(vector<MSTNode*> startRoute);

    private:
        /**
         * @brief Adjacency list representation of the graph
         */
        vector<vector<pair<double, MSTNode*>>> adjList;

        /**
         * @brief The distance function to use
         */
        function<double(const Coord&, const Coord&)> _dist;

        /**
         * @brief The vector of points to build your MST off of.
         */
        vector<MSTNode> _coords;
};