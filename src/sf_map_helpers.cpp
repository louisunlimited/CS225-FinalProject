/**
 * Implementation of sf_map.h
 *
 * implements helper methods as well as methods used exclusively for testing.
 */

#include "sf_map.h"

void SFMap::setScale(double scale) {
    if (scale <= 0) throw invalid_argument("Scaling factor must be positive");
    SCALE = scale;
}

void SFMap::setMargin(double margin) {
    if (margin < 0) throw invalid_argument("Margin must be non-negative");
    MARGIN = margin;
}

void SFMap::setRadius(double radius) {
    if (radius <= 0) throw invalid_argument("Radius of node must be positive");
    RADIUS = radius;
}

void SFMap::setLineWidth(double lineWidth) {
    if (lineWidth <= 0) throw invalid_argument("Line width must be positive");
    LINE_WIDTH = lineWidth;
}

void SFMap::setMetric(function<double(const Coord&, const Coord&)> metric) {
    _dist = metric;
}

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
    // Create new list of nodes and map i to newIndexes
    int n = size();
    int newIndex = 0;
    vector<MapNode> newNodes;
    vector<int> toNewIndex;
    for (int i = 0; i < n; i++) {
        if (validPoints[i]) {
            newNodes.push_back(MapNode(newIndex, _nodes[i].coord, _nodes[i].isPoliceStation));
            toNewIndex.push_back(newIndex);
            newIndex++;
        } else {
            toNewIndex.push_back(-1);
        }
    }

    // Create new adjacency list (using index instead of pointers)
    vector<vector<int>> newNeighbors;
    for (int i = 0; i < n; i++) {
        // Skip entire adjacency vector if i is invalid
        if (!validPoints[i]) continue;

        vector<int> neighbors;
        for (MapNode* neighbor : _neighbors[i]) {
            if (validPoints[neighbor->index]) neighbors.push_back(toNewIndex[neighbor->index]);
        }
        newNeighbors.push_back(neighbors);
    }

    // Move new values to _nodes and _neighbors
    _nodes = newNodes;
    _neighbors.clear();
    for (const vector<int>& neighbors : newNeighbors) {
        vector<MapNode*> neighborsAsPtr;
        for (int neighbor : neighbors) {
            neighborsAsPtr.push_back(&_nodes[neighbor]);
        }
        _neighbors.push_back(neighborsAsPtr);
    }
}

Coord SFMap::coord2Pixel(const Coord& coord, const Coord& lowerLeft, double zoom) const {
    Coord pixel;
    pixel.lat_ = (coord.lat_ - lowerLeft.lat_) * zoom * SCALE;
    pixel.long_ = (coord.long_ - lowerLeft.long_) * zoom * SCALE;
    // Invert the latitude due to mismatch of the y-orientation of the image and map
    pixel.lat_ = (_maxLat - _minLat + MARGIN * 2) * SCALE - pixel.lat_;

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
            colorPixel(image.getPixel(x, y), color, percentage);
        }
    }
}

