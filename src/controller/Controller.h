#ifndef CPP3_SMARTCALC_V2_0_1_SRC_CONTROLLER_CONTROLLER_HPP_
#define CPP3_SMARTCALC_V2_0_1_SRC_CONTROLLER_CONTROLLER_HPP_

#include "../model/Calculator.h"
#include "../model/Credit.h"
#include "../model/Deposit.h"
#include "../model/Validator.h"

using namespace std;

namespace s21 {
class Controller {
 public:
  Controller() = default;
  ~Controller() = default;

  // --- controller methods --- //
  static string check_validity(string &expression, string &input_option);
  static string calculate_basic(string &expression);
  static string calculate_with_x(string &expression, string &x);

  static string calculate_credit(long double amount, short period, tm date,
                                 short frequency, double persent, bool type,
                                 vector<long double> *result);
  static string calculate_deposit(vector<long double> *sums, vector<tm> *dates,
                                  vector<string> *types, double amount,
                                  short period, bool capitalisation);
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_SRC_CONTROLLER_CONTROLLER_HPP_
