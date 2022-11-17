#include <catch2/catch_test_macros.hpp>

#include "filereader.h"

TEST_CASE("Test FileReader readEdge", "[FileReader]") {
    vector<std::pair<int, int>> edges = FileReader::readEdge("../data/SF.cedge.txt");

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
    vector<Coord> police = FileReader::readPoliceStation("../data/Map_of_Police_Stations__2011_.csv");

    // check police station read
    REQUIRE(police.size() == 10);
    REQUIRE(police[0].lat_ == 37.7986453192461);
    REQUIRE(police[0].long_ == -122.4098629092911);
    REQUIRE(police[9].lat_ == 37.7801611403781);
    REQUIRE(police[9].long_ == -122.432390435179);
}

TEST_CASE("Test SFMap constructor", "[SFMap][ctor]") {
    // vector<Coord> nodes = FileReader.readNormalizedNode("../data/SF.cnode.txt");
    // FileReader.convertNode(nodes, ...);
    // vector<pair<int, int>> edges = FileReader.readEdge("../data/SF.cedge.txt");
    // vector<Coord> police = FileReader.readPoliceStation("../data/Map_of_Police_Stations__2011_.csv");

    // SFMap sfmap(nodes, edges, police);
}
