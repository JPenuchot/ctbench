#define TTS_MAIN
#include <tts/tts.hpp>

#include "grapher/utils/json.hpp"

/*

TTS_CASE("find_matching - Basic") {
  grapher::json_t json_a = {{"a", 42}};
  grapher::json_t json_b = {{"b", 42}};
  grapher::json_t json_c = {{"c", "42"}};

  grapher::json_t json_ac = {{"a", 42}, {"c", "42"}};
  grapher::json_t json_bc = {{"b", 42}, {"c", "42"}};

  grapher::json_t json_d = {{"d", 42}};
  grapher::json_t json_ab = {{"a", 42}, {"b", 42}};

  grapher::json_t json_ac_bc;

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

TTS_CASE("find_matching - Imbricated JSON") {
  grapher::json_t json_a = {{"a", 42}};
  grapher::json_t json_b = {{"b", 42}};
  grapher::json_t json_c = {{"c", "42"}};

  grapher::json_t json_jsonc = {{"json_c", json_c}};

  grapher::json_t json_ab_jsonc = {{"a", 42}, {"b", 42}, {"json_c", json_c}};
  grapher::json_t json_a_jsonc = {{"a", 42}, {"json_c", json_c}};
  grapher::json_t json_b_jsonc = {{"b", 42}, {"json_c", json_c}};

  {
    grapher::json_t list;

    list.push_back(json_a);
    list.push_back(json_b);
    list.push_back(json_ab_jsonc);
    list.push_back(json_a_jsonc);
    list.push_back(json_b_jsonc);

    TTS_EQUAL(grapher::find_matching(list.begin(), list.end(), json_jsonc),
              list.cbegin() + 2);

    TTS_EQUAL(grapher::find_matching(list.begin(), list.end(), json_c),
              list.cend());

    TTS_EQUAL(grapher::find_matching(list.begin(), list.end(), json_a),
              list.cbegin());
  }

  {
    grapher::json_t list;

    list.push_back(json_a);
    list.push_back(json_a_jsonc);
    list.push_back(json_b);
    list.push_back(json_b_jsonc);

    TTS_EQUAL(grapher::find_matching(list.begin(), list.end(), json_jsonc),
              list.cbegin() + 1);

    TTS_EQUAL(grapher::find_matching(list.begin(), list.end(), json_c),
              list.cend());

    TTS_EQUAL(grapher::find_matching(list.begin(), list.end(), json_a),
              list.cbegin());
  }

  {
    grapher::json_t list;

    list.push_back(json_a);
    list.push_back(json_b);

    TTS_EQUAL(grapher::find_matching(list.begin(), list.end(), json_jsonc),
              list.cend());

    TTS_EQUAL(grapher::find_matching(list.begin(), list.end(), json_c),
              list.cend());

    TTS_EQUAL(grapher::find_matching(list.begin(), list.end(), json_a),
              list.cbegin());
  }
};

TTS_CASE("merge_into - Basic") {
  grapher::json_t a, b;

  a["a"] = 0;
  a["b"] = 0;

  b["b"] = 1;
  b["c"] = 1;

  auto c = grapher::merge_into(a, b);

  TTS_EQUAL(c["a"], 0);
  TTS_EQUAL(c["b"], 1);
  TTS_EQUAL(c["c"], 1);
};

// TODO: merge_into with arrays and imbricated JSON objects

*/
