#include <algorithm>
#include "sf_map.h"

// Cao Tianyue
SFMap::SFMap(vector<Coord> nodes, vector<tuple<int, int>> edges) {
    // The validation criteria is slightly modified. The detailed description is in the header
    // files next to the `getValidSubset` method.

    int n = nodes.size();
    if (n == 0) throw std::invalid_argument("There should be at least one node in the data");

    // Populate the nodes (and update min & max coordinates)
    for (int i = 0; i < n; i++) {
        _nodes.push_back(SFMap::MapNode(i, nodes[i], false));
    }

    // Populate the edges
    _neighbors = vector(n, vector<SFMap::MapNode*>());
    for (auto [i, j] : edges) {
        if (find(_neighbors[i].begin(), _neighbors[i].end(), &_nodes[j]) == _neighbors[i].end()) {
            _neighbors[i].push_back(&_nodes[j]);
            _neighbors[j].push_back(&_nodes[i]);
        }
    }

    // Filter out the non-valid points
    vector<bool> validPoints = getValidSubset();
    cleanData(validPoints);

    // Find max and min coordinates
    _min_lat = _max_lat = _nodes[0].coord.lat_;
    _min_long = _max_long = _nodes[0].coord.long_;
    for (const SFMap::MapNode& node : _nodes) {
        _min_lat = min(_min_lat, node.coord.lat_);
        _max_lat = max(_max_lat, node.coord.lat_);
        _min_long = min(_min_long, node.coord.long_);
        _max_long = max(_max_long, node.coord.long_);
    }

    // Construct KDTree
    vector<pair<Coord, int>> coords;
    for (const SFMap::MapNode& node : _nodes) {
        coords.push_back(pair(node.coord, node.index));
    }
    tree = KDTree(coords, dist);
}

SFMap::SFMap(vector<Coord> nodes, vector<tuple<int, int>> edges, vector<Coord> police): SFMap(nodes, edges) {
    for (Coord coord : police) {
        // Find nearest node to the police station
        int index = tree.search(coord);
        if (index == -1) continue;
        SFMap::MapNode* node = &_nodes[index];
        if (!node->isPoliceStation) {
            node->isPoliceStation = true;
            _police.push_back(node);
        }
    }
}

// Sun Xiping
vector<SFMap::MapNode*> SFMap::escapeRouteAsVec(Coord start, double minDist) {
    vector<SFMap::MapNode*> path;
    return path;
}

// Helpers
vector<bool> SFMap::getValidSubset() {
    // TODO: finish this function
    vector<bool> validPoints;
    return validPoints;
}

void SFMap::cleanData(const vector<bool>& validPoints) {
    // Count number of valid points. Throw exception if no valid points.
    int n = 0;
    for (bool isValid : validPoints) if (isValid) n++;
    if (n == 0) throw std::invalid_argument(
        "The data received contains an insufficient number of valid nodes");

    // Clean adjacency list
    for (int i = n - 1; i >= 0; i--) {
        // Remove entire adjacency vector if i is invalid
        if (!validPoints[i]) {
            _neighbors.erase(_neighbors.begin() + i);
            continue;
        }
        // Otherwise check if each adjacent node is invalid
        for (int j = _neighbors[i].size() - 1; j >= 0; j--) {
            if (!validPoints[_neighbors[i][j]->index]) {
                _neighbors[i].erase(_neighbors[i].begin() + j);
            }
        }
    }

    // Clean nodes
    int index = n - 1;
    for (int i = _nodes.size(); i >= 0; i--) {
        if (!validPoints[_nodes[i].index]) {
            _nodes.erase(_nodes.begin() + i);
        } else {
            _nodes[i].index = index--;
        }
    }
}
