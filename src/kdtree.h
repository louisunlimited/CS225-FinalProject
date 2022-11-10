#pragma once

#include <functional>
#include <vector>
#include "coord.h"

using namespace std;

/**
 * Simplified version of the KDTree implemented in MP_mosaic
 */
class KDTree {
    private:
        /**
        * Internal structure for a node of KDTree.
        * Contains left, right children pointers, a pointer to a Coord, and the index of the node.
        */
        struct KDTreeNode {
            Coord coord;
            int index;
            KDTreeNode* left;
            KDTreeNode* right;

            KDTreeNode(const Coord& coord_, int index_):
                coord(coord_), index(index_), left(nullptr), right(nullptr) {}

            KDTreeNode* clone() {
                KDTreeNode* newNode = new KDTreeNode(coord, index);
                if (left != nullptr) newNode->left = left->clone();
                if (right != nullptr) newNode->right = right->clone();
                return newNode;
            }
        };

    public:
        /**
         * Empty constructor. Do not use it.
         */
        KDTree();

        /**
        * Constructs a KDTree from a vector of Coords.
        *
        * @param coords The vector of points to build your KDTree off of.
        */
        KDTree(const vector<pair<Coord, int>>& coords, function<double(const Coord&, const Coord&)> dist);


        /**
        * Copy constructor for KDTree.
        *
        * @param other The KDTree to copy.
        */
        KDTree(const KDTree& other);

        /**
        * Assignment operator for KDTree.
        *
        * @param rhs The right hand side of the assignment statement.
        * @return A reference for performing chained assignments.
        */
        const KDTree& operator=(const KDTree& other);

        /**
        * Destructor for KDTree.
        */
        ~KDTree();

        /**
         */
        unsigned size() const;

        /**
        * Finds the index of the closest coord to the parameter coord in the KDTree.
        *
        * @param query The point we wish to find the closest neighbor to in the tree.
        * @return The index to the closest coord.
        */
        int search(const Coord& query) const;

    private:
        /** root and size **/
        KDTreeNode* _root;
        unsigned _size;
        function<double(const Coord&, const Coord&)> _dist;

        // Helpers
        KDTreeNode* buildTree(vector<pair<Coord, int>>& coords, int d);
        void clear();
        KDTreeNode* findNearest(KDTreeNode* subroot, const Coord& query, int d) const;
        bool shouldReplace(const Coord& target, const Coord& curBest, const Coord& potential) const;
};

bool smallerDimVal(const Coord& first, const Coord& second, int d);

#include "kdtree.hpp"
