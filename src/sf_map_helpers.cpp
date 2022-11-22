/**
 * Implementation of sf_map.h
 *
 * implements helper methods as well as methods used exclusively for testing.
 */

#include "sf_map.h"

void SFMap::setScale(double scale) {
    if (scale <= 0) {
        throw invalid_argument("Scaling factor must be positive");
    }
    SCALE = scale;
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
    // Count number of valid points. Throw exception if no valid points.
    int n = 0;
    for (bool isValid : validPoints) if (isValid) n++;
    if (n == 0) throw invalid_argument(
        "The data received contains an insufficient number of valid nodes");

    // Clean adjacency list
    for (int i = validPoints.size() - 1; i >= 0; i--) {
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
    for (int i = _nodes.size() - 1; i >= 0; i--) {
        if (!validPoints[_nodes[i].index]) {
            _nodes.erase(_nodes.begin() + i);
        } else {
            _nodes[i].index = index--;
        }
    }

    if (index != -1) cout << "ERROR in cleanData: index != -1" << endl;
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
                double dist1 = thickness - abs(y - yCenter);
                double dist2 = thickness - abs(y + 1 - yCenter);
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
                double dist1 = thickness - abs(x - xCenter);
                double dist2 = thickness - abs(x + 1 - xCenter);
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
        if (findRoute(currNodes, remainDist - dist(newNode.coord, lastNode.coord), visited)) {
            return true;
        }
        currNodes.pop_back();
    }

    return false;
}

vector<double> SFMap::getParents(int start) const {
    // TODO
    vector<double> parents;
    return parents;
}