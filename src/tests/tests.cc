#include <gtest/gtest.h>

#include "../controller/Controller.h"

using namespace s21;
using namespace std;

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(Basic_calculator, bc_1) {
  Controller c;
  string e = "sin(2)";
  ASSERT_EQ(stod(c.calculate_basic(e)), 0.909297);
}

TEST(Basic_calculator, bc_2) {
  Controller c;
  string e = "8 + 2 ^ 3 * 5";
  ASSERT_EQ(stod(c.calculate_basic(e)), 48);
}

TEST(Basic_calculator, bc_3) {
  Controller c;
  string e = "(1 + 3) * (4 - 2 / 2)";
  ASSERT_EQ(stod(c.calculate_basic(e)), 12);
}

TEST(Basic_calculator, bc_4) {
  Controller c;
  string e =
      "(4 * (cos(45)^2 + sin(45)^3))^1 - ((2.1 + 2.2)^4 / (2 * sqrt(64)))";
  ASSERT_EQ(stod(c.calculate_basic(e)), -17.799312);
}

TEST(X_calculator, xc_1) {
  Controller c;
  string e = "sin(x)";
  string x = "1";
  ASSERT_EQ(stod(c.calculate_with_x(e, x)), 0.841471);
}

TEST(X_calculator, xc_2) {
  Controller c;
  string e = "sin(x)";
  string x = "1 + 1";
  ASSERT_EQ(stod(c.calculate_with_x(e, x)), 0.909297);
}

TEST(Validator, v_1) {
  Controller c;
  string e = "(1 + 3) * (4 - 2 / 2";  // not closed bracket
  ASSERT_ANY_THROW(c.calculate_basic(e));
}

TEST(Validator, v_2) {
  Controller c;
  string e = "(1 +";
  string input = "*";

  ASSERT_EQ(c.check_validity(e, input), "(1 *");
}

TEST(Validator, v_3) {
  Controller c;
  string e = "(1";
  string input = "*";

  ASSERT_EQ(c.check_validity(e, input), e + input);
}

TEST(Validator, v_4) {
  Controller c;
  string e = "(1+2";
  string input = "(";

  ASSERT_EQ(c.check_validity(e, input), e + ")");
}

TEST(Validator, v_5) {
  Controller c;
  string e = "(1+2";
  string input = "sin";

  ASSERT_EQ(c.check_validity(e, input), e + "*" + input + "(");
}

TEST(Validator, v_6) {
  Controller c;
  string e = "6+sin(";
  string input = "e";

  ASSERT_EQ(c.check_validity(e, input), "6+");
}

TEST(Validator, v_7) {
  Controller c;
  string e = "6+sin(";
  string input = "x";

  ASSERT_EQ(c.check_validity(e, input), e + "x");
}

TEST(Validator, v_8) {
  Controller c;
  string e = "";
  string input = "9";

  ASSERT_EQ(c.check_validity(e, input), "9");
}

TEST(Credit, c_1) {  // annuity
  tm date{};
  date.tm_mday = 1, date.tm_mon = 0, date.tm_year = 124;

  vector<long double> result;
  long double amount = 100000;
  double persent = 10;
  short period = 6, frequency = 1;
  bool annuity = true;

  s21::Controller::calculate_credit(amount, period, date, frequency, persent,
                                    annuity, &result);
  EXPECT_NEAR(result[result.size() - 2], 102920, 100);
}

TEST(Credit, c_2) {  // differential
  tm date{};
  date.tm_mday = 1, date.tm_mon = 0, date.tm_year = 124;

  vector<long double> result;
  long double amount = 100000;
  double persent = 10;
  short period = 6, frequency = 3;
  bool annuity = false;

  s21::Controller::calculate_credit(amount, period, date, frequency, persent,
                                    annuity, &result);
  EXPECT_NEAR(result[result.size() - 2], 103730, 10);
}

TEST(Credit, c_3) {  // differential
  tm date{};
  date.tm_mday = 1, date.tm_mon = 0, date.tm_year = 124;

  vector<long double> result;
  long double amount = 100000;
  double persent = 10;
  short period = 12, frequency = 12;
  bool annuity = false;

  s21::Controller::calculate_credit(amount, period, date, frequency, persent,
                                    annuity, &result);
  EXPECT_NEAR(result[result.size() - 2], 110000, 10);
}

TEST(Credit, c_4) {  // differential
  tm date{};
  date.tm_mday = 1, date.tm_mon = 0, date.tm_year = 124;

  vector<long double> result;
  long double amount = 100000;
  double persent = 10;
  short period = 0, frequency = 1;
  bool annuity = false;

  ASSERT_ANY_THROW(s21::Controller::calculate_credit(amount, period, date,
                                                     frequency, persent,
                                                     annuity, &result););
}

TEST(Credit, c_5) {  // differential
  tm date{};
  date.tm_mday = 1, date.tm_mon = 0, date.tm_year = 124;

  vector<long double> result;
  long double amount = 100000;
  double persent = 0;
  short period = 1, frequency = 1;
  bool annuity = false;

  ASSERT_ANY_THROW(s21::Controller::calculate_credit(amount, period, date,
                                                     frequency, persent,
                                                     annuity, &result););
}

TEST(Credit, c_6) {  // differential
  tm date{};
  date.tm_mday = 1, date.tm_mon = 0, date.tm_year = 124;

  vector<long double> result;
  long double amount = 0;
  double persent = 10;
  short period = 1, frequency = 1;
  bool annuity = false;

  ASSERT_ANY_THROW(s21::Controller::calculate_credit(amount, period, date,
                                                     frequency, persent,
                                                     annuity, &result););
}

