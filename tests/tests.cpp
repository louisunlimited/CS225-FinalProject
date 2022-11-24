#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include "filereader.h"
#include "sf_map.h"

using namespace std;

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

TEST_CASE("SFMap constructor constructs the right amount of nodes", "[SFMap]") {
    REQUIRE(sfmap.size() == 174956);
}

TEST_CASE("Test SFMap drawMap small", "[SFMap][png]") {
    // make a custom set of coords
    vector<Coord> nodes = makeCoords(); // coords from 0 to 9
    vector<pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}};
    vector<Coord> police = makeCoords(); // coords from 0 to 9
    SFMap smallmap(nodes, edges);
    smallmap.setScale(50);

    PNG smallImage = smallmap.drawMap([](int index) {
            return rgbaColor{ 0, 0, 128, 255 };
        }, [](int index1, int index2) {
            return rgbaColor{ 0, 0, 0, 255 };
        });
    smallImage.writeToFile("small.png");
}

TEST_CASE("Test SFMap drawMap SF", "[SFMap][png]") {
    PNG sfImage = sfmap.drawMap([](int index) {
            return rgbaColor{ 0, 0, 128, 255 };
        }, [](int index1, int index2) {
            return rgbaColor{ 0, 0, 0, 0 };
        });
    sfImage.writeToFile("sf.png");
}

TEST_CASE("Test SFMap drawMap zoomed", "[SFMap][png]") {
    PNG zoomedImage = sfmap.drawMap(12, Coord(37.7983, -122.3778), [](int index) {
            return rgbaColor{ 0, 0, 128, 255 };
        }, [](int index1, int index2) {
            return rgbaColor{ 0, 0, 0, 255 };
        });
    zoomedImage.writeToFile("map-zoomed.png");
}

TEST_CASE("Test SFMap drawMap colorPicker", "[SFMap][png]") {
    PNG colorpickerImage = sfmap.drawMap([](int index) {
            unsigned char i = 255.0 * index / 174955;
            return rgbaColor{ (unsigned char)(255 - i), 0, i, 255 };
        }, [](int index1, int index2) {
            return rgbaColor{ 0, 0, 0, 255 };
        });
    colorpickerImage.writeToFile("map-colorpicker.png");
}

//check DFS
TEST_CASE("Test escapeRoute with small graph", "[escapeRouteAsVec]") {
    vector<Coord> nodes = FileReader::readRawNode("../tests/small.node.txt");
    vector<pair<int, int>> edges = FileReader::readEdge("../tests/small.edge.txt");
    // call constructor
    SFMap a(nodes, edges);
    vector<int> ans = a.escapeRouteAsVec(nodes[1] , 4);
    cout << ans.size() << endl;
    REQUIRE(ans.at(0) == 2);
    REQUIRE(a.escapeRouteAsVec(nodes[1], 6).at(1) == 3);
    REQUIRE(a.escapeRouteAsVec(nodes[0], 12).back() == 3);
    REQUIRE(a.escapeRouteAsVec(nodes[1], 18).at(2) == 6);
}

// check DFS
TEST_CASE("Test escapeRoute with meidum graph", "[escapeRouteAsVec]") {
    vector<Coord> nodes = FileReader::readRawNode("../tests/medium.node.txt");
    vector<pair<int, int>> edges = FileReader::readEdge("../tests/medium.edge.txt");
    // call constructor
    SFMap b(nodes, edges);
    REQUIRE(b.escapeRouteAsVec(nodes[2], 5).at(0) == 3);
    REQUIRE(b.escapeRouteAsVec(nodes[2], 10).at(1) == 5);
    REQUIRE(b.escapeRouteAsVec(nodes[1], 11).back() == 5);
    REQUIRE(b.escapeRouteAsVec(nodes[2], 12).at(2) == 4);
}

// check DFS
TEST_CASE("Test escapeRoute with large graph", "[escapeRouteAsVec]") {
    vector<Coord> nodes = FileReader::readRawNode("../tests/large.node.txt");
    vector<pair<int, int>> edges = FileReader::readEdge("../tests/large.edge.txt");
    // call constructor
    SFMap c(nodes, edges);
    REQUIRE(c.escapeRouteAsVec(nodes[121], 20).at(0) == 108);
    REQUIRE(c.escapeRouteAsVec(nodes[121], 22).at(1) == 107);
    REQUIRE(c.escapeRouteAsVec(nodes[120], 23).back() == 107);
    REQUIRE(c.escapeRouteAsVec(nodes[121], 24).at(2) == 106);
}

// check Dijkstra
TEST_CASE("Test getParents with small graph", "[getParents]") {
    FileReader fr;
    vector<Coord> nodes = fr.readRawNode("../tests/small.node.txt");
    vector<pair<int, int>> edges = fr.readEdge("../tests/small.edge.txt");
    // call constructor
    SFMap a(nodes, edges);
    vector<int> result = a.getParents(0);
    REQUIRE(result.size() == 10);
    // 0 --> 1
    REQUIRE(result[1] == 0);
    // 0 --> 3 --> 6 --> 7
    REQUIRE(result[7] == 6);
    // 0 --> 1 --> 4 --> 8
    REQUIRE(result[8] == 4);

}

// check Dijkstra
TEST_CASE("Test getParents with medium graph", "[getParents]") {
    FileReader fr;
    vector<Coord> nodes = fr.readRawNode("../tests/medium.node.txt");
    vector<pair<int, int>> edges = fr.readEdge("../tests/medium.edge.txt");
    // call constructor
    SFMap b(nodes, edges);
    vector<int> result = b.getParents(4);
    // node4 - node6
    REQUIRE(result[5] == 4);
    // node4 - node48
    REQUIRE(result[47] == 33.5);
}

// check Dijkstra
TEST_CASE("Test getParents with large graph", "[getParents]") {
    FileReader fr;
    vector<Coord> nodes = fr.readRawNode("../tests/large.node.txt");
    vector<pair<int, int>> edges = fr.readEdge("../tests/large.edge.txt");
    // call constructor
    SFMap c(nodes, edges);
    vector<int> result = c.getParents(121);
    // node121 - node108
    REQUIRE(result[107] == 20);
    // node121 - node70
    REQUIRE(result[70] == 70);
