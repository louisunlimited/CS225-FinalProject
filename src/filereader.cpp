#include "filereader.h"

vector<Coord> FileReader::readNormalizedNode(string fileName) {
    vector<Coord> nodes;
    return nodes;
}

vector<Coord> FileReader::convertNode(vector<Coord> normalizedCoords,
    Coord anchor1, Coord normalizedAnchor1,
    Coord anchor2, Coord normalizedAnchor2) {

    vector<Coord> nodes;
    return nodes;
}

// Jiang Hezi
vector<std::pair<int, int>> FileReader::readEdge(string fileName) {
    vector<std::pair<int, int>> edges;
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
        edges.push_back(std::pair<int, int>(startnode, endnode));
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
                lastindex --;
            } else {
                break;
            }
        }
        lastindex ++;
        double latitude = 0.0;
        double longtitude = 0.0;
        for (unsigned int i = lastindex; i < line.size(); i++) {
            if (line[i] == ',') {
                latitude = stod(line.substr(lastindex, i - lastindex));
                lastindex = i + 2;
                longtitude = stod(line.substr(lastindex, line.size() - 2 - lastindex));
                break;
            }
        } // create new coord object
        Coord policestation;
        policestation.lat_ = latitude;
        policestation.long_ = longtitude;
        
        police.push_back(policestation);
    }
    return police;
}