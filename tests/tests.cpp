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
    // build small graph
    vector<Coord> nodes;
    struct Coord coord1;
    coord1.lat_ = 6;
    coord1.long_ = 1;
    nodes.push_back(coord1);

    struct Coord coord2;
    coord1.lat_ = 5.1;
    coord1.long_ = 2;
    nodes.push_back(coord2);

    struct Coord coord3;
    coord1.lat_ = 2;
    coord1.long_ = 1.8;
    nodes.push_back(coord3);

    struct Coord coord4;
    coord1.lat_ = 7;
    coord1.long_ = 6;
    nodes.push_back(coord4);

    struct Coord coord5;
    coord1.lat_ = 4;
    coord1.long_ = 2;
    nodes.push_back(coord5);

    struct Coord coord6;
    coord1.lat_ = 1;
    coord1.long_ = 7;
    nodes.push_back(coord6);

    struct Coord coord7;
    coord1.lat_ = 9;
    coord1.long_ = 9;
    nodes.push_back(coord7);

    struct Coord coord8;
    coord1.lat_ = 9;
    coord1.long_ = 10;
    nodes.push_back(coord8);

    struct Coord coord9;
    coord1.lat_ = 3;
    coord1.long_ = 8;
    nodes.push_back(coord9);

    struct Coord coord10;
    coord1.lat_ = 2;
    coord1.long_ = 10;
    nodes.push_back(coord10);

    vector<pair<int, int>> edges;
    // start at 1
    pair<int, int> edge1;
    edge1.first = 1;
    edge1.second = 2;
    edges.push_back(edge1);
   
    pair<int, int> edge2;
    edge1.first = 1;
    edge1.second = 3;
    edges.push_back(edge2);

    pair<int, int> edge3;
    edge1.first = 1;
    edge1.second = 4;
    edges.push_back(edge3);
    // start at 2
    pair<int, int> edge4;
    edge1.first = 2;
    edge1.second = 3;
    edges.push_back(edge4);

    pair<int, int> edge5;
    edge1.first = 2;
    edge1.second = 5;
    edges.push_back(edge5);
    // start at 3
    pair<int, int> edge6;
    edge1.first = 3;
    edge1.second = 6;
    edges.push_back(edge6);
    // start at 4
    pair<int, int> edge7;
    edge1.first = 4;
    edge1.second = 5;
    edges.push_back(edge7);

    pair<int, int> edge8;
    edge1.first = 4;
    edge1.second = 7;
    edges.push_back(edge8);
    // start at 5
    pair<int, int> edge9;
    edge1.first = 5;
    edge1.second = 9;
    edges.push_back(edge9);
    
    pair<int, int> edge10;
    edge1.first = 5;
    edge1.second = 6;
    edges.push_back(edge10);
    // start at 6
    pair<int, int> edge11;
    edge1.first = 6;
    edge1.second = 10;
    edges.push_back(edge11);
    // start at 7
    pair<int, int> edge12;
    edge1.first = 7;
    edge1.second = 8;
    edges.push_back(edge12);

    pair<int, int> edge13;
    edge1.first = 7;
    edge1.second = 9;
    edges.push_back(edge13);
    // start at 8
    pair<int, int> edge14;
    edge1.first = 8;
    edge1.second = 9;
    edges.push_back(edge14);

    pair<int, int> edge15;
    edge1.first = 8;
    edge1.second = 10;
    edges.push_back(edge15);
    // call constructor
    SFMap a(nodes, edges);
    REQUIRE(a.escapeRouteAsVec(coord1 , 4).at(1)->index == 2);
    REQUIRE(a.escapeRouteAsVec(coord1 , 6).at(2)->index == 3);
    REQUIRE(a.escapeRouteAsVec(coord1 , 12).at(a.escapeRouteAsVec(coord1, 12).size() - 1)->index == 3);
    REQUIRE(a.escapeRouteAsVec(coord1 , 18).at(3)->index == 6);
}
// check DFS
TEST_CASE("Test escapeRoute with meidum graph", "[escapeRoute]") {
    // build medium graph
    vector<Coord> nodes;
    // 1-10
    struct Coord coord1;
    coord1.lat_ = 4.8;
    coord1.long_ = 10;
    nodes.push_back(coord1);

    struct Coord coord2;
    coord1.lat_ = 9.8;
    coord1.long_ = 1;
    nodes.push_back(coord1);

    struct Coord coord3;
    coord1.lat_ = 7;
    coord1.long_ = 1;
    nodes.push_back(coord3);

    struct Coord coord4;
    coord1.lat_ = 9;
    coord1.long_ = 2;
    nodes.push_back(coord4);

    struct Coord coord5;
    coord1.lat_ = 7.3;
    coord1.long_ = 2.2;
    nodes.push_back(coord5);

    struct Coord coord6;
    coord1.lat_ = 9.9;
    coord1.long_ = 3;
    nodes.push_back(coord6);

    struct Coord coord7;
    coord1.lat_ = 6.9;
    coord1.long_ = 2.8;
    nodes.push_back(coord7);

    struct Coord coor8;
    coord1.lat_ = 5;
    coord1.long_ = 1.5;
    nodes.push_back(coord8);

    struct Coord coord9;
    coord1.lat_ = 3;
    coord1.long_ = 4;
    nodes.push_back(coord9);

    struct Coord coord10;
    coord1.lat_ = 2;
    coord1.long_ = 1.3;
    nodes.push_back(coord10);
    
    // 11-20
    struct Coord coord11;
    coord1.lat_ = 2.6;
    coord1.long_ = 2.2;
    nodes.push_back(coord11);

    struct Coord coord12;
    coord1.lat_ = 4.1;
    coord1.long_ = 2.1;
    nodes.push_back(coord12);

    struct Coord coord13;
    coord1.lat_ = 1.8;
    coord1.long_ = 3;
    nodes.push_back(coord13);

    struct Coord coord14;
    coord1.lat_ = 5.4;
    coord1.long_ = 3.2;
    nodes.push_back(coord14);

    struct Coord coord15;
    coord1.lat_ = 2.4;
    coord1.long_ = 2.8;
    nodes.push_back(coord15);

    struct Coord coord16;
    coord1.lat_ = 2.6;
    coord1.long_ = 3.5;
    nodes.push_back(coord16);

    struct Coord coord17;
    coord1.lat_ = 2.5;
    coord1.long_ = 4;
    nodes.push_back(coord1);

    struct Coord coord18;
    coord1.lat_ = 6.9;
    coord1.long_ = 3.8;
    nodes.push_back(coord18);

    struct Coord coord19;
    coord1.lat_ = 9.5;
    coord1.long_ = 3;
    nodes.push_back(coord19);

    struct Coord coord20;
    coord1.lat_ = 9.7;
    coord1.long_ = 4.2;
    nodes.push_back(coord20);

    // 21-30
    struct Coord coord21;
    coord1.lat_ = 9.75;
    coord1.long_ = 4.5;
    nodes.push_back(coord21);

    struct Coord coord22;
    coord1.lat_ = 7;
    coord1.long_ = 5;
    nodes.push_back(coord22);

    struct Coord coord23;
    coord1.lat_ = 5.4;
    coord1.long_ = 4.3;
    nodes.push_back(coord23);

    struct Coord coord24;
    coord1.lat_ = 7.2;
    coord1.long_ = 4.3;
    nodes.push_back(coord24);

    struct Coord coord25;
    coord1.lat_ = 2.4;
    coord1.long_ = 6;
    nodes.push_back(coord25);

    struct Coord coord26;
    coord1.lat_ = 7;
    coord1.long_ = 6;
    nodes.push_back(coord26);

    struct Coord coord27;
    coord1.lat_ = 5;
    coord1.long_ = 5.8;
    nodes.push_back(coord27);

    struct Coord coor28;
    coord1.lat_ = 4.8;
    coord1.long_ = 6.2;
    nodes.push_back(coord28);

    struct Coord coord29;
    coord1.lat_ = 4.7;
    coord1.long_ = 5.9;
    nodes.push_back(coord29);

    struct Coord coord30;
    coord1.lat_ = 2.4;
    coord1.long_ = 4.4;
    nodes.push_back(coord30);

    // 31-40
    struct Coord coord31;
    coord1.lat_ = 4.8;
    coord1.long_ = 10;
    nodes.push_back(coord1);

    struct Coord coord32;
    coord1.lat_ = 4.7;
    coord1.long_ = 4.9;
    nodes.push_back(coord1);

    struct Coord coord33;
    coord1.lat_ = 7;
    coord1.long_ = 1;
    nodes.push_back(coord3);

    struct Coord coord34;
    coord1.lat_ = 9;
    coord1.long_ = 2;
    nodes.push_back(coord4);

    struct Coord coord35;
    coord1.lat_ = 7.3;
    coord1.long_ = 2.2;
    nodes.push_back(coord5);

    struct Coord coord36;
    coord1.lat_ = 9.9;
    coord1.long_ = 3;
    nodes.push_back(coord6);

    struct Coord coord37;
    coord1.lat_ = 6.9;
    coord1.long_ = 2.8;
    nodes.push_back(coord7);

    struct Coord coor38;
    coord1.lat_ = 5;
    coord1.long_ = 1.5;
    nodes.push_back(coord8);

    struct Coord coord39;
    coord1.lat_ = 3;
    coord1.long_ = 4;
    nodes.push_back(coord9);

    struct Coord coord40;
    coord1.lat_ = 2;
    coord1.long_ = 1.3;
    nodes.push_back(coord10);
}
