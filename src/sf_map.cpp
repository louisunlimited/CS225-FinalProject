#include "sf_map.h"

// Cao Tianyue
SFMap::SFMap(vector<Coord> nodes, vector<tuple<int, int>> edges) {
}

SFMap::SFMap(vector<Coord> nodes, vector<tuple<int, int>> edges, vector<Coord> police) {
}

// Sun Xiping
vector<SFMap::MapNode*> SFMap::escapeRouteAsVec(Coord start, double minDist) {
    vector<SFMap::MapNode*> path;
    SFMap::MapNode* startNode;
    for (auto node: _nodes) {
        if (dist((&node).coord, start) < 0.001) {
            startNode = new SFMap::MapNode(node.index, start, false);
            break;
        }
    }
    vector<SFMap::MapNode*> currNodes;
    unordered_map<int, bool> visited;
    findRoute(currNodes, minDist, visited)
    return currNodes;
}
bool SFMap::DFS(vector<SFMap::MapNode*>& currNodes, double remainDist, unordered_map<int, bool>& visited) {
    if (remainDist <= 0) {
        return true;
    }
    SFMap::MapNode* lastNode = currNodes[currNodes.size()-1];
    for (auto neighbor: _neighbors[lastNode->index]) {
        if (neighbor->isPoliceStation || visited.find(neighbor.index) != visited.end()) {
            continue;
        }
        visited[neighbor.index] = true;
        SFMap::MapNode* newNode = new SFMap::MapNode*(neighbor.index, neighbor.coord, false);
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


