#include <catch2/catch_test_macros.hpp>

#include "filereader.h"
#include "sf_map.h"
#include<iostream>
using namespace std;

TEST_CASE("Test FileReader readEdge", "[FileReader]") {
    FileReader fr;
    vector<std::pair<int, int>> edges = fr.readEdge("../data/SF.cedge.txt");

    // check edge read
    REQUIRE(edges.size() == 223001);
    REQUIRE(edges[0].first == 0);
    REQUIRE(edges[0].second == 5);
    REQUIRE(edges[258].first == 5905);
    REQUIRE(edges[258].second == 5908);
    REQUIRE(edges[132812].first == 111607);
    REQUIRE(edges[132812].second == 111618);
    REQUIRE(edges[223000].first == 743);
    REQUIRE(edges[223000].second == 745);
}

TEST_CASE("Test FileReader readPoliceStation", "[FileReader]") {
    FileReader fr;
    vector<Coord> police = fr.readPoliceStation("../data/Map_of_Police_Stations__2011_.csv");

    // check police station read
    REQUIRE(police.size() == 10);
    REQUIRE(police[0].lat_ == 37.7986453192461);
    REQUIRE(police[0].long_ == -122.4098629092911);
    REQUIRE(police[9].lat_ == 37.7801611403781);
    REQUIRE(police[9].long_ == -122.432390435179);
}



//check DFS
TEST_CASE("Test escapeRoute with small graph", "[escapeRoute]") {
    FileReader fr;
    vector<Coord> nodes = fr.readNormalizedNode("../tests/small.node.txt");
    vector<pair<int, int>> edges = fr.readEdge("../tests/small.edge.txt");
    // call constructor
    SFMap a(nodes, edges);
    REQUIRE(a.escapeRouteAsVec(nodes[0] , 4).at(1)->index == 2);
    REQUIRE(a.escapeRouteAsVec(nodes[0] , 6).at(2)->index == 3);
    REQUIRE(a.escapeRouteAsVec(nodes[0] , 12).at(a.escapeRouteAsVec(nodes[0], 12).size() - 1)->index == 3);
    REQUIRE(a.escapeRouteAsVec(nodes[0] , 18).at(3)->index == 6);
}

// check DFS
TEST_CASE("Test escapeRoute with meidum graph", "[escapeRoute]") {
    FileReader fr;
    vector<Coord> nodes = fr.readNormalizedNode("../tests/medium.node.txt");
    vector<pair<int, int>> edges = fr.readEdge("../tests/medium.edge.txt");
}

// check DFS
TEST_CASE("Test escapeRoute with large graph", ["escapeRoute"]) {
    FileReader fr;
}

// check Dijkstra
TEST_CASE("Test importance with small graph", ["importance"]) {
    FileReader fr;
    vector<Coord> nodes = fr.readNormalizedNode("../tests/small.node.txt");
    vector<pair<int, int>> edges = fr.readEdge("../tests/small.edge.txt");
}

// check Dijkstra
TEST_CASE("Test importance with medium graph", ["importance"]) {
    FileReader fr;
    vector<Coord> nodes = fr.readNormalizedNode("../tests/medium.node.txt");
    vector<pair<int, int>> edges = fr.readEdge("../tests/medium.edge.txt");
}

// check Dijkstra
TEST_CASE("Test importance with large graph", ["importance"]) {
    FileReader fr;
}
