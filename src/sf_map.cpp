#include <iostream>
#include "sf_map.h"

/***    Constructors   ***/
SFMap::SFMap(const vector<Coord>& nodes, const vector<pair<int, int>>& edges) {
    // The validation criteria is slightly modified. The detailed description is in the header
    // files next to the `getValidSubset` method.

    if (nodes.size() == 0) throw invalid_argument("There should be at least one node in the data");

    // Populate the nodes (and update min & max coordinates)
    _nodes = vector<MapNode>();
    for (int i = 0; i < (int)nodes.size(); i++) {
        _nodes.push_back(MapNode(i, nodes.at(i), false));
    }

    // Populate the edges
    _neighbors = vector(nodes.size(), vector<MapNode*>());
    for (auto [i, j] : edges) {
        if (find(_neighbors[i].begin(), _neighbors[i].end(), &_nodes[j]) == _neighbors[i].end()) {
            _neighbors[i].push_back(&_nodes[j]);
            _neighbors[j].push_back(&_nodes[i]);
        }
    }

    _dist = dist;

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
    tree = KDTree(coords, normalizedDist);
}

SFMap::SFMap(const vector<Coord>& nodes, const vector<pair<int, int>>& edges,
    const vector<Coord>& police): SFMap(nodes, edges) {

    for (const Coord& coord : police) {
        addPoliceStation(coord);
    }
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

/***    Draw Png   ***/
PNG SFMap::drawMap(double zoom, const Coord& center, function<rgbaColor(int)> nodeColor,
    function<rgbaColor(int, int)> edgeColor) const {

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

    // The image should be slightly larger than the map
    double minLat = _minLat - MARGIN;
    double maxLat = _maxLat + MARGIN;
    double minLong = _minLong - MARGIN;
    double maxLong = _maxLong + MARGIN;

    double mHeight = maxLat - minLat;  // height of map (in deg)
    double mWidth = maxLong - minLong;  // height of map (in deg)
    int pHeight = mHeight * SCALE;  // height of image (in pxl)
    int pWidth = mWidth * SCALE;  // width of image (in pxl)
    double zHeight = mHeight / zoom;  // height of zoomed map (in deg)
    double zWidth = mWidth / zoom;  // height of zoomed map (in deg)
    if (mHeight == 0 || mWidth == 0) {
        throw invalid_argument("Map too narrow to be drawn");
    }

    // Find the lower left corner of the zoomed rectangle's borders
    // The zoomed rectangle will be placed such that `center` is at the center of it.
    // Note that it should not get out of bounds as defined by minLat, maxLat
    double zMinLat = min(max(center.lat_ - 0.5 * zHeight, minLat), maxLat - zHeight);
    double zMinLong = min(max(center.long_ - 0.5 * zWidth, minLong), maxLong - zWidth);
    Coord lowerLeft = Coord(zMinLat, zMinLong);

    // Create the canvas
    PNG image(pWidth, pHeight);

    // Draw edges
    for (int i = 0; i < size(); i++) {
        const MapNode& node = _nodes[i];
        for (const MapNode* neighbor : _neighbors[i]) {
            if (neighbor->index > i) {
                // DO NOT SKIP even if node/neighbor is out of bounds
                // Because a segment of the path may lie inside the zoomed rectangle
                // Find the zoomed location (in pxl) of the pair of nodes
                rgbaColor color = edgeColor(i, neighbor->index);
                if (color.a == 0) continue;  // if alpha value is 0, then the edge will not be shown
                Coord start = coord2Pixel(node.coord, lowerLeft, zoom);
                Coord end = coord2Pixel(neighbor->coord, lowerLeft, zoom);
                drawLine(image, start, end, LINE_WIDTH * sqrt(zoom), color);
            }
        }
    }

    // Draw points
    for (const MapNode& node : _nodes) {
        // Skip if node not in the zoomed rectangle
        if (node.coord.lat_ < zMinLat || node.coord.lat_ > zMinLat + zHeight
            || node.coord.long_ < zMinLong || node.coord.long_ > zMinLong + zWidth)
                continue;

        rgbaColor color = nodeColor(node.index);
        if (color.a == 0) continue;  // if alpha value is 0, then the edge will not be shown
        // Find the zoomed location (in pxl) of the node
        Coord zoomed = coord2Pixel(node.coord, lowerLeft, zoom);
        drawCircle(image, zoomed, RADIUS * sqrt(zoom), color);
    }

    return image;
}

PNG SFMap::drawMap(function<rgbaColor(int)> nodeColor, function<rgbaColor(int, int)> edgeColor) const {
    return drawMap(1, Coord(0, 0), nodeColor, edgeColor);
}

/***    Goal 1   ***/
PNG SFMap::importance(const rgbaColor& color) const {
    vector<double> imp = importanceAsVec();
    hslaColor baseColor = rgb2hsl(color);
    return drawMap([&imp, &baseColor](int index) {
            hslaColor c = baseColor;
            c.l = 0.95 - imp[index] * 0.9;
            return hsl2rgb(c);
        }, [](int index1, int index2) {
            return rgbaColor{ 0, 0, 0, 225 };
        });
}

vector<double> SFMap::importanceAsVec() const {
    // for each node as a start node
    // do Dijkstra's algorithm and add all nodes' visited times in the vector
    // then we get a list of importance of each node
    int n = size();
    vector<double> importanceValues(n, 0);

    for (int index = 0; index < n; index++) {
        if (index % 100 == 0) cout << index << " / " << n - 1 << endl;
        // call getParents function
        vector<int> parents = getParents(index, 20);
        for (int i = 0; i < n; i++) {
            if (parents[i] == -1) continue;
            int j = i;
            while (j != -1) {
                importanceValues[j] += 1;
                j = parents[j];
            }
        }
    }

    // Normalize importance values
    double maxValue = importanceValues[0];
    for (double val : importanceValues) maxValue = max(maxValue, val);
    for (double& val : importanceValues) val /= maxValue;

    return importanceValues;
}

/***    Goal 2   ***/

/***    Goal 3   ***/
vector<int> SFMap::escapeRouteAsVec(const Coord& start, double minDist) const {
    // find start node
    const MapNode* startNode = &_nodes[tree.search(start)];

    // check if start node is police station
    if (startNode->isPoliceStation) {
        throw runtime_error("Never start at the police station.");
    }

    // set DFS path
    vector<int> currNodes = { startNode->index };
    vector<bool> visited = vector(_nodes.size(), false);
    visited[startNode->index] = true;
    if (!findRoute(currNodes, minDist, visited)) {
        throw runtime_error("Cannot find escape route");
    }
    return currNodes;
}

/***    Goal 4   ***/

/***    Other helpers   ***/
int SFMap::size() const {
    return _nodes.size();
}
