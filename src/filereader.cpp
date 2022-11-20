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
    double anchor1Long = anchor1.long_;
    double anchor1Lat = anchor1.lat_;
    double anchor2Long = anchor2.long_;
    double anchor2Lat = anchor2.lat_;
    double normalizedAnchor1Long = normalizedAnchor1.long_;
    double normalizedAnchor1Lat = normalizedAnchor1.lat_;
    double normalizedAnchor2Long = normalizedAnchor2.long_;
    double normalizedAnchor2Lat = normalizedAnchor2.lat_;

    double k = (anchor2Lat - anchor1Lat) / (normalizedAnchor2Lat - normalizedAnchor1Lat);
    double k1 = (anchor2Long - anchor1Long) / (normalizedAnchor2Long - normalizedAnchor1Long);

    for (auto& coord : normalizedCoords) {
        coord.long_ = k1 * (coord.long_ - normalizedAnchor1Long) + anchor1Long;
        coord.lat_ = k * (coord.lat_ - normalizedAnchor1Lat) + anchor1Lat;
    }
}

vector<pair<int, int>> FileReader::readEdge(string fileName) {
    vector<pair<int, int>> edges;
    ifstream read_file(fileName);
	string line;
    
	while(getline(read_file, line)) {
        // each line of SF.cedge.txt get √
        // split the content by space 只要中间俩 
        int startnode = 0; // 1
        int endnode = 0; // 2
        int count = 0; // 1, 2
        int lastindex = 0; // + 1

        for (unsigned int i = 0; i < line.size(); i++) {
            if (line[i] == ' ') {
                if (count == 1) {
                    startnode = stoi(line.substr(lastindex, i - lastindex));
                } else if (count == 2) {
                    endnode = stoi(line.substr(lastindex, i - lastindex));
                }
                count++;
                lastindex = i + 1;
            }
        }
        edges.push_back(pair<int, int>(startnode, endnode));
	}
    return edges;
}

vector<Coord> FileReader::readPoliceStation(string fileName) {
    vector<Coord> police;

    ifstream read_file(fileName);
    string line;
    getline(read_file, line);
    while (getline(read_file, line)) {
        unsigned int lastindex = line.size() - 1;
        while (lastindex > 0) {
            if (line[lastindex] != '(') {
                lastindex--;
            } else {
                break;
            }
        }
        lastindex++;
        double latitude = 0.0;
        double longtitude = 0.0;
        for (unsigned int i = lastindex; i < line.size(); i++) {
            if (line[i] == ',') {
                latitude = stod(line.substr(lastindex, i - lastindex));
                lastindex = i + 2;
                longtitude = stod(line.substr(lastindex, line.size() - 2 - lastindex));
                break;
            }
        }

        police.push_back(Coord(latitude, longtitude));
    }
    return police;
}
