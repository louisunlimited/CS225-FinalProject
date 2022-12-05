#include "mst.h"

using namespace std;

MST::MST() {}

MST::MST(const vector<pair<Coord, int>>& coords, const vector<pair<int, int>>& edges, 
                                function<double(const Coord&, const Coord&)> dist) {
    _dist = dist;
    _coords = vector<MSTNode>();
    for (auto& coord : coords) {
        _coords.push_back(MSTNode(coord.second, coord.first));
    }

    adjList = vector<vector<pair<double, MSTNode*>>>(coords.size(), vector<pair<double, MSTNode*>>());
    for (auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        double weight = _dist(_coords[u].coord, _coords[v].coord);
        adjList[u].push_back(make_pair(weight, &_coords[v]));
        adjList[v].push_back(make_pair(weight, &_coords[u]));
    }
}

// MST::MSTNode* MST::findMinEdge(MSTNode* node) {
//     MSTNode* minEdge = nullptr;
//     double minWeight = numeric_limits<double>::max();
//     for (auto& edge : adjList[node->index]) {
//         if (edge.first < minWeight) {
//             minEdge = edge.second;
//             minWeight = edge.first;
//         }
//     }
//     return minEdge;
// }

vector<pair<int, int>> MST::primMST(int start) {
    // performs a prim's algorithm on the graph with adjList
    // returns a vector of edges
    vector<pair<int, int>> edges;
    vector<bool> visited(adjList.size(), false);
    priority_queue<pair<double, MSTNode*>, vector<pair<double, MSTNode*>>, greater<pair<double, MSTNode*>>> pq;
    pq.push(make_pair(0, &_coords[start]));

    while (!pq.empty()) {
        pair<double, MSTNode*> curr = pq.top();
        pq.pop();
        if (visited[curr.second->index]) {
            continue;
        }
        visited[curr.second->index] = true;
        if (curr.second->parent != nullptr) {
            edges.push_back(make_pair(curr.second->parent->index, curr.second->index));
        }
        for (auto& edge : adjList[curr.second->index]) {
            if (!visited[edge.second->index]) {
                edge.second->parent = curr.second;
                pq.push(edge);
            }
        }
    }
    return edges;
}

vector<MST::MSTNode*> MST::findValidNode(MSTNode* startNode, MSTNode* dirNode) {
    // go down the path of the direction node using adjlist
    vector<MSTNode*> path;
    path.push_back(startNode);
    path.push_back(dirNode);
    MSTNode* curr = dirNode;
    while (adjList[curr->index].size() == 2) {
        // go to the next node
        if (adjList[curr->index][0].second == path.back()) {
            curr = adjList[curr->index][1].second;
        } else {
            curr = adjList[curr->index][0].second;
        }
        path.push_back(curr);
    }
    return path;
}

double MST::findDistance(vector<MSTNode*> startRoute) {
    // adds up the distance between all the nodes in the route
    double totalDist = 0;
    for (size_t i = 0; i < startRoute.size() - 1; i++) {
        // if we were doing this, we might not need to store double in the adjlist
        totalDist += _dist(startRoute[i]->coord, startRoute[i + 1]->coord);
    }
    return totalDist;
}