TEST(Credit, c_7) {  // differential
  tm date{};
  date.tm_mday = 1, date.tm_mon = 0, date.tm_year = 124;

  vector<long double> result;
  long double amount = 100000;
  double persent = 10;
  short period = 4, frequency = 3;
  bool annuity = false;

  ASSERT_ANY_THROW(s21::Controller::calculate_credit(amount, period, date,
                                                     frequency, persent,
                                                     annuity, &result););
}

TEST(Deposit, d_1) {
  tm date_1{};
  date_1.tm_mday = 1, date_1.tm_mon = 0, date_1.tm_year = 124;

  tm date_2{};
  date_2.tm_mday = 1, date_2.tm_mon = 0, date_2.tm_year = 124;

  vector<long double> sums = {10, 0};
  vector<tm> dates = {date_1, date_2};
  vector<string> types = {"1",    // процент
                          "3m"};  // периодичность выплат
  double amount = 100000;
  short period = 12;
  bool capitalisation = false;

  s21::Controller::calculate_deposit(&sums, &dates, &types, amount, period,
                                     capitalisation);
  EXPECT_NEAR(sums[sums.size() - 1], 110000, 1);
}

TEST(Deposit, d_2) {
  tm date_1{};
  date_1.tm_mday = 15, date_1.tm_mon = 0, date_1.tm_year = 124;

  tm date_2{};
  date_2.tm_mday = 15, date_2.tm_mon = 0, date_2.tm_year = 124;

  vector<long double> sums = {15, 0};
  vector<tm> dates = {date_1, date_2};
  vector<string> types = {"1",    // процент
                          "3q"};  // периодичность выплат
  double amount = 100000;
  short period = 12;
  bool capitalisation = true;

  s21::Controller::calculate_deposit(&sums, &dates, &types, amount, period,
                                     capitalisation);
  EXPECT_NEAR(sums[sums.size() - 1], 115867, 1);
}

TEST(Deposit, d_3) {
  tm date_1{};
  date_1.tm_mday = 15, date_1.tm_mon = 0, date_1.tm_year = 124;

  tm date_2{};
  date_2.tm_mday = 15, date_2.tm_mon = 0, date_2.tm_year = 124;

  tm date_3{};
  date_3.tm_mday = 21, date_3.tm_mon = 2, date_3.tm_year = 124;

  tm date_4{};
  date_4.tm_mday = 23, date_4.tm_mon = 2, date_4.tm_year = 124;

  vector<long double> sums = {15, 0, 12000, 500};
  vector<tm> dates = {date_1, date_2, date_3, date_4};
  vector<string> types = {"1", "3q", "5o", "6o"};
  double amount = 100000;
  short period = 12;
  bool capitalisation = true;

  s21::Controller::calculate_deposit(&sums, &dates, &types, amount, period,
                                     capitalisation);
  EXPECT_NEAR(sums[sums.size() - 1], 128844.60, 1);
}

TEST(Deposit, d_4) {
  vector<long double> sums = {};
  vector<tm> dates = {};
  vector<string> types{};
  double amount = 0;
  short period = 12;
  bool capitalisation = false;

  ASSERT_ANY_THROW(s21::Controller::calculate_deposit(
      &sums, &dates, &types, amount, period, capitalisation));
}

TEST(Deposit, d_5) {
  tm date_1{};
  date_1.tm_mday = 1, date_1.tm_mon = 0, date_1.tm_year = 124;

  tm date_2{};
  date_2.tm_mday = 1, date_2.tm_mon = 0, date_2.tm_year = 124;

  vector<long double> sums = {10, 0};
  vector<tm> dates = {date_1, date_2};
  vector<string> types = {"1", "3m"};
  double amount = 0;
  short period = 12;
  bool capitalisation = false;

  ASSERT_ANY_THROW(s21::Controller::calculate_deposit(
      &sums, &dates, &types, amount, period, capitalisation));
}

TEST(Deposit, d_6) {
  tm date_1{};
  date_1.tm_mday = 1, date_1.tm_mon = 0, date_1.tm_year = 124;

  tm date_2{};
  date_2.tm_mday = 1, date_2.tm_mon = 0, date_2.tm_year = 124;

  vector<long double> sums = {10, 0};
  vector<tm> dates = {date_1, date_2};
  vector<string> types = {"1", "3m"};
  double amount = 1;
  short period = 0;
  bool capitalisation = false;

  ASSERT_ANY_THROW(s21::Controller::calculate_deposit(
      &sums, &dates, &types, amount, period, capitalisation));
}

TEST(Deposit, d_7) {
  tm date_1{};
  date_1.tm_mday = 1, date_1.tm_mon = 0, date_1.tm_year = 124;

  tm date_2{};
  date_2.tm_mday = 1, date_2.tm_mon = 0, date_2.tm_year = 124;

  vector<long double> sums = {10, 0, 17};
  vector<tm> dates = {date_1, date_2, date_2};
  vector<string> types = {"1", "3d", "2"};
  double amount = 2000000;
  short period = 12;
  bool capitalisation = false;

  s21::Controller::calculate_deposit(&sums, &dates, &types, amount, period,
                                     capitalisation);

  // при ключевой ставке в 17% налог со вклада:
  EXPECT_NEAR(sums[sums.size() - 2], 3829, 1);
}