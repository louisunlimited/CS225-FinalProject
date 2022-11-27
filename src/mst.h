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
        MST(const vector<pair<Coord, int>>& coords, function<double(const Coord&, const Coord&)> dist);

        /**
         * @brief Make MST using Prim's algorithm
         * 
         * @param start The starting node
         * @return vector<pair<int, int>> The MST represented as a vector of edges of the form (parent, child)
         */
        vector<pair<int, int>> makeMST(int start);

        /**
         * @brief print the MST
         */
        void printMST();


    private:
        /**
         * @brief Adjaency list representation of the graph
         */
        vector<vector<MSTNode*>> adjList;

        /**
         * @brief The distance function to use
         */
        function<double(const Coord&, const Coord&)> _dist;

        /**
         * @brief The vector of points to build your MST off of.
         */
        vector<MSTNode*> _coords;
};