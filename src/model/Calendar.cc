#include "Calendar.h"

namespace s21 {
bool Calendar::is_monthly_event(tm date, tm event) {
  return date.tm_mday == event.tm_mday;
}

bool Calendar::is_quartaly_event(tm date, tm event) {
  return date.tm_mday == event.tm_mday && date.tm_mon == event.tm_mon;
}

bool Calendar::is_yearly_event(tm date, tm event) {
  return date.tm_mday == event.tm_mday && date.tm_mon == event.tm_mon &&
         date.tm_year == event.tm_year;
}

void Calendar::iterate_day(tm *date) {
  static const short days_in_month[12] = {31, 28, 31, 30, 31, 30,
                                          31, 31, 30, 31, 30, 31};

  if (date->tm_year % 4 == 0 && date->tm_mon == 1 && date->tm_mday > 27) {
    if (date->tm_mday == 28)
      date->tm_mday++;
    else if (date->tm_mday == 29) {
      date->tm_mday = 1;
      date->tm_mon++;
    }
  } else if (date->tm_mday == days_in_month[date->tm_mon]) {
    if (date->tm_mon == 11) {
      date->tm_year++;
      date->tm_mon = 0;
      date->tm_mday = 1;
    } else {
      date->tm_mon++;
      date->tm_mday = 1;
    }
  } else
    date->tm_mday++;
}

tm Calendar::next_date(tm *date, short friequency) {
  tm next = *date;

  for (int m = 0; m < friequency; m++) {
    do iterate_day(&next);
    while (date->tm_mday != next.tm_mday);
  }
  next.tm_year--;
  return next;
}

tm Calendar::end_date(tm *date, short period) {
  tm end_date = *date;
  while (period >= 12) {
    period -= 12;
    end_date.tm_year++;
  }
  while (period != 0) {
    if (end_date.tm_mon == 12) {
      end_date.tm_mon = 0;
      end_date.tm_year++;
      period--;
    } else {
      end_date.tm_mon++;
      period--;
    }
  }
  iterate_day(&end_date);

  return end_date;
}
}  // namespace s21
