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
        MST(const vector<pair<Coord, int>>& coords, const vector<vector<int>>& adjList, function<double(const Coord&, const Coord&)> dist);

        /**
         * Find the number of nodes in the graph
         *
         * @return The number of nodes
         */
        int size() const;

        /**
         * @brief Make MST using Prim's algorithm
         * 
         * @param start The starting node
         * 
         * @return vector<pair<int, int>> The edges of the MST
         */
        vector<pair<int, int>> primMST(int start) const;

        /**
         * @brief Helper for MST to search destination node given a direction as a node
         * 
         * @param startNode as the starting node
         * @param dirNode as the first node on a path
         * 
         * @return vector<MSTNode*> as all the nodes along the way
         */
        vector<const MSTNode*> findCompleteRoute(const MSTNode* startNode, const MSTNode* dirNode) const;

        /**
         * @brief Helper for calculating dist between start and end of a route
         * 
         * @param route the route from start to end
         * 
         * @return sum of the distance of each segment
         */
        double findDistance(const vector<const MSTNode*>& route) const;

    private:
        /**
         * @brief Adjacency list representation of the graph
         */
        vector<vector<MSTNode*>> _adjList;

        /**
         * @brief The distance function to use
         */
        function<double(const Coord&, const Coord&)> _dist;

        /**
         * @brief The vector of points to build your MST off of.
         */
        vector<MSTNode> _nodes;

        /**
         * @brief A vecotr of nodes with deg != 2
         */
        vector<MSTNode*> _mstNodes;
};
