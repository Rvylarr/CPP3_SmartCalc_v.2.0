#include "Filter.h"

namespace s21 {
Filter &Filter::get_instance() {
  static Filter instance;
  return instance;
}

const regex &Filter::get_filter(const string &id) const {
  auto it = filter_.find(id);
  if (it == filter_.end())
    throw std::logic_error("[get_filter] : id \"" + id + "\" not found");
  return it->second;
}
}  // namespace s21