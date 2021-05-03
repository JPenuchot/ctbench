#include <grapher/iostream.hpp>

namespace grapher {

std::ostream &to_csv(std::ostream &o, entry_t const &e) {
  o << ',' << e.size << ',' << e.execute_compiler << ',' << e.frontend << ','
    << e.source << ',' << e.instantiate_function << ',' << e.parse_class << ','
    << e.instantiate_class << ',' << e.backend << ',' << e.opt_module << ','
    << e.parse_template << ',' << e.opt_function << ',' << e.run_pass << ','
    << e.per_module_passes << ',' << e.perform_pending_instantiations << ','
    << e.run_loop_pass << ',' << e.code_gen_passes << ',' << e.code_gen_function
    << ',' << e.per_function_passes;

  return o;
}

std::ostream &to_csv(std::ostream &o, category_t const &cats) {
  o << "category,"
       "name,"
       "execute_compiler,"
       "frontend,"
       "source,"
       "instantiate_function,"
       "parse_class,"
       "instantiate_class,"
       "backend,"
       "opt_module,"
       "parse_template,"
       "opt_function,"
       "run_pass,"
       "per_module_passes,"
       "perform_pending_instantiations,"
       "run_loop_pass,"
       "code_gen_passes,"
       "code_gen_function,"
       "per_function_passes\n";
  for (auto const &[k_cat, v_cat] : cats) {
    for (auto const &e : v_cat) {
      to_csv((o << k_cat), e) << '\n';
    }
  }
  return o;
}

} // namespace grapher
