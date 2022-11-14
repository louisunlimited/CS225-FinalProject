#include <algorithm>
#include <stack>
#include "sf_map.h"

// Cao Tianyue
SFMap::SFMap(const vector<Coord>& nodes, const vector<pair<int, int>>& edges) {
    // The validation criteria is slightly modified. The detailed description is in the header
    // files next to the `getValidSubset` method.

    int n = nodes.size();
    if (n == 0) throw std::invalid_argument("There should be at least one node in the data");

    // Populate the nodes (and update min & max coordinates)
    for (int i = 0; i < n; i++) {
        _nodes.push_back(SFMap::MapNode(i, nodes.at(i), false));
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

SFMap::SFMap(const vector<Coord>& nodes, const vector<pair<int, int>>& edges,
    const vector<Coord>& police): SFMap(nodes, edges) {

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

int SFMap::size() const {
    return _nodes.size();
}

vector<SFMap::MapNode*> SFMap::escapeRouteAsVec(Coord start, double minDist) {
    // find start node
    SFMap::MapNode* startNode = &_nodes[tree.search(start)];
    // check if start node is police station
    if (startNode->isPoliceStation == true) {
        throw runtime_error("Never start at the police station.");
    }
    // set DFS path
    vector<SFMap::MapNode*> currNodes;
    unordered_map<int, bool> visited;
    if (!findRoute(currNodes, minDist, visited)) {
        throw runtime_error("Cannot find escape route");
    }
    return currNodes;
}

bool SFMap::findRoute(vector<SFMap::MapNode*>& currNodes, double remainDist, unordered_map<int, bool>& visited) {
    if (remainDist <= 0) {
        return true;
    }
    SFMap::MapNode* lastNode = currNodes[currNodes.size() - 1];
    for (auto neighbor: _neighbors[lastNode->index]) {
        if (neighbor->isPoliceStation || visited.find(neighbor->index) != visited.end()) {
            continue;
        }
        visited[neighbor->index] = true;
        SFMap::MapNode* newNode = &_nodes[neighbor -> index];
        currNodes.push_back(newNode);
        remainDist -= dist(newNode->coord, lastNode->coord);
        if (findRoute(currNodes, remainDist, visited)) {
            return true;
        }
        lastNode = currNodes[currNodes.size() - 1];
        delete lastNode;
        currNodes.pop_back();
    }
    return false;
}

// Helpers
vector<bool> SFMap::getValidSubset() {
    int n = _nodes.size();
    // The result
    // validPoints[i] == true  <==>  node i is in the valid subset
    vector<bool> validPoints = vector(n, false);

    // Use DFS to find connected components
    // parent[i] == -1  <==>  node i has not been visited
    // parent[i] == parent[j] != -1  <==>  node i & j connected
    vector<int> parent = vector(n, -1);
    for (int i = 0; i < n; i++) {
        if (parent[i] != -1) continue;

        // DFS starting at node i
        int count = 1;
        parent[i] = i;
        stack<int> s;
        s.push(i);
        while (!s.empty()) {
            int curr = s.top();
            s.pop();
            for (SFMap::MapNode* nextNode : _neighbors[curr]) {
                int next = nextNode->index;
                if (parent[next] == -1) {
                    parent[next] = i;
                    count++;
                    s.push(next);
                }
            }
        }

        // If a connected component with at least 90% total size is found, return the largest
        // subset satisfying the conditions specified in the header file.
        if (count >= 0.9 * n) {
            for (int j = 0; j < n; j++) {
                validPoints[j] = parent[j] == i;
            }
            getValidSubsetHelper(validPoints);
            return validPoints;
        }
    }

    // If all connected components are too small, return an empty result
    return validPoints;
}

void SFMap::getValidSubsetHelper(vector<bool>& validPoints) {
    // I don't know how to implement this
    // The time complexity will likely go above O(|V|^2)
    return;
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