void SFMap::drawLine(PNG& image, const Coord& start, const Coord& end, double width,
    const rgbaColor& color) const {

    double dx = end.long_ - start.long_;
    double dy = end.lat_ - start.lat_;
    if (dx == 0 && dy == 0) {
        // If start point and end point overlap
        colorPixel(image.getPixel(start.long_, start.lat_), color, 1);
    } else if (abs(dx) >= abs(dy)) {
        // If the line connecting start to end is closer to x-axis (|slope| <= 1)
        // We iterate over all possible x values on the line
        Coord start_(dx > 0 ? start : end);
        Coord end_(dx > 0 ? end : start);
        // Make sure start and end are in range
        if (start_.long_ < 0) {
            start_.lat_ += -start_.long_ * dy / dx;
            start_.long_ = 0;
        }
        if (end_.long_ > image.width() - 1) {
            end_.lat_ += (image.width() - 1 - end_.long_) * dy / dx;
            end_.long_ = image.width() - 1;
        }

        double thickness = width * sqrt(dx * dx + dy * dy) / abs(dx);
        for (int x = start_.long_; x <= end_.long_; x++) {
            double yCenter = start_.lat_ + (x + 0.5 - start_.long_) * dy / dx;
            for (int y = yCenter - thickness * 0.5; y < yCenter + thickness * 0.5; y++) {
                if (y < 0 || y >= (int)image.height()) continue;

                // Calculate the percentage of the grid (x, y) inside the line
                double dist1 = thickness * 0.5 - abs(y - yCenter);
                double dist2 = thickness * 0.5 - abs(y + 1 - yCenter);
                double sum = dist1 + dist2;
                double absSum = abs(dist1) + abs(dist2);
                double percentage = (sum + absSum) / (2 * absSum);
                // Just to make sure...
                percentage = min(max(percentage, 0.0), 1.0);
                // Color the pixel
                colorPixel(image.getPixel(x, y), color, percentage);
            }
        }
    } else {
        // If the line connecting start to end is closer to y-axis (|slope| > 1)
        // We iterate over all possible y values on the line
        Coord start_(dy > 0 ? start : end);
        Coord end_(dy > 0 ? end : start);
        // Make sure start and end are in range
        if (start_.lat_ < 0) {
            start_.long_ += -start_.lat_ * dx / dy;
            start_.lat_ = 0;
        }
        if (end_.lat_ > image.height() - 1) {
            end_.long_ += (image.height() - 1 - end_.lat_) * dx / dy;
            end_.lat_ = image.height() - 1;
        }

        double thickness = width * sqrt(dx * dx + dy * dy) / abs(dy);
        for (int y = start_.lat_; y <= end_.lat_; y++) {
            double xCenter = start_.long_ + (y + 0.5 - start_.lat_) * dx / dy;
            for (int x = xCenter - thickness * 0.5; x < xCenter + thickness * 0.5; x++) {
                if (x < 0 || x >= (int)image.width()) continue;

                // Calculate the percentage of the grid (x, y) inside the line
                double dist1 = thickness * 0.5 - abs(x - xCenter);
                double dist2 = thickness * 0.5 - abs(x + 1 - xCenter);
                double sum = dist1 + dist2;
                double absSum = abs(dist1) + abs(dist2);
                double percentage = (sum + absSum) / (2 * absSum);
                // Just to make sure...
                percentage = min(max(percentage, 0.0), 1.0);
                // Color the pixel
                colorPixel(image.getPixel(x, y), color, percentage);
            }
        }
    }
}

void SFMap::colorPixel(HSLAPixel& pixel, const rgbaColor& color, double percentage) const {
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
        if (findRoute(currNodes, remainDist - _dist(newNode.coord, lastNode.coord), visited)) {
            return true;
        }
        currNodes.pop_back();
    }

    return false;
}

vector<int> SFMap::getParents(int start, double threshold) const {
    // No need to remember the distances because we are using a priority queue
    // This guarantees that we are always visiting the end point of the shortest path possible
    vector<int> prev(_nodes.size(), -1);
    priority_queue<tuple<double, int, int>, vector<tuple<double, int, int>>,
        greater<tuple<double, int, int>>> myprq;
    myprq.push(tuple(0.0, start, -1));

    while (!myprq.empty()) {
        auto [curdist, curindex, previndex] = myprq.top();
        myprq.pop();

        // if curnode is visited before then the current distance must be larger
        // therefore skip the remaining
        if (prev[curindex] != -1) continue;

        const MapNode& curnode = _nodes[curindex];
        prev[curindex] = previndex;

        for (const MapNode* neighbor : _neighbors[curindex]) {
            // skip if visited
            if (neighbor->index == start || prev[neighbor->index] != -1) continue;

            // _dist is the metric used in this map
            double edgeweight = _dist(curnode.coord, neighbor->coord);
            double distance = curdist + edgeweight;
            if (distance > threshold) continue;
            myprq.push(tuple(distance, neighbor->index, curindex));
        }
    }

    return prev;
}
