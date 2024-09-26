#include <catch2/catch_test_macros.hpp>

#include <grapher/predicates.hpp>

TEST_CASE("regex", "[predicates]") {
  grapher::json_t constraint;

  constraint["type"] = "regex";
  constraint["pointer"] = "/foo";
  constraint["regex"] = "bar.*";

  grapher::predicate_t pred = grapher::get_predicate(constraint);

  REQUIRE(pred({{"foo", "bar"}}) == true);
  REQUIRE(pred({{"foo", "rar"}}) == false);
  REQUIRE(pred({{"foo", "barbar"}}) == true);
  REQUIRE(pred({{"foof", "bar"}}) == false);
}

TEST_CASE("match without regex", "[predicates]") {
  grapher::json_t constraint;
  constraint["type"] = "match";
  constraint["regex"] = false;

  constraint["matcher"]["foo"] = "bar";
  constraint["matcher"]["bar"] = "foo";
  constraint["matcher"]["sub"]["okok"] = "bar";

  grapher::predicate_t pred = grapher::get_predicate(constraint);

  grapher::json_t positive_case;
  positive_case["foo"] = "bar";
  positive_case["bar"] = "foo";
  positive_case["sub"]["okok"] = "bar";

  grapher::json_t negative_case;
  negative_case["foo"] = "bar";
  negative_case["bar"] = "foo";
  negative_case["sub"]["okok"] = "babar";

  REQUIRE(pred(positive_case) == true);
  REQUIRE(pred(negative_case) == false);
  REQUIRE(pred({{"foo", "rar"}}) == false);
  REQUIRE(pred({{"foo", "barbar"}}) == false);
  REQUIRE(pred({{"foof", "bar"}}) == false);
}

TEST_CASE("match with regex", "[predicates]") {
  grapher::json_t constraint;
  constraint["type"] = "match";
  constraint["regex"] = true;

  constraint["matcher"]["foo"] = "bar";
  constraint["matcher"]["bar"] = "foo";
  constraint["matcher"]["sub"]["okok"] = "bar.*";

  grapher::predicate_t pred = grapher::get_predicate(constraint);

  grapher::json_t positive_case;
  positive_case["foo"] = "bar";
  positive_case["bar"] = "foo";
  positive_case["sub"]["okok"] = "barbar";

  grapher::json_t negative_case;
  negative_case["foo"] = "bar";
  negative_case["bar"] = "foo";
  negative_case["sub"]["okok"] = "babar";

  REQUIRE(pred(positive_case) == true);
  REQUIRE(pred(negative_case) == false);
  REQUIRE(pred({{"foo", "rar"}}) == false);
  REQUIRE(pred({{"foo", "barbar"}}) == false);
  REQUIRE(pred({{"foof", "bar"}}) == false);
}

TEST_CASE("streq", "[predicates]") {
  grapher::json_t constraint;

  constraint["type"] = "streq";
  constraint["pointer"] = "/sub/okok";
  constraint["string"] = "barbar";

  grapher::predicate_t pred = grapher::get_predicate(constraint);

  grapher::json_t positive_case;
  positive_case["foo"] = "bar";
  positive_case["bar"] = "foo";
  positive_case["sub"]["okok"] = "barbar";

  grapher::json_t negative_case;
  negative_case["foo"] = "baar";
  negative_case["bar"] = "foo";
  negative_case["sub"]["okok"] = "babar";

  REQUIRE(pred(positive_case) == true);
  REQUIRE(pred(negative_case) == false);
}

TEST_CASE("op_or", "[predicates]") {
  {
    grapher::json_t constraint;
    constraint["type"] = "op_or";
    constraint["first"]["type"] = "val_true";
    constraint["second"]["type"] = "val_true";
    REQUIRE(grapher::get_predicate(constraint)({}) == true);
  }

  {
    grapher::json_t constraint;
    constraint["type"] = "op_or";
    constraint["first"]["type"] = "val_false";
    constraint["second"]["type"] = "val_true";
    REQUIRE(grapher::get_predicate(constraint)({}) == true);
  }

  {
    grapher::json_t constraint;
    constraint["type"] = "op_or";
    constraint["first"]["type"] = "val_true";
    constraint["second"]["type"] = "val_false";
    REQUIRE(grapher::get_predicate(constraint)({}) == true);
  }

  {
    grapher::json_t constraint;
    constraint["type"] = "op_or";
    constraint["first"]["type"] = "val_false";
    constraint["second"]["type"] = "val_false";
    REQUIRE(grapher::get_predicate(constraint)({}) == false);
  }
}

TEST_CASE("op_and", "[predicates]") {
  {
    grapher::json_t constraint;
    constraint["type"] = "op_and";
    constraint["first"]["type"] = "val_true";
    constraint["second"]["type"] = "val_true";
    REQUIRE(grapher::get_predicate(constraint)({}) == true);
  }

  {
    grapher::json_t constraint;
    constraint["type"] = "op_and";
    constraint["first"]["type"] = "val_false";
    constraint["second"]["type"] = "val_true";
    REQUIRE(grapher::get_predicate(constraint)({}) == false);
  }

  {
    grapher::json_t constraint;
    constraint["type"] = "op_and";
    constraint["first"]["type"] = "val_true";
    constraint["second"]["type"] = "val_false";
    REQUIRE(grapher::get_predicate(constraint)({}) == false);
  }

  {
    grapher::json_t constraint;
    constraint["type"] = "op_and";
    constraint["first"]["type"] = "val_false";
    constraint["second"]["type"] = "val_false";
    REQUIRE(grapher::get_predicate(constraint)({}) == false);
  }
}

TEST_CASE("val_true", "[predicates]") {
  grapher::json_t constraint;
  constraint["type"] = "val_true";
  grapher::predicate_t pred = grapher::get_predicate(constraint);
  REQUIRE(pred({}) == true);
}

TEST_CASE("val_false", "[predicates]") {
  grapher::json_t constraint;
  constraint["type"] = "val_false";
  grapher::predicate_t pred = grapher::get_predicate(constraint);
  REQUIRE(pred({}) == false);
}
