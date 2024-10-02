#include "Validator.h"

namespace s21 {
// === Validator class === //
string Validator::check_expression() {
  sregex_iterator it(expression_.begin(), expression_.end(),
                     filter_.get_filter("all"));
  sregex_iterator end;
  string last_elem;
  for (; it != end; ++it) last_elem = it->str();

  if (input_option_.front() == 'e')
    check_erase(last_elem);  //
  else if (regex_search(input_option_, filter_.get_filter("open_bracket")))
    check_brackets(last_elem);
  else if (regex_search(input_option_, filter_.get_filter("num"))) {
    if (expression_.empty() ||
        regex_search(last_elem, filter_.get_filter("num")) ||
        regex_search(last_elem, filter_.get_filter("simple_op")) ||
        regex_search(last_elem, filter_.get_filter("trig")) ||
        regex_search(last_elem, filter_.get_filter("open_bracket")) ||
        expression_[expression_.length() - 1] == '.')
      expression_ += input_option_;
    else if (regex_search(last_elem, filter_.get_filter("x")))
      expression_ += "*" + input_option_;
  } else if (regex_search(input_option_, filter_.get_filter("x"))) {
    if (regex_search(last_elem, filter_.get_filter("num")) ||
        regex_search(last_elem, filter_.get_filter("close_bracket")))
      expression_ += "*" + input_option_;
    else if (expression_.empty() ||
             regex_search(last_elem, filter_.get_filter("open_bracket")) ||
             regex_search(last_elem, filter_.get_filter("simple_op")))
      expression_ += input_option_;
  } else if (regex_search(input_option_, filter_.get_filter("dot"))) {
    if (regex_search(last_elem, filter_.get_filter("num")) &&
        stod(last_elem) == (double)stoi(last_elem) &&
        expression_[expression_.length() - 1] != '.')
      expression_ += input_option_;
  } else if (regex_search(input_option_, filter_.get_filter("simple_op"))) {
    if (regex_search(last_elem, filter_.get_filter("simple_op")))
      expression_.erase(expression_.length() - 1);
    if (regex_search(last_elem, filter_.get_filter("num")) ||
        regex_search(last_elem, filter_.get_filter("x")) ||
        regex_search(last_elem, filter_.get_filter("simple_op")) ||
        regex_search(last_elem, filter_.get_filter("close_bracket")))
      expression_ += input_option_;
  } else if (regex_search(input_option_, filter_.get_filter("trig"))) {
    if (regex_search(last_elem, filter_.get_filter("num")) ||
        regex_search(last_elem, filter_.get_filter("x")) ||
        regex_search(last_elem, filter_.get_filter("close_bracket")))
      expression_ += "*" + input_option_;
    else if (expression_.empty() ||
             regex_search(last_elem, filter_.get_filter("open_bracket")) ||
             regex_search(last_elem, filter_.get_filter("simple_op")))
      expression_ += input_option_;
    expression_ += "(";
  }
  return expression_;
}

void Validator::check_brackets(string &last_elem) {
  sregex_iterator it(expression_.begin(), expression_.end(),
                     filter_.get_filter("all"));
  sregex_iterator end;
  short opened = 0, closed = 0;
  string bracket;
  while (it != end) {
    bracket = it->str();
    if (regex_search(bracket, filter_.get_filter("open_bracket"))) opened++;
    if (regex_search(bracket, filter_.get_filter("close_bracket"))) closed++;
    ++it;
  }
  if (regex_search(last_elem, filter_.get_filter("num")) ||
      regex_search(last_elem, filter_.get_filter("close_bracket")))
    opened > closed ? expression_ += ")" : expression_ += "*(";
  else if (opened > closed &&
           !regex_search(bracket, filter_.get_filter("open_bracket")) &&
           !regex_search(last_elem, filter_.get_filter("simple_op")))
    expression_ += ")";
  else
    expression_ += "(";
}

void Validator::check_erase(string &last_elem) {
  if (expression_.back() == '.' ||
      regex_match(last_elem, filter_.get_filter("num")) ||
      regex_match(last_elem, filter_.get_filter("x")) ||
      regex_match(last_elem, filter_.get_filter("simple_op")) ||
      regex_match(last_elem, filter_.get_filter("open_bracket")) ||
      regex_match(last_elem, filter_.get_filter("close_bracket"))) {
    expression_.erase(expression_.end() - 1);

    sregex_iterator it(expression_.begin(), expression_.end(),
                       filter_.get_filter("all"));
    sregex_iterator end;
    for (; it != end; ++it) last_elem = it->str();
    if (regex_search(last_elem, filter_.get_filter("trig"))) {
      for (auto l = last_elem.length(); l > 0; l--)
        expression_.erase(expression_.end() - 1);
    }
  }
}

string Validator::input_x(string &x) {
  sregex_iterator it(expression_.begin(), expression_.end(),
                     filter_.get_filter("all"));
  sregex_iterator end;
  string expression;

  for (; it != end; ++it) {
    if (regex_search(it->str(), filter_.get_filter("x")))
      expression += x;
    else
      expression += it->str();
  }
  return expression;
}

void Validator::final_check() {
  if (expression_.empty()) throw std::logic_error("expression is empty");
  sregex_iterator it(expression_.begin(), expression_.end(),
                     filter_.get_filter("all"));
  sregex_iterator end;
  short opened = 0, closed = 0;

  while (it != end) {
    string bracket = it->str();
    if (regex_search(bracket, filter_.get_filter("open_bracket"))) opened++;
    if (regex_search(bracket, filter_.get_filter("close_bracket"))) closed++;
    ++it;
  }
  if (opened > closed)
    throw std::logic_error("expression is not valid(close brackets)");
  if (expression_[expression_.length() - 1] == '.' ||
      regex_search(&expression_[expression_.length() - 1],
                   filter_.get_filter("simple_op")))
    throw std::logic_error("expression is not valid(last symbol)");
}
}  // namespace s21