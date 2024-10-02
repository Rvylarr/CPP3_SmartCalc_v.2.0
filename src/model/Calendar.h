#ifndef CPP3_SMARTCALC_V2_0_1_SRC_MODEL_CALENDAR_H
#define CPP3_SMARTCALC_V2_0_1_SRC_MODEL_CALENDAR_H

#include <ctime>
#include <iostream>

using namespace std;

namespace s21 {
class Calendar {
 public:
  Calendar() = default;
  ~Calendar() = default;

  static bool is_monthly_event(tm date, tm event);
  static bool is_quartaly_event(tm date, tm event);
  static bool is_yearly_event(tm date, tm event);
  static void iterate_day(tm *date);
  static tm next_date(tm *date, short friequency);
  static tm end_date(tm *date, short period);
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_SRC_MODEL_CALENDAR_H
