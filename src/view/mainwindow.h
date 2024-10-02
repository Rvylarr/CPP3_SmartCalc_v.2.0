#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <creditwindow.h>
#include <depositwindow.h>

#include <QIcon>
#include <QMainWindow>
#include <QPixmap>
#include <QVector>
#include <string>

#include "../controller/Controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_calcButton_clicked();
  void on_graphicButton_clicked();
  void on_creditButton_clicked();
  void on_depositButton_clicked();

  void on_pushButton_dot_clicked();
  void on_pushButton_C_clicked();
  void on_pushButton_del_clicked();
  void on_pushButton_equals_clicked();
  void on_pushButton_brackets_clicked();
  void on_pushButton_x_clicked();
  void on_pushButton_input_x_clicked();

  void on_pushButton_0_clicked();
  void on_pushButton_1_clicked();
  void on_pushButton_2_clicked();
  void on_pushButton_3_clicked();
  void on_pushButton_4_clicked();
  void on_pushButton_5_clicked();
  void on_pushButton_6_clicked();
  void on_pushButton_7_clicked();
  void on_pushButton_8_clicked();
  void on_pushButton_9_clicked();

  void on_pushButton_pow_clicked();
  void on_pushButton_sqrt_clicked();
  void on_pushButton_mod_clicked();
  void on_pushButton_tg_clicked();
  void on_pushButton_atg_clicked();
  void on_pushButton_ln_clicked();
  void on_pushButton_log_clicked();
  void on_pushButton_cos_clicked();
  void on_pushButton_acos_clicked();
  void on_pushButton_sin_clicked();
  void on_pushButton_asin_clicked();
  void on_pushButton_mul_clicked();
  void on_pushButton_div_clicked();
  void on_pushButton_plus_clicked();
  void on_pushButton_min_clicked();

 private:
  // --- functions --- //
  void UI_preparing();
  void graphic_draw();
  void enter_command(string command);
  void graphicButton_disabled();
  void calcButton_disabled();

  // --- values --- //
  Ui::MainWindow *ui;
  CreditWindow *credit_window;
  DepositWindow *deposit_window;
  s21::Controller controller;

  bool switched_;
  bool basic_mode_;
  std::string input_, x_input_;
};
#endif  // MAINWINDOW_H
