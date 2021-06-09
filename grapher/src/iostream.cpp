#include <grapher/iostream.hpp>

namespace grapher {

std::ostream &to_csv(std::ostream &o, entry_t const &) { return o; }

std::ostream &to_csv(std::ostream &o, category_t const &cat) { return o; }

} // namespace grapher
