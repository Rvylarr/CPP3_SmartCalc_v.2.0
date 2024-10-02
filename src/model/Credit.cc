#include "Credit.h"

namespace s21 {
// === credit class === //

void Credit::calculate_different(vector<long double> *values) {
  long double balance = amount_;
  long double base_payment = amount_ / (period_ / frequency_);
  long double total_overpayment = 0;
  short payment_days = 0;
  tm end_date = s21::Calendar::end_date(&current_date_, period_);
  tm next_payment = s21::Calendar::next_date(&current_date_, frequency_);

  while (!s21::Calendar::is_yearly_event(current_date_, end_date)) {
    if (s21::Calendar::is_quartaly_event(current_date_, next_payment) &&
        payment_days != 0) {
      short d = 0;
      if (payment_days == 366)
        d = payment_days;
      else
        d = current_date_.tm_year % 4 == 0 ? 366 : 365;
      long double overpayment = (balance * (persent_ / 100) * payment_days) / d;
      values->push_back(round((overpayment + base_payment) * 100) / 100);
      balance -= base_payment;
      total_overpayment += overpayment;
      next_payment = s21::Calendar::next_date(&current_date_, frequency_);
      payment_days = 0;
    }
    payment_days++;
    s21::Calendar::iterate_day(&current_date_);
  }
  long double total = amount_ + total_overpayment;
  values->push_back(round(total * 100) / 100);
  values->push_back(round(total_overpayment * 100) / 100);
}

void Credit::calculate_annuity(vector<long double> *values) {
  long double m_persent = persent_ / 12 / 100;
  long double annuity_coef = (m_persent * pow(1 + m_persent, period_)) /
                             (pow(1 + m_persent, period_) - 1);
  long double payment = amount_ * annuity_coef;

  for (short i = 0; i < period_ / frequency_; i++)
    values->push_back(round(payment * 100) / 100);

  long double total = payment * period_;
  values->push_back(round(total * 100) / 100);
  long double overpayment = total - amount_;
  values->push_back(round(overpayment * 100) / 100);
}
}  // namespace s21
