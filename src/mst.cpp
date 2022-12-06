#include "mst.h"

using namespace std;

MST::MST() {}

MST::MST(const vector<pair<Coord, int>>& coords, const vector<vector<int>>& adjList,
    function<double(const Coord&, const Coord&)> dist) {

    _dist = dist;
    _nodes = vector<MSTNode>();
    for (auto [coord, index] : coords) {
        _nodes.push_back(MSTNode(index, coord));
    }

    _adjList = vector<vector<MSTNode*>>();
    for (const vector<int>& neighbors : adjList) {
        _adjList.push_back(vector<MSTNode*>());
        for (int neighbor : neighbors) {
            _adjList.back().push_back(&_nodes[neighbor]);
        }
    }

    _mstNodes = vector<MSTNode*>();
    for (size_t i = 0; i < _adjList.size(); i++) {
        if (_adjList[i].size() != 2) {
            _mstNodes.push_back(&_nodes[i]);
        }
    }
}

int MST::size() const {
    return _nodes.size();
}

vector<pair<int, int>> MST::primMST(int start) const {
    // Makes sure that start node does not have degree 2
    if (start < 0 || start >= size()) throw invalid_argument("Start node must be in range");
    if (_adjList[start].size() == 2) throw invalid_argument("Start node must not have degree 2");

    vector<pair<int, int>> edges;

    // Keeps track of already visited nodes
    vector<bool> visited(size(), false);
    visited[start] = true;

    // All possible paths that can be added each step
    priority_queue<pair<double, vector<const MSTNode*>>, vector<pair<double, vector<const MSTNode*>>>,
        greater<pair<double, vector<const MSTNode*>>>> pq;
    // Initially only include paths from start node
    for (const MSTNode* neighbor : _adjList[start]) {
        vector<const MSTNode*> path = findCompleteRoute(&_nodes[start], neighbor);
        double dist = findDistance(path);
        pq.push(pair(dist, path));
    }

    // Main loop
    while (!pq.empty()) {
        auto [distance, path] = pq.top();
        pq.pop();

        // Check if the path connects to a new node
        if (visited[path.back()->index]) continue;
        visited[path.back()->index] = true;

        // Add the path to edges
        for (int i = 0; i < (int) path.size() - 1; i++) {
            edges.push_back(pair(path[i]->index, path[i + 1]->index));
        }

        // Add new possible paths to the priority queue
        for (const MSTNode* neighbor : _adjList[path.back()->index]) {
            vector<const MSTNode*> newPath = findCompleteRoute(path.back(), neighbor);
            // Check visiting a new node
            if (visited[newPath.back()->index]) continue;
            double dist = findDistance(newPath);
            pq.push(pair(dist, newPath));
        }
    }

    return edges;
}

vector<const MST::MSTNode*> MST::findCompleteRoute(const MSTNode* startNode, const MSTNode* dirNode) const {
    // go down the path of the direction node using _adjList
    vector<const MSTNode*> path{ startNode, dirNode };
    const MSTNode* curr = dirNode;
    while (_adjList[curr->index].size() == 2) {
        // go to the next node
        int i = _adjList[curr->index][0] == path.back() ? 1 : 0;
        curr = _adjList[curr->index][i];
        path.push_back(curr);
    }
    return path;
}

double MST::findDistance(const vector<const MSTNode*>& route) const {
    // Adds up the distance between all the nodes in the route
    double totalDist = 0;
    for (int i = 0; i < (int)route.size() - 1; i++) {
        // if we were doing this, we might not need to store double in the adjList
        totalDist += _dist(route[i]->coord, route[i + 1]->coord);
    }
    return totalDist;
}
