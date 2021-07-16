#define TTS_MAIN
#include <tts/tts.hpp>

#include "grapher/json-utils.hpp"

TTS_CASE("find_matching - Basic inclusion test") {
  nlohmann::json json_a = {{"a", 42}};
  nlohmann::json json_b = {{"b", 42}};
  nlohmann::json json_c = {{"c", "42"}};

  nlohmann::json json_ac = {{"a", 42}, {"c", "42"}};
  nlohmann::json json_bc = {{"b", 42}, {"c", "42"}};

  nlohmann::json json_d = {{"d", 42}};
  nlohmann::json json_ab = {{"a", 42}, {"b", 42}};

  nlohmann::json json_ac_bc;

  json_ac_bc.push_back(json_ac);
  json_ac_bc.push_back(json_bc);

  // Success cases

  TTS_EQUAL(
      grapher::find_matching(json_ac_bc.begin(), json_ac_bc.end(), json_a),
      json_ac_bc.cbegin());

  TTS_EQUAL(
      grapher::find_matching(json_ac_bc.begin(), json_ac_bc.end(), json_b),
      json_ac_bc.cbegin() + 1);

  TTS_EQUAL(
      grapher::find_matching(json_ac_bc.begin(), json_ac_bc.end(), json_c),
      json_ac_bc.cbegin());

  TTS_EQUAL(
      grapher::find_matching(json_ac_bc.begin(), json_ac_bc.end(), json_ac),
      json_ac_bc.cbegin());

  TTS_EQUAL(
      grapher::find_matching(json_ac_bc.begin(), json_ac_bc.end(), json_bc),
      json_ac_bc.cbegin() + 1);

  // Failure cases

  TTS_EQUAL(
      grapher::find_matching(json_ac_bc.begin(), json_ac_bc.end(), json_d),
      json_ac_bc.cend());

  TTS_EQUAL(
      grapher::find_matching(json_ac_bc.begin(), json_ac_bc.end(), json_ab),
      json_ac_bc.cend());
};
