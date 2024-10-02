#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  QWidget::setFixedSize(562, 888);
  ui->setupUi(this);
  switched_ = false;
  basic_mode_ = true;
  input_ = "", x_input_ = "";

  credit_window = new CreditWindow();
  deposit_window = new DepositWindow();

  connect(credit_window, &CreditWindow::main_window, this, &MainWindow::show);
  connect(deposit_window, &DepositWindow::main_window, this, &MainWindow::show);
  UI_preparing();
}

MainWindow::~MainWindow() { delete ui; }

// --- slots --- //

void MainWindow::on_calcButton_clicked() {
  QWidget::setFixedSize(562, 888);
  QPixmap calc_on(":/icons/icons/calc_on.png");
  QIcon calc2(calc_on);

  basic_mode_ = true;
  input_.clear();
  graphicButton_disabled();
  ui->calcButton->setIcon(calc2);
  ui->Input_x->setText("0");
  ui->Input->setText("0");
  ui->pushButton_input_x->setVisible(false);
  ui->pushButton_x->setVisible(false);
  ui->pushButton_mod->resize(191, 81);
}

void MainWindow::on_graphicButton_clicked() {
  QWidget::setFixedSize(1052, 888);
  QPixmap graphic_on(":/icons/icons/graphic_on.png");
  QIcon graphic2(graphic_on);

  basic_mode_ = false;
  input_.clear();
  calcButton_disabled();
  ui->graphicButton->setIcon(graphic2);
  ui->Input_x->setText("0");
  ui->Input->setText("0");
  ui->Input->setFixedWidth(981);
  ui->pushButton_input_x->setVisible(true);
  ui->pushButton_x->setVisible(true);
  ui->pushButton_mod->resize(81, 81);
}

void MainWindow::on_creditButton_clicked() {
  credit_window->show();
  this->close();
}

void MainWindow::on_depositButton_clicked() {
  deposit_window->show();
  this->close();
}

// --- calc buttons --- //

void MainWindow::on_pushButton_dot_clicked() { enter_command("."); }

void MainWindow::on_pushButton_C_clicked() {
  input_.clear();
  switched_ ? ui->Input_x->setText("0") : ui->Input->setText("0");
}

void MainWindow::on_pushButton_del_clicked() { enter_command("e"); }

void MainWindow::on_pushButton_equals_clicked() {
  ui->error_output->clear();
  try {
    if (basic_mode_)
      ui->Input->setText(
          QString::fromStdString(input_ = controller.calculate_basic(input_)));
    else {
      ui->Input->setText(QString::fromStdString(
          controller.calculate_with_x(input_, x_input_)));
      graphic_draw();
    }
  } catch (exception &e) {
    ui->error_output->setText(e.what());
  }
}

void MainWindow::on_pushButton_brackets_clicked() { enter_command("("); }

void MainWindow::on_pushButton_x_clicked() { enter_command("x"); }

void MainWindow::on_pushButton_input_x_clicked() {
  string tmp = input_;
  input_ = x_input_;
  x_input_ = tmp;
  if (!switched_) {
    ui->Input->setStyleSheet("color: #828282;");
    ui->Input_x->setStyleSheet("color: white;");
    ui->pushButton_x->setEnabled(false);
  } else {
    ui->Input->setStyleSheet("color: white;");
    ui->Input_x->setStyleSheet("color: #828282;");
    ui->pushButton_x->setEnabled(true);
  }
  switched_ = switched_ ? false : true;
}

void MainWindow::on_pushButton_0_clicked() { enter_command("0"); }

void MainWindow::on_pushButton_1_clicked() { enter_command("1"); }

void MainWindow::on_pushButton_2_clicked() { enter_command("2"); }

void MainWindow::on_pushButton_3_clicked() { enter_command("3"); }

void MainWindow::on_pushButton_4_clicked() { enter_command("4"); }

void MainWindow::on_pushButton_5_clicked() { enter_command("5"); }

void MainWindow::on_pushButton_6_clicked() { enter_command("6"); }

void MainWindow::on_pushButton_7_clicked() { enter_command("7"); }

void MainWindow::on_pushButton_8_clicked() { enter_command("8"); }

void MainWindow::on_pushButton_9_clicked() { enter_command("9"); }

void MainWindow::on_pushButton_pow_clicked() { enter_command("^"); }

void MainWindow::on_pushButton_sqrt_clicked() { enter_command("sqrt"); }

void MainWindow::on_pushButton_mod_clicked() { enter_command("%"); }

void MainWindow::on_pushButton_tg_clicked() { enter_command("tg"); }

void MainWindow::on_pushButton_atg_clicked() { enter_command("atg"); }

void MainWindow::on_pushButton_ln_clicked() { enter_command("ln"); }

void MainWindow::on_pushButton_log_clicked() { enter_command("log"); }

