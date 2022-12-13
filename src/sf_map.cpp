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

    _tree = KDTree(coords, normalizedDist);
    _mst = MST(coords, getAdjList(), _dist);
}

SFMap::SFMap(const vector<Coord>& nodes, const vector<pair<int, int>>& edges,
    const vector<Coord>& police): SFMap(nodes, edges) {

    for (const Coord& coord : police) {
        addPoliceStation(coord);
    }
}

void SFMap::addPoliceStation(const Coord& coord) {
    // Find nearest node to the police station
    int index = _tree.search(coord);

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
PNG SFMap::importance() const {
    vector<double> imp = importanceAsVec();
    return drawMap([&imp](int index) {
            // red = 360, blue = 240
            return hsl2rgb(hslaColor{ 240 + 120 * (imp[index]), 0.8, 0.5, 1 });
        }, [](int index1, int index2) {
            return rgbaColor{ 0, 0, 0, 255 };
        });
}

vector<double> SFMap::importanceAsVec() const {
    // for each node as a start node
    // do Dijkstra's algorithm and add all nodes' visited times in the vector
    // then we get a list of importance of each node
    int n = size();
    vector<double> importanceValues(n, 0);

    cout << "Calculating importance..." << endl;
    for (int index = 0; index < n; index++) {
        if (index % 100 == 0) cout << index << " / " << n << endl;
        // call getParents function
        vector<int> parents = getParents(index, 30);
        for (int i = 0; i < n; i++) {
            if (parents[i] == -1) continue;
            int j = i;
            while (j != -1) {
                importanceValues[j] += 1;
                j = parents[j];
            }
        }
    }
    cout << n << " / " << n << endl;

    // Normalize importance values
    double maxValue = importanceValues[0];
    for (double val : importanceValues) maxValue = max(maxValue, val);
    for (double& val : importanceValues) val /= maxValue;

    return importanceValues;
}

/***    Goal 2   ***/
PNG SFMap::accessPoint() const {
    // Get mst edges
    vector<pair<int, int>> edges = getMST();
    // Construct adjList with edges
    vector<vector<int>> adjList = vector(size(), vector<int>());
    for (auto [i, j] : edges) {
        adjList[i].push_back(j);
        adjList[j].push_back(i);
    }

    // draw edge in mstEdges
    PNG image = drawMap([this](int index) {
            if (_neighbors[index].size() == 2) {
                return rgbaColor{ 0, 0, 0, 0 };
            } else {
                return rgbaColor{ 255, 0, 0, 255 };
            }
        }, [&adjList](int index1, int index2) {
            if (find(adjList[index1].begin(), adjList[index1].end(), index2) != adjList[index1].end()) {
                return rgbaColor{ 0, 0, 0, 255 };
            } else {
                return rgbaColor{ 0, 0, 0, 0 };
            }
        });

    return image;
}

vector<pair<int, int>> SFMap::getMST() const {
    for (int i = 0; i < size(); i++) {
        if (_neighbors[i].size() != 2) {
            return _mst.primMST(i);
        }
    }
    throw runtime_error("All nodes have degree 2");
}

/***    Goal 3   ***/
Animation SFMap::escapeRoute(const Coord& start, double minDist, double zoom) {
    Animation animation;
    vector<int> route = escapeRouteAsVec(start, minDist);

    // Find total distance
    double total = 0;
    vector<double> distances = { 0 };
    for (int i = 0; i < (int)route.size() - 1; i++) {
        total += _dist(_nodes[route[i]].coord, _nodes[route[i + 1]].coord);
        distances.push_back(total);
    }
    double step = total / (FRAMES - 1);

    // Change size of image
    const double MAX_DIM = 1000;
    double originalScale = SCALE;
    double originalRadius = RADIUS;
    double originalWidth = LINE_WIDTH;
    double latDiff = _maxLat - _minLat;
    double longDiff = _maxLong - _minLong;
    if (SCALE * latDiff > MAX_DIM || SCALE * longDiff > MAX_DIM) {
        SCALE = MAX_DIM / max(latDiff, longDiff);
    }
    RADIUS /= sqrt(originalScale / SCALE);
    LINE_WIDTH /= sqrt(originalScale / SCALE);

    // Draw frames
    int j = 0;
    Coord center = _nodes[route[0]].coord;

    cout << "Drawing GIF for escape route..." << endl;
    for (int i = 0; i < FRAMES; i++) {
        if (i % 10 == 0) cout << "Loading: "<< i << " / " << FRAMES << " finished" << endl;

        // Calculate criminal's current location
        double curDist = (i == FRAMES - 1) ? total : step * i;
        while (distances[j + 1] < curDist) j++;
        double percentage = (curDist - distances[j]) / (distances[j + 1] - distances[j]);
        const Coord& left = _nodes[route[j]].coord;
        const Coord& right = _nodes[route[j + 1]].coord;
        Coord target(left.lat_ * (1 - percentage) + right.lat_ * percentage,
            left.long_ * (1 - percentage) + right.long_ * percentage);
        center.lat_ = center.lat_ * (1 - ALPHA) + target.lat_ * ALPHA;
        center.long_ = center.long_ * (1 - ALPHA) + target.long_ * ALPHA;

        // Draw a single frame
        PNG image = drawMap(zoom, center, [&route](int index) {
                if (find(route.begin(), route.end(), index) != route.end()) {
                    return rgbaColor{ 0, 80, 255, 255 };
                } else {
                    return rgbaColor{ 0, 0, 0, 255 };
                }
            }, [&route](int index1, int index2) {
                for (int i = 0; i < (int)route.size() - 1; i++) {
                    if ((index1 == route[i] && index2 == route[i + 1]) ||
                        (index2 == route[i] && index1 == route[i + 1])) {
                        return rgbaColor{ 0, 80, 255, 255 };
                    }
                }
                return rgbaColor{ 0, 0, 0, 255 };
            });

        // Draw criminal
        double zHeight = (_maxLat - _minLat + MARGIN * 2) / zoom;
        double zWidth = (_maxLong - _minLong + MARGIN * 2) / zoom;
        double zMinLat = min(max(center.lat_ - 0.5 * zHeight, _minLat - MARGIN), _maxLat + MARGIN - zHeight);
        double zMinLong = min(max(center.long_ - 0.5 * zWidth, _minLong - MARGIN), _maxLong + MARGIN - zWidth);
        Coord lowerLeft = Coord(zMinLat, zMinLong);
        Coord zoomedTarget = coord2Pixel(target, lowerLeft, zoom);
        drawCircle(image, zoomedTarget, RADIUS * sqrt(zoom) * 2, rgbaColor{ 255, 0, 0, 255 });

        animation.addFrame(image);
    }
    cout << "Loading: "<< FRAMES << " / " << FRAMES << " finished" << endl;

    SCALE = originalScale;
    RADIUS = originalRadius;
    LINE_WIDTH = originalWidth;
    return animation;
}

vector<int> SFMap::escapeRouteAsVec(const Coord& start, double minDist) const {
    // find start node
    const MapNode* startNode = &_nodes[_tree.search(start)];

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

PNG SFMap::nextPoliceStation(double zoom) const {
    int next = nextPoliceStationAsIndex();
    Coord center = _nodes[next].coord;

    PNG image = drawMap(zoom, center, [next](int index) {
                return rgbaColor{ 0, 0, 0, 255 };
            }, [](int index1, int index2) {
                return rgbaColor{ 0, 0, 0, 255 };
            });

    double zHeight = (_maxLat - _minLat + MARGIN * 2) / zoom;
    double zWidth = (_maxLong - _minLong + MARGIN * 2) / zoom;
    double zMinLat = min(max(center.lat_ - 0.5 * zHeight, _minLat - MARGIN), _maxLat + MARGIN - zHeight);
    double zMinLong = min(max(center.long_ - 0.5 * zWidth, _minLong - MARGIN), _maxLong + MARGIN - zWidth);
    Coord lowerLeft = Coord(zMinLat, zMinLong);

    // draw original police stations
    for (int i = 0; i < (int)_police.size(); i++) {
        Coord zoomedTarget = coord2Pixel(_police[i]->coord, lowerLeft, zoom);
        drawCircle(image, zoomedTarget, RADIUS * sqrt(zoom) * 10, rgbaColor{ 255, 0, 0, 255 });
    }

    // draw new police stations
    Coord zoomedTarget = coord2Pixel(center, lowerLeft, zoom);
    drawCircle(image, zoomedTarget, RADIUS * sqrt(zoom) * 10, rgbaColor{ 0, 255, 0, 255 });

    return image;
}

int SFMap::nextPoliceStationAsIndex() const {
    // Algorithm as described in the PDF file
    double eSup = numeric_limits<double>::max();
    vector<double> eInf = vector(size(), 0.0);

    // Construct list of potential locations
    vector<int> potential;
    int curBest = -1;  // Current best choice of police station
    for (int i = 0; i < size(); i++) {
        if (!_nodes[i].isPoliceStation) potential.push_back(i);
    }

    while (!potential.empty()) {
        cout << potential.size() << " potential locations... ";

        // Find next location to check with minimum eInf
        int nextId = 0;
        double lowestInf = eInf[potential[0]];
        for (int i = 0; i < (int)potential.size(); i++) {
            if (eInf[potential[i]] < lowestInf) {
                nextId = i;
                lowestInf = eInf[potential[i]];
            }
        }
        int next = potential[nextId];  // Next location to check
        // Remove next from the list of potential locations
        swap(potential[nextId], potential.back());
        potential.pop_back();

        cout << "Checking node " << next << ": ";

        // Check if next is better than curBest
        auto [ev, v] = getEccentricity(next);
        if (eSup > ev) {
            eSup = ev;
            curBest = next;
        }
        cout << ev << " (best: " << eSup << ")";

        // Update lower bound of eccentricity
        vector<double> d = getDistances(vector{ v });
        for (int i = 0; i < size(); i++) {
            eInf[i] = max(eInf[i], d[i]);
        }

        // Remove candidates with inf >= sup (which disqualifies them from being a better location)
        vector<int> potential_;
        for (int i : potential) {
            if (eInf[i] < eSup) {
                potential_.push_back(i);
            }
        }
        potential = potential_;

        cout << endl;
    }

    return curBest;
}

pair<vector<int>, double> SFMap::nextPoliceStationAsIndexSlow(int index, int numProcesses) const {
    double min_eccentricity = numeric_limits<double>::max();
    vector<int> potential;
    for (const MapNode& node : _nodes) {
        // Filter all nodes by index (for parallel computing)
        if (node.index % numProcesses != index) continue;

        if (node.index % 1 == 0) {
            cout << "[" << index << "] ";
            cout << node.index << " / " << size() << endl;
        }
        if (node.isPoliceStation) continue;
        auto tmp = getEccentricity(node.index);
        if (tmp.first < min_eccentricity) {
            min_eccentricity = tmp.first;
            potential = vector{ node.index };
        } else if (tmp.first == min_eccentricity) {
            potential.push_back(node.index);
        }
    }

    cout << size() << " / " << size() << endl;
    return pair(potential, min_eccentricity);
}

/***    Other helpers   ***/
int SFMap::size() const {
    return _nodes.size();
}
