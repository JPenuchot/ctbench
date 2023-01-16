#include <boost/preprocessor/repetition/enum.hpp>

/// Compile-time unsigned int
template <unsigned N> struct ct_uint_t { static constexpr unsigned value = N; };

/// Expansion compile-time sum implementation
template <typename... Ts> auto sum(Ts const &...) { return (Ts::value + ...); }

// Driver code

#define GEN_MACRO(Z, N, TEXT)                                                  \
  TEXT<N> {}

unsigned foo() {
  return sum(BOOST_PP_ENUM(BENCHMARK_SIZE, GEN_MACRO, ct_uint_t));
}
