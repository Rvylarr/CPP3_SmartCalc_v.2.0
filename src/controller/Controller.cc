#include "Controller.h"

namespace s21 {
string Controller::check_validity(string &expression, string &input_option) {
  Validator v(expression, input_option);
  return v.check_expression();
}

string Controller::calculate_basic(string &expression) {
  try {
    Validator ve(expression);
    string result;

    ve.final_check();
    Calculator c(expression);
    result = c.calculate_expression();
    return result;
  } catch (string &error_mes) {
    return error_mes;
  }
}

string Controller::calculate_with_x(string &expression, string &x) {
  try {
    Validator ve(expression);
    Validator vx(x);
    string result;

    ve.final_check();
    vx.final_check();
    Calculator cx(x);
    string x_num = cx.calculate_expression();
    string xpression = ve.input_x(x_num);
    Calculator c(xpression);
    result = c.calculate_expression();
    return result;
  } catch (string &error_mes) {
    return error_mes;
  }
}

string Controller::calculate_credit(long double amount, short period, tm date,
                                    short frequency, double persent, bool type,
                                    vector<long double> *result) {
  try {
    if (period < 1) throw std::out_of_range("invalid period");
    if (amount <= 0) throw std::out_of_range("amount is empty");
    if (persent <= 0) throw std::out_of_range("persent is lower than 0.1");
    if (period % frequency != 0) throw std::logic_error("payment frequency ");

    Credit c(amount, period, date, persent, frequency);
    type ? c.calculate_annuity(result) : c.calculate_different(result);
    return "";
  } catch (string &error_mes) {
    return error_mes;
  }
}

string Controller::calculate_deposit(vector<long double> *sums,
                                     vector<tm> *dates, vector<string> *types,
                                     double amount, short period,
                                     bool capitalisation) {
  try {
    if (sums->empty()) throw std::out_of_range("invalid input");
    if (amount <= 0) throw std::logic_error("Deposit amount is empty");
    if (period < 1) throw std::out_of_range("invalid period");
    Deposit d(amount, period, capitalisation);
    d.create_events_list(*sums, *dates, *types);
    sums->clear();
    *sums = d.calculate_deposit();
    return "";
  } catch (string &error_mes) {
    return error_mes;
  }
}
}  // namespace s21