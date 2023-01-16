#include <boost/preprocessor/repetition/enum.hpp>

/// Compile-time unsigned int
template <unsigned N> struct ct_uint_t { static constexpr unsigned value = N; };

/// Expansion compile-time sum implementation
template <typename... Ts> constexpr unsigned sum(Ts const &...) {
  return (Ts::value + ...);
}

// Driver code

#define GEN_MACRO(Z, N, TEXT)                                                  \
  TEXT<N> {}

constexpr unsigned foo() {
  return sum(BOOST_PP_ENUM(BENCHMARK_SIZE, GEN_MACRO, ct_uint_t));
}

[[maybe_unused]] constexpr unsigned result = foo();
