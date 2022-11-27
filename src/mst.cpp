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
    vector<pair<int, int>> mst;
    vector<bool> visited(adjList.size(), false);
    priority_queue<pair<double, MSTNode*>, vector<pair<double, MSTNode*>>, greater<pair<double, MSTNode*>>> pq;
    pq.push(make_pair(0, &_coords[start]));
    while (!pq.empty()) {
        MSTNode* node = pq.top().second;
        pq.pop();
        if (visited[node->index]) continue;
        visited[node->index] = true;
        if (node->parent != nullptr) {
            mst.push_back(make_pair(node->parent->index, node->index));
        }
        for (auto& edge : adjList[node->index]) {
            if (!visited[edge.second->index]) {
                edge.second->parent = node;
                pq.push(edge);
            }
        }
    }
    return mst;
}
