#include <catch2/catch.hpp>

#include <vector>

#include <grapher/core.hpp>
#include <grapher/utils/math.hpp>

TEST_CASE("average", "[math]") {
  REQUIRE(grapher::math::average({0}) == 0);
  REQUIRE(grapher::math::average({0, 10, 20, 30, 40}) == 20);
  REQUIRE(grapher::math::average({10, 10, 20, 30, 40}) == 22);
  REQUIRE(grapher::math::average({10, 10, 10, 10, 10}) == 10);
}

TEST_CASE("median on sorted vectors", "[math]") {
  REQUIRE(grapher::math::median({10}) == 10);
  REQUIRE(grapher::math::median({0, 10}) == 05);
  REQUIRE(grapher::math::median({0, 10, 20, 30, 40, 50}) == 25);
  REQUIRE(grapher::math::median({0, 10, 20, 30, 40}) == 20);
  REQUIRE(grapher::math::median({0, 10, 20, 20, 30, 40}) == 20);
  REQUIRE(grapher::math::median({0, 10, 20, 30, 30, 40}) == 25);
}

TEST_CASE("median on unsorted vectors", "[math]") {
  REQUIRE(grapher::math::median({10}) == 10);
  REQUIRE(grapher::math::median({10, 0}) == 05);
  REQUIRE(grapher::math::median({0, 10, 40, 20, 30, 50}) == 25);
  REQUIRE(grapher::math::median({0, 40, 20, 30, 10}) == 20);
  REQUIRE(grapher::math::median({10, 0, 40, 20, 30, 20}) == 20);
  REQUIRE(grapher::math::median({40, 30, 10, 30, 20, 0}) == 25);
}
