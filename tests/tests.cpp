#include <catch2/catch_test_macros.hpp>

#include "filereader.h"

TEST_CASE("readNormalizedNode", "[weight=1][part=1]") {
    vector<Coord> nodes = FileReader::readNormalizedNode("../data/SF.cnode.txt");
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
