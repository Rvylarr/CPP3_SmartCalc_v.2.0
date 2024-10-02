#ifndef CPP3_SMARTCALC_V2_0_1_SRC_MODEL_CREDIT_H
#define CPP3_SMARTCALC_V2_0_1_SRC_MODEL_CREDIT_H

#include <cmath>
#include <ctime>
#include <vector>

#include "Calendar.h"

using namespace std;

namespace s21 {
class Credit {
 public:
  // --- constructors --- //
  Credit() = delete;
  Credit(long double amount, short period, tm date, double persent,
         short freauency)
      : amount_(amount),
        period_(period),
        current_date_(date),
        persent_(persent),
        frequency_(freauency) {}
  ~Credit() = default;

  // --- calculation --- //
  void calculate_different(vector<long double> *values);
  void calculate_annuity(vector<long double> *values);

 private:
  long double amount_;  // сумма займа
  short period_;        // срок
  tm current_date_;
  double persent_;   // процент
  short frequency_;  // периодичность
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_SRC_MODEL_CREDIT_H
