#ifndef CPP3_SMARTCALC_V2_0_1_SRC_MODEL_FILTER_H
#define CPP3_SMARTCALC_V2_0_1_SRC_MODEL_FILTER_H

#include <regex>
#include <unordered_map>

using namespace std;

namespace s21 {
class Filter {
 public:
  Filter(Filter const&) = delete;
  void operator=(Filter const&) = delete;

  static Filter& get_instance();
  const regex& get_filter(const string& id) const;

 private:
  Filter() = default;

  const unordered_map<string, regex> filter_ = {
      {"all",
       regex(
           R"((\d+(\.\d+)?|[+\-*/^%()]|x|sin|cos|tg|asin|acos|atg|log|ln|sqrt))")},
      {"simple_op", regex("[+\\-*/^%]")},
      {"num", regex(R"(([-+]?)(\d+)(\.\d+)?)")},
      {"trig", regex("[sin|cos|tg|asin|acos|atg|log|ln|sqrt]")},
      {"open_bracket", regex("[(]")},
      {"close_bracket", regex("[)]")},
      {"dot", regex("[.]")},
      {"x", regex("[x]")}};
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_SRC_MODEL_FILTER_H
