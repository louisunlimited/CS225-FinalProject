#include "mst.h"

using namespace std;

MST::MST() {}

MST::MST(const vector<pair<Coord, int>>& coords, function<double(const Coord&, const Coord&)> dist) {
    _dist = dist;
    for (auto& coord : coords) {
        _coords.push_back(new MSTNode(coord.second, coord.first));
    }
    // build adjList
    for (int i = 0; i < (int) _coords.size(); i++) {
        for (int j = i + 1; j < (int) _coords.size(); j++) {
            adjList[i].push_back(_coords[j]);
            adjList[j].push_back(_coords[i]);
        }
    }
}

vector<pair<int, int>> MST::makeMST(int start) {
    vector<pair<int, int>> mst;
    vector<bool> visited(_coords.size(), false);
    priority_queue<pair<double, pair<int, int>>, vector<pair<double, pair<int, int>>>, greater<pair<double, pair<int, int>>>> pq;
    visited[start] = true;
    for (auto& node : adjList[start]) {
        pq.push({ _dist(_coords[start]->coord, node->coord), { start, node->index } });
    }
    while (!pq.empty()) {
        auto cur = pq.top();
        pq.pop();
        if (visited[cur.second.second]) continue;
        visited[cur.second.second] = true;
        mst.push_back(cur.second);
        for (auto& node : adjList[cur.second.second]) {
            pq.push({ _dist(_coords[cur.second.second]->coord, node->coord), { cur.second.second, node->index } });
        }
    }
    return mst;
}

void MST::printMST() {
    for (auto& edge : makeMST(0)) {
        cout << edge.first << " " << edge.second << endl;
    }
}
