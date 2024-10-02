#ifndef CREDITWINDOW_H
#define CREDITWINDOW_H

#include <depositwindow.h>

#include <QWidget>
#include <vector>

#include "../controller/Controller.h"

namespace Ui {
class CreditWindow;
}

class CreditWindow : public QWidget {
  Q_OBJECT

 public:
  explicit CreditWindow(QWidget *parent = nullptr);
  ~CreditWindow();

 signals:
  void main_window();

 private slots:
  void on_calcButton_clicked();
  void on_graphicButton_clicked();
  void on_calculate_button_clicked();

 private:
  void generate_payments_table();
  QString chop_zeros(QString s);
  tm convert_to_date(string raw_date);
  short get_frequency(string f);

  Ui::CreditWindow *ui;
  s21::Controller controller;
  vector<long double> results;
};

#endif  // CREDITWINDOW_H
