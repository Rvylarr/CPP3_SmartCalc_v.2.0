#ifndef CPP3_SMARTCALC_V2_0_1_SRC_MODEL_DEPOSIT_H
#define CPP3_SMARTCALC_V2_0_1_SRC_MODEL_DEPOSIT_H

#include <cmath>
#include <ctime>
#include <iostream>
#include <regex>
#include <vector>

#include "Calendar.h"

using namespace std;

namespace s21 {
class Deposit {
 public:
  struct event {
    long double sum;
    tm date;
    string type;
  };

  // --- constructors --- //
  Deposit() = delete;
  Deposit(long double amount, short period, bool capitalisation)
      : amount_(amount), period_(period), capitalisation_(capitalisation) {}
  ~Deposit() = default;

  // --- build and check exceptions --- //
  vector<long double> calculate_deposit();
  void create_events_list(vector<long double> sums, vector<tm> dates,
                          vector<string> types);
  void sort_events();
  void check_events();

  // --- deposit calculation --- //
  vector<long double> run_calendar();
  void enter_payment(long double *buffer, long double persent, short *days,
                     short year);
  void supplemention(long double *buffer, long double sup, long double persent,
                     short *days, short year);
  void elimination(long double *buffer, long double sup, long double persent,
                   short *days, short year);
  long double payment(long double persent, short days, short year) const;
  long double calculate_taxes(long double limit);
  tm early_closure(bool *early);

 private:
  long double amount_;
  long double deposit_body;
  short period_;
  bool capitalisation_;
  vector<event> events;
  vector<long double> payments;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_SRC_MODEL_DEPOSIT_H
