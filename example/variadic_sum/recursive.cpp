#include <boost/preprocessor/repetition/enum.hpp>

/// Compile-time unsigned int
template <unsigned N> struct ct_uint_t { static constexpr unsigned value = N; };

/// Recursive compile-time sum implementation
template <typename T> constexpr auto sum(T const &) { return T::value; }
template <typename T, typename... Ts>
constexpr auto sum(T const &, Ts const &...tl) {
  return T::value + sum(tl...);
}

// Driver code

#define GEN_MACRO(Z, N, TEXT)                                                  \
  TEXT<N> {}

constexpr unsigned foo() {
  return sum(BOOST_PP_ENUM(BENCHMARK_SIZE, GEN_MACRO, ct_uint_t));
}

[[maybe_unused]] constexpr unsigned result = foo();
