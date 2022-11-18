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

TEST_CASE("Test SFMap drawMap", "[SFMap][png]") {
    vector<Coord> nodes = FileReader::readRawNode("../data/SF.cnode.txt");
    // FileReader.convertNode(nodes, ...);
    vector<pair<int, int>> edges = FileReader::readEdge("../data/SF.cedge.txt");
    vector<Coord> police = FileReader::readPoliceStation("../data/Map_of_Police_Stations__2011_.csv");

    SFMap sfmap(nodes, edges, police);
    PNG image = sfmap.drawMap(false);
    image.writeToFile("map.png");
}

TEST_CASE("Test SFMap drawMap zoomed", "[SFMap][png][target]") {
    vector<Coord> nodes = FileReader::readRawNode("../data/SF.cnode.txt");
    // FileReader.convertNode(nodes, ...);
    vector<pair<int, int>> edges = FileReader::readEdge("../data/SF.cedge.txt");
    vector<Coord> police = FileReader::readPoliceStation("../data/Map_of_Police_Stations__2011_.csv");

    SFMap sfmap(nodes, edges, police);
    PNG image = sfmap.drawMap(8, Coord(6000, 6000), false);
    image.writeToFile("map(x8).png");
}
