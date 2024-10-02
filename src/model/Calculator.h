#ifndef CPP3_SMARTCALC_V2_0_1_SRC_MODEL_CALCULATOR_HPP_
#define CPP3_SMARTCALC_V2_0_1_SRC_MODEL_CALCULATOR_HPP_

#include <cmath>
#include <iostream>
#include <regex>
#include <stack>

#include "Filter.h"

using namespace std;

namespace s21 {
enum Operations {
  BRACKET_OPEN,
  BRACKET_CLOSE,
  POW,
  SIN,
  COS,
  TG,
  ASIN,
  ACOS,
  ATG,
  LN,
  LOG,
  SQRT,
  MOD,
  MUL,
  DIV,
  PLUS,
  MIN,
  NONE
};

enum Priority { BRACKETS, TRIGONOMETRY, DIGREE, MUL_DIV, PLUS_MIN };

class Calculator {
 public:
  struct Operation;

  // --- constructors --- //
  Calculator() = delete;
  explicit Calculator(string &expression)
      : filter_(Filter::get_instance()), expression_(expression) {}
  ~Calculator() = default;

  // --- calculation methods --- //
  string calculate_expression();
  void round(string &expression);
  void parsing();
  void operation_in_brackets(const string &match);
  void operation_without_brackets(const string &match);
  void break_brackets();
  void calculation();

  struct Operation {
   public:
    Operation() = default;
    Operation(Operations operation, Priority priority)
        : operation_(operation), priority_(priority){};
    ~Operation() = default;

    Operations get_operation();
    Priority get_priority();

   private:
    Operations operation_;
    Priority priority_;
  };

 private:
  Filter &filter_;
  string expression_;
  stack<Operation> op_stack;
  stack<double> num_stack;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_SRC_MODEL_CALCULATOR_HPP_
