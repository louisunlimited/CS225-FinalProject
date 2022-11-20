#include <iostream>
#include "sf_map.h"

cs225::PNG SFMap::importance(const cs225::rgbaColor& color) {
    vector<double> importanceValues = importanceAsVec();

    double zoom = 1.0;
    Coord center;
    center.lat_ = 0.0;
    center.long_ = 0.0;
    double mHeight = _maxLat - _minLat;
    double mWidth = _maxLong - _minLong;
    int pHeight = mHeight * SCALE;
    int pWidth = mWidth * SCALE;
    double zHeight = mHeight / zoom;
    double zWidth = mWidth / zoom;
    if (mHeight == 0 || mWidth == 0) {
        throw invalid_argument("Map too narrow to be drawn");
    }

    double zMinLat = min(max(center.lat_ - 0.5 * zHeight, _minLat), _maxLat - zHeight);
    double zMinLong = min(max(center.long_ - 0.5 * zWidth, _minLong), _maxLong - zWidth);
    Coord lowerLeft;
    lowerLeft.lat_ = zMinLat;
    lowerLeft.long_ = zMinLong;

    cs225::PNG image(pWidth, pHeight);
    for (int i = 0; i < size(); i++) {
        const MapNode& node = _nodes[i];
        for (const MapNode* neighbor : _neighbors[i]) {
            if (neighbor->index > i) {
                Coord start = coord2Pixel(node.coord, lowerLeft, zoom);
                Coord end = coord2Pixel(neighbor->coord, lowerLeft, zoom);
                drawLine(image, start, end, LINE_WIDTH * sqrt(zoom), color);
            }
        }
    }

    double maxImportanceValue = -1.0;
    for (auto value : importanceValues) {
        if (value > maxImportanceValue) {
            maxImportanceValue = value;
        }
    }

    int index = 0;
    for (const MapNode& node : _nodes) {
        if (node.coord.lat_ < zMinLat || node.coord.lat_ > zMinLat + zHeight
            || node.coord.long_ < zMinLong || node.coord.long_ > zMinLong + zWidth) {
                continue;
        }
        Coord zoomed = coord2Pixel(node.coord, lowerLeft, zoom);
        // set different colors to different nodes based on their importanceValues
        double importanceValue = importanceValues[index];
        hslaColor hsla = rgb2hsl(color);

        // todo, change hsla's s value based on importancevalue
        hsla.s = importanceValue / maxImportanceValue;

        cs225::rgbaColor newcolor = hsl2rgb(hsla);
        drawCircle(image, zoomed, RADIUS * sqrt(zoom), newcolor);
        index ++;
    }

    return image;

}



vector<double> SFMap::importanceAsVec() {
    // for each node as a start node
    // do Dijkstra's algorithm and add all nodes' visited times in the vector
    // then we get a list of importance of each node
    vector<double> importanceValues(_nodes.size(), 0.0);
    for (auto node : _nodes) {
        // implement Dijkstra's algorithm
        vector<double> dist(_nodes.size(), INT_MAX);
        vector<int> prev(_nodes.size(), -1);
        dist[node.index] = 0;
        prev[node.index] = node.index;
        std::priority_queue<std::tuple<double, int, int>, vector<std::tuple<double, int, int>>, \
            std::greater<std::tuple<double, int, int>>> myprq;
        myprq.push(std::tuple<double, int, int>(0.0, node.index, node.index));
        
        while (!myprq.empty()) {
            std::tuple<double, int, int> topTuple = myprq.top();
            myprq.pop();
            
            double curdist = std::get<0>(topTuple);
            int curindex = std::get<1>(topTuple);
            MapNode curnode = _nodes[curindex];
            if (prev[curindex] == -1 && dist[curindex] > curdist) { // curnode is not visited before
                prev[curindex] = std::get<2>(topTuple);
                dist[curindex] = curdist;
                for (auto neighbor : _neighbors[curindex]) {
                    if (prev[neighbor->index] == -1) { //not visited
                        // calculate distance = curdist + edgeweight
                        // todo calculate edgeweight by yourself
                        double edgeweight = sqrt((curnode.coord.lat_ - neighbor->coord.lat_) * (curnode.coord.lat_ - neighbor->coord.lat_) + \
                            (curnode.coord.long_ - neighbor->coord.long_) * (curnode.coord.long_ - neighbor->coord.long_));
                        double distance = curdist + edgeweight;
                        myprq.push(std::tuple<double, int, int>(distance, neighbor->index, curindex));
                    }
                }
            }
            else {
                continue;
            }
        }
        for (auto index : prev) {
            importanceValues[index] += 1.0;
        }
    }

    return importanceValues;
}