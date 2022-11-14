#include "filereader.h"

vector<Coord> FileReader::readRawNode(string fileName) {
    vector<Coord> nodes;
    fstream file(fileName);
    string line;
    while (getline(file, line)) {
        string long_, lat_;
        // split by space and get the last two elements
        istringstream iss(line);
        for (int i = 0; i < 2; i++) {
            iss >> long_;
        }
        iss >> lat_;
        Coord node;
        node.long_ = stod(long_);
        node.lat_ = stod(lat_);
        nodes.push_back(node);
    }
    return nodes;
}

void FileReader::convertNode(vector<Coord>& normalizedCoords,
    Coord anchor1, Coord normalizedAnchor1,
    Coord anchor2, Coord normalizedAnchor2) {
        // calculate the scale factor
        double scaleFactor = sqrt(pow(anchor1.long_ - anchor2.long_, 2) + pow(anchor1.lat_ - anchor2.lat_, 2)) /
            sqrt(pow(normalizedAnchor1.long_ - normalizedAnchor2.long_, 2) + pow(normalizedAnchor1.lat_ - normalizedAnchor2.lat_, 2));
        // calculate the offset
        double offsetX = anchor1.long_ - normalizedAnchor1.long_ * scaleFactor;
        double offsetY = anchor1.lat_ - normalizedAnchor1.lat_ * scaleFactor;
        // convert normalized coordinates into actual coordinates
        vector<Coord> coords;
        for (Coord coord : normalizedCoords) {
            Coord actualCoord;
            actualCoord.long_ = coord.long_ * scaleFactor + offsetX;
            actualCoord.lat_ = coord.lat_ * scaleFactor + offsetY;
            coords.push_back(actualCoord);
        }
        return coords;
}

// Jiang Hezi
vector<pair<int, int>> FileReader::readEdge(string fileName) {
    vector<pair<int, int>> edges;
    return edges;
}

vector<Coord> FileReader::readPoliceStation(string fileName) {
    vector<Coord> police;
    return police;
}
