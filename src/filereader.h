#pragma once;

#include <string>
#include <tuple>
#include <vector>

using namespace std;

class FileReader {
    public:
        /**
         * Read data from the node file into a vector of coordinates.
         *
         * @param fileName The relative path to the target file
         */
        static vector<Coord> readNode(string fileName);

        /**
         * Read data from the edge file into a vector of Tuples (start node, end node, road length).
         * Road length is converted into km.
         *
         * @param fileName The relative path to the target file
         */
        static vector<tuple<int, int, double>> readEdge(string fileName);

        /**
         * Read data from the police station file into a vector of coordinates.
         *
         * @param fileName The relative path to the target file
         */
        static vector<Coord> readPoliceStation(string fileName);
};
