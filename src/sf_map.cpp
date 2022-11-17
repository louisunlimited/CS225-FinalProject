#include "sf_map.h"

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
    _minLat = _maxLat = _nodes[0].coord.lat_;
    _minLong = _maxLong = _nodes[0].coord.long_;
    for (const MapNode& node : _nodes) {
        _minLat = min(_minLat, node.coord.lat_);
        _maxLat = max(_maxLat, node.coord.lat_);
        _minLong = min(_minLong, node.coord.long_);
        _maxLong = max(_maxLong, node.coord.long_);
    }

    // Construct KDTree
    vector<pair<Coord, int>> coords;
    for (const MapNode& node : _nodes) {
        coords.push_back(pair(node.coord, node.index));
    }
    tree = KDTree(coords, dist);
}

SFMap::SFMap(const vector<Coord>& nodes, const vector<pair<int, int>>& edges,
    const vector<Coord>& police): SFMap(nodes, edges) {

    for (Coord coord : police) {
        addPoliceStation(coord);
    }
}

int SFMap::size() const {
    return _nodes.size();
}

void SFMap::addPoliceStation(const Coord& coord) {
    // Find nearest node to the police station
    int index = tree.search(coord);

    MapNode* node = &_nodes[index];
    if (!node->isPoliceStation) {
        node->isPoliceStation = true;
        _police.push_back(node);
    }
}

cs225::PNG SFMap::drawMap(double zoom, const Coord& center, bool drawLines) const {
    // Lat. and long. increases in the  direction shown below:
    //               ↑ (lat)
    //               |
    //               +------> (long)

    // Check for invalid inputs
    if (zoom < 1) {
        throw std::invalid_argument("Zoom factor must be at least 1.0");
    }

    int height = (_maxLat - _minLat) * SCALE;
    int width = (_maxLong - _minLong) * SCALE;

    // Determine the lat. and long. of the borders after the map is zoomed
    double centerLat = min(max(center.lat_, _minLat), _maxLat);
    double centerLong = min(max(center.long_, _minLong), _maxLong);
    double zMinLat = (1 / zoom) * _minLat + (1 - 1 / zoom) * centerLat;
    double zMinLong = (1 / zoom) * _minLong + (1 - 1 / zoom) * centerLong;

    cs225::PNG image(width + 2 * MARGIN, height + 2 * MARGIN);
    cs225::rgbaColor black{ 0, 0, 0, 255 };

    // Draw points
    for (const MapNode& node : _nodes) {
        drawCircle(image, node.coord, RADIUS, black);
    }

    if (!drawLines) {
        return image;
    }

    // Draw lines
    for (int i = 0; i < size(); i++) {
        const MapNode& node = _nodes[i];
        for (const MapNode* neighbor : _neighbors[i]) {
            if (neighbor->index > i) {
                drawLine(image, node.coord, neighbor->coord, LINE_WIDTH, black);
            }
        }
    }
    return image;
}

cs225::PNG SFMap::drawMap(bool drawLines) const {
    return drawMap(1, Coord(0, 0), drawLines);
}

vector<double> SFMap::importanceAsVec() {
    // TODO
    vector<double> result;
    return result;
}

vector<double> SFMap::getParents(int start) const {
    // TODO
    vector<double> parents;
    return parents;
}

vector<int> SFMap::escapeRouteAsVec(const Coord& start, double minDist) const {
    // find start node
    const MapNode* startNode = &_nodes[tree.search(start)];

    // check if start node is police station
    if (startNode->isPoliceStation) {
        throw runtime_error("Never start at the police station.");
    }

    // set DFS path
    vector<int> currNodes;
    vector<bool> visited = vector(0, false);
    if (!findRoute(currNodes, minDist, visited)) {
        throw runtime_error("Cannot find escape route");
    }
    return currNodes;
}


// -------------------------- Helpers -------------------------------


vector<bool> SFMap::getValidSubset() const {
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
            for (const MapNode* nextNode : _neighbors[curr]) {
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

void SFMap::getValidSubsetHelper(vector<bool>& validPoints) const {
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

void SFMap::drawCircle(cs225::PNG& image, const Coord& center, double radius,
    const cs225::rgbaColor& color) const {

    // TODO
}

void SFMap::drawLine(cs225::PNG& image, const Coord& start, const Coord& end, double width,
    const cs225::rgbaColor& color) const {

    // TODO
}

// Using recursion might cause a stack overflow given the large number of nodes
// Consider using stack + loop instead of recursion
bool SFMap::findRoute(vector<int>& currNodes, double remainDist, vector<bool>& visited) const {
    // base case
    if (remainDist <= 0) {
        return true;
    }

    const MapNode& lastNode = _nodes[currNodes.back()];
    for (const MapNode* neighbor : _neighbors[lastNode.index]) {
        if (neighbor->isPoliceStation || visited[neighbor->index]) {
            continue;
        }

        visited[neighbor->index] = true;
        const MapNode& newNode = _nodes[neighbor->index];

        // recursion
        currNodes.push_back(newNode.index);
        if (findRoute(currNodes, remainDist - dist(newNode.coord, lastNode.coord), visited)) {
            return true;
        }
        currNodes.pop_back();
    }

    return false;
}
