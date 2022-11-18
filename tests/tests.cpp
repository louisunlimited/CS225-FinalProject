#include <catch2/catch_test_macros.hpp>

#include "filereader.h"

vector<Coord> makeCoords() {
    vector<Coord> coords;
    for (int i = 0; i < 10; i++) {
        Coord coord;
        coord.long_ = i;
        coord.lat_ = i;
        coords.push_back(coord);
    }
    return coords;
}

TEST_CASE("readRawNode", "[FileReader]") {
    vector<Coord> nodes = FileReader::readRawNode("../data/SF.cnode.txt");
    REQUIRE(nodes.size() == 174956);
    REQUIRE(nodes[0].long_ == 1905.934692);
    REQUIRE(nodes[0].lat_ == 2760.598145);
    REQUIRE(nodes[1].long_ == 1910.898193);
    REQUIRE(nodes[1].lat_ == 2758.484863);
    REQUIRE(nodes[91899].long_ == 7556.671875);
    REQUIRE(nodes[91899].lat_ == 8581.037109);
    REQUIRE(nodes[174955].long_ == 3999.741943);
    REQUIRE(nodes[174955].lat_ == 5436.662598);
}

TEST_CASE("convertNode", "[FileReader]") {
    vector<Coord> nodes = makeCoords(); // coords from 0 to 9

    Coord anchor1, anchor2;
    anchor1.long_ = 1;
    anchor1.lat_ = 1;
    anchor2.long_ = 2;
    anchor2.lat_ = 2;
    Coord normalizedAnchor1, normalizedAnchor2;
    normalizedAnchor1.long_ = 2;
    normalizedAnchor1.lat_ = 2;
    normalizedAnchor2.long_ = 8;
    normalizedAnchor2.lat_ = 8;

    FileReader::convertNode(nodes, anchor1, normalizedAnchor1, anchor2, normalizedAnchor2);
    REQUIRE(nodes.size() == 10); 
    REQUIRE(nodes[2].long_ == 1);
    REQUIRE(nodes[2].lat_ == 1);
    REQUIRE(nodes[8].long_ == 2);
    REQUIRE(nodes[8].lat_ == 2);
    REQUIRE(nodes[9].long_ > 2.1);
    REQUIRE(nodes[9].lat_ > 2.1);

}

TEST_CASE("Test SFMap constructor", "[SFMap][ctor]") {
    // vector<Coord> nodes = FileReader.readNormalizedNode("../data/SF.cnode.txt");
    // FileReader.convertNode(nodes, ...);
    // vector<pair<int, int>> edges = FileReader.readEdge("../data/SF.cedge.txt");
    // vector<Coord> police = FileReader.readPoliceStation("../data/Map_of_Police_Stations__2011_.csv");

    // SFMap sfmap(nodes, edges, police);
}
