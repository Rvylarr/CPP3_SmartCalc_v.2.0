#include "Calculator.h"

namespace s21 {
// === calculator class ===//

string Calculator::calculate_expression() {
  parsing();
  if (num_stack.size() > 1) {
    while (!op_stack.empty()) calculation();
  }
  string result = to_string(num_stack.top());
  round(result);
  return result;
}

void Calculator::round(string &expr) {
  expr.erase(expr.find_last_not_of('0') + 1, string::npos);
  if (expr.back() == '.') expr.erase(expr.length() - 1);
}

void Calculator::parsing() {
  sregex_iterator it(expression_.begin(), expression_.end(),
                     filter_.get_filter("all"));

  for (sregex_iterator end; it != end; ++it) {
    string match = it->str();

    if (regex_search(match, filter_.get_filter("open_bracket")) ||
        regex_search(match, filter_.get_filter("trig"))) {
      if (regex_search(std::next(it)->str(), filter_.get_filter("simple_op"))) {
        ++it;
        ++it;
        match = it->str();
        match.insert(match.begin(), '-');
        num_stack.push(stod(match));
        ++it;
      } else
        operation_in_brackets(match);
    } else if (regex_search(match, filter_.get_filter("close_bracket")))
      break_brackets();
    else if (regex_search(match, filter_.get_filter("simple_op")))
      operation_without_brackets(match);
    else if (regex_search(match, filter_.get_filter("num")))
      num_stack.push(stod(match));
  }
}

void Calculator::operation_in_brackets(const string &match) {
  if (match != "(") {
    Operations op = NONE;

    if (match == "sin")
      op = SIN;
    else if (match == "cos")
      op = COS;
    else if (match == "tg")
      op = TG;
    else if (match == "asin")
      op = ASIN;
    else if (match == "acos")
      op = ACOS;
    else if (match == "atg")
      op = ATG;
    else if (match == "ln")
      op = LN;
    else if (match == "log")
      op = LOG;
    else if (match == "sqrt")
      op = SQRT;

    unique_ptr<Operation> new_operation =
        make_unique<Operation>(op, TRIGONOMETRY);
    op_stack.push(std::move(*new_operation));
  } else {
    unique_ptr<Operation> bracket =
        make_unique<Operation>(BRACKET_OPEN, BRACKETS);
    op_stack.push(std::move(*bracket));
  }
}

void Calculator::operation_without_brackets(const string &match) {
  Operations op = NONE;
  Priority priority;

  if (match == "+")
    op = PLUS;
  else if (match == "-")
    op = MIN;
  else if (match == "*")
    op = MUL;
  else if (match == "/")
    op = DIV;
  else if (match == "^")
    op = POW;
  else if (match == "%")
    op = MOD;

  if (op == PLUS || op == MIN)
    priority = PLUS_MIN;
  else if (op == MUL || op == DIV || op == MOD)
    priority = MUL_DIV;
  else
    priority = DIGREE;

  unique_ptr<Operation> new_operation = make_unique<Operation>(op, priority);
  if (!op_stack.empty()) {
    Priority top_priority = op_stack.top().get_priority();
    while (!op_stack.empty() && top_priority <= new_operation->get_priority() &&
           top_priority != BRACKETS) {
      calculation();
      if (!op_stack.empty()) top_priority = op_stack.top().get_priority();
    }
  }
  op_stack.push(std::move(*new_operation));
}

void Calculator::break_brackets() {
  if (!op_stack.empty()) {
    while (op_stack.top().get_operation() != BRACKET_OPEN) calculation();
    op_stack.pop();
    if (!op_stack.empty() && op_stack.top().get_priority() == TRIGONOMETRY)
      calculation();
  }
}

void Calculator::calculation() {
  double result = 0;
  double num_1 = num_stack.top();
  num_stack.pop();
  Operation op = op_stack.top();
  op_stack.pop();

  if (op.get_priority() != TRIGONOMETRY) {
    double num_2 = num_stack.top();
    num_stack.pop();

    if (op.get_operation() == PLUS)
      result = num_1 + num_2;
    else if (op.get_operation() == MIN)
      result = num_2 - num_1;
    else if (op.get_operation() == MUL)
      result = num_1 * num_2;
    else if (op.get_operation() == DIV)
      if (num_1 == 0)
        throw out_of_range("divide on zero");
      else
        result = num_2 / num_1;
    else if (op.get_operation() == MOD)
      result = fmod(num_2, num_1);
    else if (op.get_operation() == POW)
      result = pow(num_2, num_1);
  } else {
    if (op.get_operation() == SIN)
      result = sin(num_1);
    else if (op.get_operation() == COS)
      result = cos(num_1);
    else if (op.get_operation() == TG)
      result = tan(num_1);
    else if (op.get_operation() == ASIN)
      result = asin(num_1);
    else if (op.get_operation() == ACOS)
      result = acos(num_1);
    else if (op.get_operation() == ATG)
      result = atan(num_1);
    else if (op.get_operation() == LOG)
      result = log10(num_1);
    else if (op.get_operation() == LN)
      result = log(num_1);
    else if (op.get_operation() == SQRT)
      result = sqrt(num_1);
  }
  num_stack.push(result);
}

Operations Calculator::Operation::get_operation() { return operation_; }

Priority Calculator::Operation::get_priority() { return priority_; }
}  // namespace s21