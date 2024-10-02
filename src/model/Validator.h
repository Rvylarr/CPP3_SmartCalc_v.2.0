#ifndef CPP3_SMARTCALC_V2_0_1_VALIDATOR_H
#define CPP3_SMARTCALC_V2_0_1_VALIDATOR_H

#include <iostream>

#include "Filter.h"

using namespace std;

namespace s21 {
class Validator {
 public:
  // --- constructors --- //
  Validator() = delete;
  Validator(string &expression)
      : filter_(Filter::get_instance()), expression_(expression) {}
  Validator(string &expression, string &input_option)
      : filter_(Filter::get_instance()),
        expression_(expression),
        input_option_(input_option) {}
  ~Validator() = default;

  // --- check expr methods --- //
  string check_expression();
  void check_brackets(string &last_elem);
  void check_erase(string &last_elem);
  void check_unary();
  void final_check();
  string input_x(string &x);

 private:
  Filter &filter_;
  string expression_;
  string input_option_;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_VALIDATOR_H
