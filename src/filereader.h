#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "coord.h"

using namespace std;

class FileReader {
    public:
        /**
         * Read data from the node file into a vector of normalized coordinates.
         *
         * @param fileName The relative path to the target file
         */
        static vector<Coord> readRawNode(string fileName);

        /**
         * Convert normalized data into actual coordinates.
         *
         * @param normalizedCoords Normalized coordinates
         * @return Actual coordinates
         */
        static void convertNode(vector<Coord>& normalizedCoords,
            Coord anchor1, Coord normalizedAnchor1,
            Coord anchor2, Coord normalizedAnchor2);

        /**
         * Read data from the edge file into a vector of Tuples (start node, end node, road length).
         * Road length is converted into km.
         *
         * @param fileName The relative path to the target file
         */
        static vector<pair<int, int>> readEdge(string fileName);

        /**
         * Read data from the police station file into a vector of coordinates.
         *
         * @param fileName The relative path to the target file
         */
        static vector<Coord> readPoliceStation(string fileName);
};
