#ifndef DEPOSITWINDOW_H
#define DEPOSITWINDOW_H

#include <QWidget>
#include <ctime>
#include <string>

#include "../controller/Controller.h"
#include "eventbox.h"

using namespace std;

namespace Ui {
class DepositWindow;
}

class DepositWindow : public QWidget {
  Q_OBJECT

 public:
  explicit DepositWindow(QWidget *parent = nullptr);
  ~DepositWindow();

 signals:
  void main_window();

 private slots:
  void on_calcButton_clicked();
  void on_graphicButton_clicked();
  void on_calculate_button_clicked();

  void on_add_supplementions_clicked();
  void on_add_eliminations_clicked();
  void on_early_closure_stateChanged(int arg1);

 private:
  void delete_supplementions();
  void delete_eliminations();
  void generate_payments_table(vector<long double> results);
  tm convert_to_date(string raw_date);
  string get_frequency(string f);
  QString chop_zeros(QString s);

  Ui::DepositWindow *ui;
  s21::Controller controller;
  QVector<EventBox *> spls;
  QVector<EventBox *> elms;
  short spl_count;
  short elm_count;
};

#endif  // DEPOSITWINDOW_H
