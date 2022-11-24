#pragma once;

#include <vector>
#include "coord.h"

class MST {
    private:
        struct Edge {
            int u, v;
            double weight;
            Edge(int u, int v, double weight) {
                this->u = u;
                this->v = v;
                this->weight = weight;
            }
        };
    public:
        MST();
    private:
        int vertices;
};