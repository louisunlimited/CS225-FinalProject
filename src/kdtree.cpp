#include <algorithm>
#include <stack>

#include "kdtree.h"

using namespace std;

KDTree::KDTree() {}

KDTree::KDTree(const vector<pair<Coord, int>>& coords, function<double(const Coord&, const Coord&)> dist) {
    _size = coords.size();
    _dist = dist;
    vector<pair<Coord, int>> coords_ = coords;
    _root = buildTree(coords_, 0);
}

KDTree::KDTree(const KDTree& other) {
    _size = other._size;
    _dist = other._dist;
    _root = other._root == nullptr ? nullptr : other._root->clone();
}

const KDTree& KDTree::operator=(const KDTree& other) {
    if (this != &other) {
        clear();
        _size = other._size;
        _dist = other._dist;
        _root = other._root == nullptr ? nullptr : other._root->clone();
    }
    return *this;
}

KDTree::~KDTree() {
    clear();
}

unsigned KDTree::size() const {
    return _size;
}

int KDTree::search(const Coord& query) const {
    return _root == nullptr ? -1 : findNearest(_root, query, 0)->index;
}

KDTree::KDTreeNode* KDTree::findNearest(KDTree::KDTreeNode* subroot, const Coord& query, int d) const {
    KDTree::KDTreeNode* curBest = subroot;
    // next level's dimension
    int nd = (d + 1) % 2;
    // distance in dimension d
    Coord projCoord = subroot->coord;
    projCoord[nd] = query.at(nd);
    int proj = _dist(query, projCoord);

    if (smallerDimVal(query, subroot->coord, d)) {
        // Explore left subtree
        if (subroot->left != nullptr) {
            KDTree::KDTreeNode* potential = findNearest(subroot->left, query, nd);
            if (shouldReplace(query, curBest->coord, potential->coord)) {
                curBest = potential;
            }
        }

        // Explore right subtree if intersect
        if (subroot->right != nullptr && proj <= _dist(query, curBest->coord)) {
            KDTree::KDTreeNode* potential = findNearest(subroot->right, query, nd);
            if (shouldReplace(query, curBest->coord, potential->coord)) {
                curBest = potential;
            }
        }
    } else {
        // Explore right subtree
        if (subroot->right != nullptr) {
            KDTree::KDTreeNode* potential = findNearest(subroot->right, query, nd);
            if (shouldReplace(query, curBest->coord, potential->coord)) {
                curBest = potential;
            }
        }

        // Explore left subtree if intersect
        if (subroot->left != nullptr && proj <= _dist(query, curBest->coord)) {
            KDTree::KDTreeNode* potential = findNearest(subroot->left, query, nd);
            if (shouldReplace(query, curBest->coord, potential->coord)) {
                curBest = potential;
            }
        }
    }

    return curBest;
}

// Helpers
KDTree::KDTreeNode* KDTree::buildTree(vector<pair<Coord, int>>& coords, int d) {
    int n = coords.size();
    if (n == 0) return nullptr;
    int mid = (n - 1) / 2;

    // Split vectors
    vector<pair<Coord, int>>& left = coords;
    vector<pair<Coord, int>> right;
    sort(left.begin(), left.end(),
        [d] (const pair<Coord, int>& a, const pair<Coord, int>& b) -> bool {
            return a.first.at(d) < b.first.at(d);
        });
    while ((int) left.size() > mid + 1) {
        right.push_back(left.back());
        left.pop_back();
    }

    // Create current node
    KDTreeNode* node = new KDTreeNode(left[mid].first, left[mid].second);
    left.pop_back();

    // Build left and right subtree
    node->left = buildTree(left, (d + 1) % 2);
    node->right = buildTree(right, (d + 1) % 2);
    return node;
}

void KDTree::clear() {
    std::stack<KDTreeNode*> s;
    s.push(_root);
    while (!s.empty()) {
        KDTreeNode* next = s.top();
        s.pop();
        if (next == nullptr) continue;
        s.push(next->left);
        s.push(next->right);
        delete next;
    }
}

bool KDTree::shouldReplace(const Coord& target, const Coord& curBest, const Coord& potential) const {
    int curDist = _dist(target, curBest);
    int potDist = _dist(target, potential);
    return potDist < curDist;
}


bool smallerDimVal(const Coord& first, const Coord& second, int d) {
    int nd = (d + 1) % 2;
    if (first.at(d) == second.at(d)) {
        return first.at(nd) < second.at(nd);
    } else {
        return first.at(d) < second.at(d);
    }
}
