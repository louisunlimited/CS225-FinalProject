#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include "filereader.h"
#include "sf_map.h"

using namespace std;

double INF = numeric_limits<double>::infinity();

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

void configSmallGraph(SFMap& m) {
    m.setScale(50);
    m.setMargin(1);
    m.setRadius(4);
    m.setLineWidth(2);
    m.setMetric(normalizedDist);
}

void matchEdges(const vector<pair<int, int>>& result, const vector<pair<int, int>>& ans, int n) {
    vector<vector<bool>> resultMatrix = vector(n, vector(n, false));
    for (auto [a, b] : result) {
        resultMatrix[a][b] = true;
        resultMatrix[b][a] = true;
    }

    vector<vector<bool>> ansMatrix = vector(n, vector(n, false));
    for (auto [a, b] : ans) {
        ansMatrix[a][b] = true;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            REQUIRE(resultMatrix[i][j] == ansMatrix[i][j]);
        }
    }
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
    configSmallGraph(smallmap);

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

void checkRoute(const vector<int>& route, const vector<Coord>& nodes, double minDist) {
    double sum = 0;
    for (int i = 0; i < (int)route.size() - 1; i++) {
        sum += normalizedDist(nodes[route[i]], nodes[route[i+1]]);
    }
    REQUIRE(sum >= minDist);
}

TEST_CASE("Test escapeRoute with small graph", "[escapeRouteAsVec][DFS]") {
    vector<Coord> nodes = FileReader::readRawNode("../tests/small.node.txt");
    vector<pair<int, int>> edges = FileReader::readEdge("../tests/small.edge.txt");
    // call constructor
    SFMap a(nodes, edges);
    configSmallGraph(a);
    // nodes[1] -- nodes[2] = 1.345
    vector<int> ans1 = a.escapeRouteAsVec(nodes[1] , 1);
    checkRoute(ans1, nodes, 1);
    // nodes[1] -- nodes[2] -- nodes[3] = 4.451
    vector<int> ans2 = a.escapeRouteAsVec(nodes[1] , 4);
    checkRoute(ans2, nodes, 4);
}

TEST_CASE("Test escapeRoute with medium graph", "[escapeRouteAsVec][DFS]") {
    vector<Coord> nodes = FileReader::readRawNode("../tests/medium.node.txt");
    vector<pair<int, int>> edges = FileReader::readEdge("../tests/medium.edge.txt");
    // call constructor
    SFMap b(nodes, edges);
    configSmallGraph(b);
    // nodes[2] -- nodes[3] = 2.8
    vector<int> ans1 = b.escapeRouteAsVec(nodes[2] , 2.5);
    checkRoute(ans1, nodes, 2.5);
    // nodes[2] -- nodes[3] -- nodes[5] = 3.8
    vector<int> ans2 = b.escapeRouteAsVec(nodes[2] , 3.6);
    checkRoute(ans2, nodes, 3.6);
}

TEST_CASE("Test escapeRoute with large graph", "[escapeRouteAsVec][DFS]") {
    vector<Coord> nodes = FileReader::readRawNode("../tests/large.node.txt");
    vector<pair<int, int>> edges = FileReader::readEdge("../tests/large.edge.txt");
    // call constructor
    SFMap c(nodes, edges);
    configSmallGraph(c);
    // nodes[34] -- nodes[33] = 0.223
    vector<int> ans1 = c.escapeRouteAsVec(nodes[34] , 0.1);
    checkRoute(ans1, nodes, 0.1);
    // nodes[34] -- nodes[33] -- nodes[32] = 0.67
    vector<int> ans2 = c.escapeRouteAsVec(nodes[34] , 0.5);
    checkRoute(ans2, nodes, 0.5);
}

TEST_CASE("Test getParents with small graph", "[getParents][Dijkstra]") {
    vector<Coord> nodes = FileReader::readRawNode("../tests/small.node.txt");
    vector<pair<int, int>> edges = FileReader::readEdge("../tests/small.edge.txt");
    // call constructor
    SFMap a(nodes, edges);
    configSmallGraph(a);
    vector<int> result = a.getParents(1, INF);
    REQUIRE(result.size() == 10);
    // parent of the start node should be -1
    REQUIRE(result[1] == -1);
    // 1 --> 0
    REQUIRE(result[0] == 1);
    // 1 --> 4 --> 3 --> 6 --> 7
    REQUIRE(result[7] == 6);
    // 1 --> 4 --> 8
    REQUIRE(result[8] == 4);
}

