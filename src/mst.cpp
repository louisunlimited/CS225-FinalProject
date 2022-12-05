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
}

// MST::MSTNode* MST::findMinEdge(MSTNode* node) {
//     MSTNode* minEdge = nullptr;
//     double minWeight = numeric_limits<double>::max();
//     for (auto& edge : _adjList[node->index]) {
//         if (edge.first < minWeight) {
//             minEdge = edge.second;
//             minWeight = edge.first;
//         }
//     }
//     return minEdge;
// }

vector<pair<int, int>> MST::primMST(int start) {
    // performs a prim's algorithm on the graph with _adjList
    // returns a vector of edges
    vector<pair<int, int>> edges;
    // vector<bool> visited(_adjList.size(), false);
    // priority_queue<pair<double, MSTNode*>, vector<pair<double, MSTNode*>>, greater<pair<double, MSTNode*>>> pq;
    // pq.push(make_pair(0, &_nodes[start]));

    // while (!pq.empty()) {
    //     pair<double, MSTNode*> curr = pq.top();
    //     pq.pop();
    //     if (visited[curr.second->index]) {
    //         continue;
    //     }
    //     visited[curr.second->index] = true;
    //     if (curr.second->parent != nullptr) {
    //         edges.push_back(make_pair(curr.second->parent->index, curr.second->index));
    //     }
    //     for (auto& edge : _adjList[curr.second->index]) {
    //         if (!visited[edge.second->index]) {
    //             edge.second->parent = curr.second;
    //             pq.push(edge);
    //         }
    //     }
    // }
    return edges;
}

vector<MST::MSTNode*> MST::findValidNode(MSTNode* startNode, MSTNode* dirNode) {
    // go down the path of the direction node using _adjList
    vector<MSTNode*> path{ startNode, dirNode };
    MSTNode* curr = dirNode;
    while (_adjList[curr->index].size() == 2) {
        // go to the next node
        int next = _adjList[curr->index][0] == path.back() ? 1 : 0;
        curr = _adjList[curr->index][0];
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
