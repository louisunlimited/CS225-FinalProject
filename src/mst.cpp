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

void MST::primMST(int start) {
    // Prim's algorithm, node is selected using findMinEdge
    vector<int> parent(_coords.size(), -1);
    vector<int> key(_coords.size(), numeric_limits<int>::max());
    vector<bool> inMST(_coords.size(), false);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, start));
    key[start] = 0;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        inMST[u] = true;
        for (auto& edge : adjList[u]) {
            int v = edge.second->index;
            double weight = edge.first;
            if (!inMST[v] && key[v] > weight) {
                key[v] = weight;
                pq.push(make_pair(key[v], v));
                parent[v] = u;
            }
        }
    }

    // print MST
    for (size_t i = 1; i < _coords.size(); i++) {
        cout << parent[i] << " - " << i << endl;
    }
}
