#include "Deposit.h"

namespace s21 {

// === deposit class === //
vector<long double> Deposit::calculate_deposit() {
  vector<long double> result;
  sort_events();
  check_events();
  result = run_calendar();
  return result;
}

void Deposit::create_events_list(vector<long double> sums, vector<tm> dates,
                                 vector<string> types) {
  short i = 0;
  for (auto it = sums.begin(); it != sums.end(); ++it, i++) {
    event new_event;
    new_event.sum = sums[i];
    new_event.date = dates[i];
    new_event.type = types[i];
    events.push_back(new_event);
  }
}

void Deposit::sort_events() {
  sort(events.begin(), events.end(),
       [](const event &e1, const event &e2) -> bool {
         return e1.type[0] < e2.type[0];
       });
}

void Deposit::check_events() {
  for (auto &e : events) {
    if (stol(to_string(e.date.tm_year) + to_string(e.date.tm_mon) +
             to_string(e.date.tm_mday)) <
        stol(to_string(events[0].date.tm_year) +
             to_string(events[0].date.tm_mon) +
             to_string(events[0].date.tm_mday)))
      throw out_of_range("some of events have forbidden date");
  }
}

vector<long double> Deposit::run_calendar() {
  deposit_body = amount_;
  long double persents = events[0].sum;
  long double buffer = 0;
  long double tax = 0;
  short payment_days = 0;
  bool early = false;
  tm current_date = events[0].date;
  tm end_date = early_closure(&early);

  events.erase(events.begin());
  for (auto &event : events) {
    if (event.type == "2") tax = event.sum;
    if (event.type.size() == 2 && event.type[1] == 'q')
      event.date = s21::Calendar::next_date(&event.date, 3);
  }
  while (!s21::Calendar::is_yearly_event(current_date, end_date)) {
    short year = current_date.tm_year % 4 == 0 ? 366 : 365;
    for (auto &e : events) {
      if ((e.type.size() == 2 && e.type[1] == 'd' && payment_days != 0) ||
          e.type == "5d" || e.type == "6d") {
        if (e.type == "3d")
          enter_payment(&buffer, persents, &payment_days, year);
        else if (e.type == "5d")
          supplemention(&buffer, e.sum, persents, &payment_days, year);
        else if (e.type == "6d")
          elimination(&buffer, e.sum, persents, &payment_days, year);
      } else if (s21::Calendar::is_yearly_event(current_date, e.date)) {
        if (e.type == "3o" || (e.type == "3y" && payment_days != 0))
          enter_payment(&buffer, persents, &payment_days, year);
        else if (e.type == "5o" || e.type == "5y")
          supplemention(&buffer, e.sum, persents, &payment_days, year);
        else if (e.type == "6o" || e.type == "6y")
          elimination(&buffer, e.sum, persents, &payment_days, year);
        if (e.type.size() == 2 && e.type[1] == 'y') e.date.tm_year++;
      } else if (s21::Calendar::is_quartaly_event(current_date, e.date) &&
                 payment_days != 0) {
        if (e.type == "3q")
          enter_payment(&buffer, persents, &payment_days, year);
        else if (e.type == "5q")
          supplemention(&buffer, e.sum, persents, &payment_days, year);
        else if (e.type == "6q")
          elimination(&buffer, e.sum, persents, &payment_days, year);
        if (e.type.size() == 2 && e.type[1] == 'q')
          e.date = s21::Calendar::next_date(&e.date, 3);
      }
      if (s21::Calendar::is_monthly_event(current_date, e.date) &&
          payment_days != 0) {
        if (e.type == "3m")
          enter_payment(&buffer, persents, &payment_days, year);
        else if (e.type == "5m")
          supplemention(&buffer, e.sum, persents, &payment_days, year);
        else if (e.type == "6m")
          elimination(&buffer, e.sum, persents, &payment_days, year);
      }
    }
    tm post_date = current_date;
    s21::Calendar::iterate_day(&current_date);
    if (post_date.tm_year < current_date.tm_year)
      supplemention(&buffer, 0, persents, &payment_days, year);
    payment_days++;
  }
  if (payment_days > 1) {
    if (!early) payment_days--;
    enter_payment(&buffer, persents, &payment_days,
                  current_date.tm_year % 4 == 0 ? 366 : 365);
  }
  if (tax != 0) {
    long double tax_amount = calculate_taxes(tax / 100 * 1000000);
    deposit_body -= tax_amount;
    payments.push_back(tax_amount);
  } else
    payments.push_back(0);
  payments.push_back(capitalisation_ ? deposit_body : amount_);

  return payments;
}

void Deposit::enter_payment(long double *buffer, long double persent,
                            short *days, short year) {
  long double pay = payment(persent, *days, year);
  pay += *buffer;
  *buffer = 0;
  *days = 0;
  capitalisation_ ? deposit_body += pay : amount_ += pay;
  payments.push_back(pay);
}

void Deposit::supplemention(long double *buffer, long double sup,
                            long double persent, short *days, short year) {
  *buffer += payment(persent, *days, year);
  *days = 0;
  deposit_body = round((deposit_body + sup) * 100) / 100;
}

void Deposit::elimination(long double *buffer, long double sup,
                          long double persent, short *days, short year) {
  *buffer += payment(persent, *days, year);
  *days = 0;
  deposit_body = round((deposit_body - sup) * 100) / 100;
  if (deposit_body < 0) throw logic_error("elimination sum is to high");
}

long double Deposit::payment(long double persent, short days,
                             short year) const {
  return round(deposit_body * persent * days / 100 / year * 100) / 100;
}

long double Deposit::calculate_taxes(long double limit) {
  long double profit = 0, sub = 0;
  short count = 0, repeat = 0, modifier = 0, i = 0;

  for (auto &event : events) {
    if (event.type == "3d")
      modifier = 365;
    else if (event.type == "3m")
      modifier = 12;
    else if (event.type == "3q")
      modifier = 4;
    else if (event.type == "3y")
      modifier = 1;
  }
  for (int j = 0; j != repeat; j++)
    (short)payments.size() == modifier ? count = (short)payments.size()
                                       : repeat++;
  do {
    short what = modifier == 0 ? count : modifier;
    for (int j = 0; j < what; j++) profit += payments[i];
    if (profit > limit) sub += (profit - limit) * 0.13;
    repeat--;
  } while (repeat > 0);

  return sub;
}

tm Deposit::early_closure(bool *early) {
  tm end_date{};
  if (events[events.size() - 1].type == "7") {
    *early = true;
    end_date = events[events.size() - 1].date;
    events.pop_back();
  } else
    end_date = s21::Calendar::end_date(&events[0].date, period_);

  return end_date;
}
}  // namespace s21