void MainWindow::on_pushButton_cos_clicked() { enter_command("cos"); }

void MainWindow::on_pushButton_acos_clicked() { enter_command("acos"); }

void MainWindow::on_pushButton_sin_clicked() { enter_command("sin"); }

void MainWindow::on_pushButton_asin_clicked() { enter_command("asin"); }

void MainWindow::on_pushButton_mul_clicked() { enter_command("*"); }

void MainWindow::on_pushButton_div_clicked() { enter_command("/"); }

void MainWindow::on_pushButton_plus_clicked() { enter_command("+"); }

void MainWindow::on_pushButton_min_clicked() { enter_command("-"); }

void MainWindow::UI_preparing() {
  QPixmap calc_on(":/icons/icons/calc_on.png");
  QPixmap graphic_off(":/icons/icons/graphic_off.png");
  QPixmap credit_off(":/icons/icons/credit_off.png");
  QPixmap deposit_off(":/icons/icons/deposit_off.png");
  QPixmap clear(":/icons/icons/clear.png");
  QColor dark_blue(0, 93, 178);
  QColor light_blue(36, 165, 255);

  QIcon calc(calc_on);
  QIcon graphic(graphic_off);
  QIcon credit(credit_off);
  QIcon deposit(deposit_off);
  QIcon clr(clear);

  ui->calcButton->setIcon(calc);
  ui->graphicButton->setIcon(graphic);
  ui->creditButton->setIcon(credit);
  ui->depositButton->setIcon(deposit);
  ui->pushButton_del->setIcon(clr);
  ui->pushButton_x->setVisible(false);
  ui->pushButton_input_x->setVisible(false);

  ui->widget->setBackground(QColor(23, 24, 26));
  ui->widget->xAxis->setBasePen(QPen(dark_blue, 1));
  ui->widget->yAxis->setBasePen(QPen(dark_blue, 1));
  ui->widget->xAxis->setTickLabelColor(light_blue);  // координаты
  ui->widget->yAxis->setTickLabelColor(light_blue);
  ui->widget->xAxis->setSubTickPen(QPen(dark_blue, 1));  // засечки
  ui->widget->yAxis->setSubTickPen(QPen(dark_blue, 1));
  ui->widget->xAxis->grid()->setPen(QPen(dark_blue, 1, Qt::DotLine));
  ui->widget->yAxis->grid()->setPen(QPen(dark_blue, 1, Qt::DotLine));

  ui->xMin->setMaximum(1000000);
  ui->xMax->setMaximum(1000000);
  ui->yMin->setMaximum(1000000);
  ui->yMax->setMaximum(1000000);

  ui->xMin->setMinimum(-1000000);
  ui->xMax->setMinimum(-1000000);
  ui->yMin->setMinimum(-1000000);
  ui->yMax->setMinimum(-1000000);
}

void MainWindow::graphic_draw() {
  double x_min = ui->xMin->text().toInt();
  double x_max = ui->xMax->text().toInt();
  double y_min = ui->yMin->text().toInt();
  double y_max = ui->yMax->text().toInt();

  if (x_min < x_max && y_min < y_max) {
    ui->widget->xAxis->setRange(x_min, x_max);
    ui->widget->yAxis->setRange(y_min, y_max);
    double dots = (x_max - x_min) / 0.05 + 2;
    QVector<double> x(dots), y(dots);
    int i = 0;
    for (double X = x_min; X <= x_max; X += 0.05) {
      string sx = std::to_string(X);
      double result = std::stod(controller.calculate_with_x(input_, sx));
      x[i] = X;
      y[i] = result;
      i++;
    }
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x, y);
    ui->widget->graph(0)->setPen(QColor(143, 240, 164));
    ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(0)->setScatterStyle(
        QCPScatterStyle(QCPScatterStyle::ssCircle, 1));
    ui->widget->replot();
    x.clear();
    y.clear();
  }
}

void MainWindow::enter_command(string command) {
  string tmp = input_;
  ui->error_output->clear();
  input_ = controller.check_validity(input_, command);
  if (input_.empty())
    switched_ ? ui->Input_x->setText("0") : ui->Input->setText("0");
  else if (tmp != input_) {
    switched_ ? ui->Input_x->setText(QString::fromStdString(input_))
              : ui->Input->setText(QString::fromStdString(input_));
  }
}

void MainWindow::calcButton_disabled() {
  QPixmap calc_off(":/icons/icons/calc_off.png");
  QIcon calc1(calc_off);
  ui->calcButton->setIcon(calc1);
}

void MainWindow::graphicButton_disabled() {
  QPixmap graphic_off(":/icons/icons/graphic_off.png");
  QIcon graphic1(graphic_off);
  ui->Input->setFixedWidth(511);
  ui->graphicButton->setIcon(graphic1);
}
