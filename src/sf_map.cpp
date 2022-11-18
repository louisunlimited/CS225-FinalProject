#include <iostream>
#include "sf_map.h"

SFMap::SFMap(const vector<Coord>& nodes, const vector<pair<int, int>>& edges) {
    // The validation criteria is slightly modified. The detailed description is in the header
    // files next to the `getValidSubset` method.

    int n = nodes.size();
    if (n == 0) throw invalid_argument("There should be at least one node in the data");

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
    // The map should be slightly larger than the actual range of all nodes
    _minLat -= MARGIN;
    _maxLat += MARGIN;
    _minLong -= MARGIN;
    _maxLong += MARGIN;

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

PNG SFMap::drawMap(double zoom, const Coord& center, bool drawLines) const {
    // Lat. and long. increases in the  direction below:
    //               ↑ (lat)
    //               |
    //               +------> (long)
    // x, y values of PNG increases in the direction below:
    //               +------> (x)
    //               |
    //               ↓ (y)

    // Check for invalid inputs
    if (zoom < 1 || zoom > MAX_ZOOM) {
        throw invalid_argument("Zoom factor must be in the range 1.0 ~ 20.0 inclusive");
    }

    double mHeight = _maxLat - _minLat;  // height of map (in deg)
    double mWidth = _maxLong - _minLong;  // height of map (in deg)
    int pHeight = mHeight * SCALE;  // height of image (in pxl)
    int pWidth = mWidth * SCALE;  // width of image (in pxl)
    double zHeight = mHeight / zoom;  // height of zoomed map (in deg)
    double zWidth = mWidth / zoom;  // height of zoomed map (in deg)
    if (mHeight == 0 || mWidth == 0) {
        throw invalid_argument("Map too narrow to be drawn");
    }

    // Find the lower left corner of the zoomed rectangle's borders
    // The zoomed rectangle will be placed such that `center` is at the center of it.
    // Note that it should not get out of bounds as defined by _minLat, _maxLat
    double zMinLat = min(max(center.lat_ - 0.5 * zHeight, _minLat), _maxLat - zHeight);
    double zMinLong = min(max(center.long_ - 0.5 * zWidth, _minLong), _maxLong - zWidth);
    Coord lowerLeft = Coord(zMinLat, zMinLong);

    // Create the canvas
    PNG image(pWidth, pHeight);
    rgbaColor black{ 0, 0, 0, 255 };

    // Draw lines
    if (drawLines) {
        for (int i = 0; i < size(); i++) {
            const MapNode& node = _nodes[i];
            for (const MapNode* neighbor : _neighbors[i]) {
                if (neighbor->index > i) {
                    // DO NOT SKIP even if node/neighbor is out of bounds
                    // Because a segment of the path may lie inside the zoomed rectangle
                    // Find the zoomed location (in pxl) of the pair of nodes
                    Coord start = coord2Pixel(node.coord, lowerLeft, zoom);
                    Coord end = coord2Pixel(neighbor->coord, lowerLeft, zoom);
                    drawLine(image, start, end, LINE_WIDTH * sqrt(zoom), black);
                }
            }
        }
    }

    // Draw points
    for (const MapNode& node : _nodes) {
        // Skip if node not in the zoomed rectangle
        if (node.coord.lat_ < zMinLat || node.coord.lat_ > zMinLat + zHeight
            || node.coord.long_ < zMinLong || node.coord.long_ > zMinLong + zWidth)
                continue;
        // Find the zoomed location (in pxl) of the node
        Coord zoomed = coord2Pixel(node.coord, lowerLeft, zoom);
        drawCircle(image, zoomed, RADIUS * sqrt(zoom), black);
    }

    return image;
}

PNG SFMap::drawMap(bool drawLines) const {
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
    if (n == 0) throw invalid_argument(
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

Coord SFMap::coord2Pixel(const Coord& coord, const Coord& lowerLeft, double zoom) const {
    Coord pixel;
    pixel.lat_ = (coord.lat_ - lowerLeft.lat_) * zoom * SCALE;
    pixel.long_ = (coord.long_ - lowerLeft.long_) * zoom * SCALE;
    // Invert the latitude due to mismatch of the y-orientation of the image and map
    pixel.lat_ = (_maxLat - _minLat) * SCALE - pixel.lat_;

    return pixel;
}

void SFMap::drawCircle(PNG& image, const Coord& center, double radius,
    const rgbaColor& color) const {

    double top = center.lat_ - radius;
    double bottom = center.lat_ + radius;
    double left = center.long_ - radius;
    double right = center.long_ + radius;

    for (int x = left; x < right; x++) {
        for (int y = top; y < bottom; y++) {
            // Skip if (x, y) out of bounds
            if (x < 0 || x >= (int)image.width() || y < 0 || y >= (int)image.height()) continue;

            // An approximation of the area of the pixel (x, y) that is inside the circle
            double score_tl = radius - normalizedDist(Coord(y, x), center);
            double score_tr = radius - normalizedDist(Coord(y, x + 1), center);
            double score_bl = radius - normalizedDist(Coord(y + 1, x), center);
            double score_br = radius - normalizedDist(Coord(y + 1, x + 1), center);
            double sum = score_tl + score_tr + score_bl + score_br;
            double absSum = abs(score_tl) + abs(score_tr) + abs(score_bl) + abs(score_br);
            double percentage = (sum + absSum) / (2 * absSum);
            // Just to make sure...
            percentage = min(max(percentage, 0.0), 1.0);

            // Color the pixel
            HSLAPixel& pixel = image.getPixel(x, y);
            hslaColor hsla{ pixel.h, pixel.s, pixel.l, pixel.a };
            rgbaColor rgba = hsl2rgb(hsla);
            rgba.r = (1 - percentage) * rgba.r + percentage * color.r;
            rgba.g = (1 - percentage) * rgba.g + percentage * color.g;
            rgba.b = (1 - percentage) * rgba.b + percentage * color.b;
            hsla = rgb2hsl(rgba);
            pixel.h = hsla.h;
            pixel.l = hsla.l;
            pixel.s = hsla.s;
            pixel.a = hsla.a;
        }
    }
}

void SFMap::drawLine(PNG& image, const Coord& start, const Coord& end, double width,
    const rgbaColor& color) const {

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