TEST_CASE("Test getParents with medium graph", "[getParents][Dijkstra]") {
    vector<Coord> nodes = FileReader::readRawNode("../tests/medium.node.txt");
    vector<pair<int, int>> edges = FileReader::readEdge("../tests/medium.edge.txt");
    // call constructor
    SFMap b(nodes, edges);
    configSmallGraph(b);
    vector<int> result = b.getParents(3, INF);
    REQUIRE(result.size() == 50);
    // parent of the start node should be -1
    REQUIRE(result[3] == -1);
    // 3 --> 5
    REQUIRE(result[5] == 3);
    // 3 --> ... --> 46 --> 47
    REQUIRE(result[46] == 45);
    // 3 --> 4 --> 7 --> 8
    REQUIRE(result[8] == 7);
}

TEST_CASE("Test SFMap nextPoliceStationAsIndex", "[nextPoliceStation]") {
    int node = sfmap.nextPoliceStationAsIndex();
    cout << "Result: node " << node << endl;
}

TEST_CASE("Test SFMap nextPoliceStation as PNG", "[nextPoliceStation][png]") {
    PNG image = sfmap.nextPoliceStation(1.0);
    image.writeToFile("police-station.png");
}

TEST_CASE("Test SFMap 10 nextPoliceStation as PNG", "[nextPoliceStation][png]") {
    vector<Coord> coords = FileReader::readRawNode("../data/SF.cnode.txt");
    Coord anchor1(37.5108, -122.1117);
    Coord normalizedAnchor1(5037.15, 4518.17);
    Coord anchor2(37.9956, -123.0232);
    Coord normalizedAnchor2(2261.93, 0);
    FileReader::convertNode(coords, anchor1, normalizedAnchor1, anchor2, normalizedAnchor2);

    SFMap sfmap_ = loadMap();
    for (int i = 0; i < 9; i++) {
        int node = sfmap_.nextPoliceStationAsIndex();
        cout << "Result: node " << node << endl;
        sfmap_.addPoliceStation(coords[node]);
    }
    PNG image = sfmap_.nextPoliceStation(1.0);
    image.writeToFile("police-station-10.png");
}

// TEST_CASE("Test SFMap nextPoliceStationAsIndexSlow", "[SFMap][nextPoliceStationSlow]") {
//     int index = 0;
//     auto [targets, ecc] = sfmap.nextPoliceStationAsIndexSlow(index, 6);
//     // Save it to file
//     cout << "Size: " << targets.size() << endl;
//     ofstream out("potential-" + to_string(index) + ".txt");
//     out << "eccentricity: " << ecc << endl;
//     for (int target : targets) out << target << endl;
//     out.close();
// }

TEST_CASE("Test importance as PNG with medium graph", "[importance][png]") {
    vector<Coord> nodes = FileReader::readRawNode("../tests/medium.node.txt");
    vector<pair<int, int>> edges = FileReader::readEdge("../tests/medium.edge.txt");
    SFMap m(nodes, edges);
    configSmallGraph(m);
    PNG image = m.importance();
    image.writeToFile("importance-medium.png");
}

TEST_CASE("Test importance as PNG with large graph", "[importance][png]") {
    vector<Coord> nodes = FileReader::readRawNode("../tests/large.node.txt");
    vector<pair<int, int>> edges = FileReader::readEdge("../tests/large.edge.txt");
    SFMap m(nodes, edges);
    configSmallGraph(m);
    PNG image = m.importance();
    image.writeToFile("importance-large.png");
}

// TEST_CASE("Test importance as PNG", "[importance][png]") {
//     PNG image = sfmap.importance();
//     image.writeToFile("importance.png");
// }

TEST_CASE("Test escapeRoute as GIF", "[escapeRoute][gif]") {
    Animation animation = sfmap.escapeRoute(Coord(37.5108, -122.1117), 10, 18);
    animation.write("escapeRoute.gif");
    cout << "GIF saved." << endl;
}

void checkEqual(const vector<pair<int, int>>& list1, const vector<pair<int, int>>& list2) {
    set<pair<int, int>> set1(list1.cbegin(), list1.cend());
    set<pair<int, int>> set2(list2.cbegin(), list2.cend());
    REQUIRE(set1 == set2);
}

TEST_CASE("Test getMST with small graph", "[mst][prim]") {
    vector<Coord> nodes = FileReader::readRawNode("../tests/small.node.txt");
    vector<pair<int, int>> edges = FileReader::readEdge("../tests/small.edge.txt");
    SFMap m(nodes, edges);
    configSmallGraph(m);

    vector<pair<int, int>> result = m.getMST();
    vector<pair<int, int>> ans{ {0, 1}, {1, 4}, {1, 2}, {4, 3}, {3, 6}, {6, 7}, {2, 5}, {4, 8} };
    checkEqual(result, ans);
}

TEST_CASE("Test accessPoint as PNG", "[accessPoint][png]") {
    PNG image = sfmap.accessPoint();
    image.writeToFile("access-point.png");
}
