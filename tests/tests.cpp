#include <catch2/catch_test_macros.hpp>

#include "filereader.h"
#include "sf_map.h"

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

SFMap loadMap() {
    vector<Coord> nodes = FileReader::readRawNode("../data/SF.cnode.txt");
    Coord anchor1(37.5108, -122.1117);
    Coord normalizedAnchor1(5037.15, 4518.17);
    Coord anchor2(37.9956, -123.0232);
    Coord normalizedAnchor2(2261.93, 0);
    FileReader::convertNode(nodes, anchor1, normalizedAnchor1, anchor2, normalizedAnchor2);
    vector<pair<int, int>> edges = FileReader::readEdge("../data/SF.cedge.txt");
    vector<Coord> police = FileReader::readPoliceStation("../data/Map_of_Police_Stations__2011_.csv");
    return SFMap(nodes, edges, police);
}

SFMap sfmap = loadMap();

TEST_CASE("Test FileReader readEdge", "[FileReader]") {
    vector<pair<int, int>> edges = FileReader::readEdge("../data/SF.cedge.txt");

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
    vector<Coord> nodes_ = makeCoords(); // coords from 0 to 9

    Coord anch1, anch2;
    anch1.long_ = 1;
    anch1.lat_ = 1;
    anch2.long_ = 2;
    anch2.lat_ = 2;
    Coord nAnch1, nAnch2;
    nAnch1.long_ = 2;
    nAnch1.lat_ = 2;
    nAnch2.long_ = 8;
    nAnch2.lat_ = 8;

    FileReader::convertNode(nodes_, anch1, nAnch1, anch2, nAnch2);
    REQUIRE(nodes_.size() == 10); 
    REQUIRE(nodes_[2].long_ == 1);
    REQUIRE(nodes_[2].lat_ == 1);
    REQUIRE(nodes_[8].long_ == 2);
    REQUIRE(nodes_[8].lat_ == 2);
    REQUIRE(nodes_[9].long_ > 2.1);
    REQUIRE(nodes_[9].lat_ > 2.1);
}

TEST_CASE("Test SFMap drawMap", "[SFMap][png]") {
    PNG image = sfmap.drawMap(true);
    image.writeToFile("map.png");
    PNG zoomedImage = sfmap.drawMap(12, Coord(37.7983, -122.3778), true);
    zoomedImage.writeToFile("map-zoomed.png");
}
