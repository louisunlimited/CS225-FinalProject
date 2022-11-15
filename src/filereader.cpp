#include "filereader.h"

using namespace std;

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

    double x1n = anchor1.long_;
    double y1n = anchor1.lat_;
    double x2n = anchor2.long_;
    double y2n = anchor2.lat_;
    double x1 = normalizedAnchor1.long_;
    double y1 = normalizedAnchor1.lat_;
    double x2 = normalizedAnchor2.long_;
    double y2 = normalizedAnchor2.lat_;

    double a = (y2 - y1) / (x2 - x1);
    double b = y1 - a * x1;
    double an = (y2n - y1n) / (x2n - x1n);
    double bn = y1n - an * x1n;

    for (Coord& coord : normalizedCoords) {
        double x = coord.long_;
        double y = coord.lat_;
        double xn = (x - x1) / (x2 - x1) * (x2n - x1n) + x1n;
        double yn = (y - y1) / (y2 - y1) * (y2n - y1n) + y1n;
        coord.long_ = xn;
        coord.lat_ = yn;
        cout << coord.long_ << " " << coord.lat_ << endl;
    }

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
