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

        Coord policestation;
        policestation.lat_ = latitude;
        policestation.long_ = longtitude;
        police.push_back(policestation);
    }
    return police;
}