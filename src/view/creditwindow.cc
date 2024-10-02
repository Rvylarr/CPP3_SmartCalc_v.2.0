#include "creditwindow.h"

#include "ui_creditwindow.h"

CreditWindow::CreditWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::CreditWindow) {
  ui->setupUi(this);
  QWidget::setFixedSize(562, 888);
}

CreditWindow::~CreditWindow() {
  ui->m_payments->clear();
  delete ui;
}

void CreditWindow::on_calcButton_clicked() {
  this->close();
  emit main_window();
}

void CreditWindow::on_graphicButton_clicked() {
  this->close();
  emit main_window();
}

void CreditWindow::on_calculate_button_clicked() {
  ui->error_output->clear();
  results.clear();
  try {
    string error = controller.calculate_credit(
        ui->amount->text().toDouble(), ui->timeframe->text().toShort(),
        convert_to_date(ui->date->date().toString("d.M.yyyy").toStdString()),
        get_frequency(ui->frequency->currentText().toStdString()),
        ui->rate->text().toDouble(),
        ui->type->currentText() == "annuity" ? true : false, &results);
    generate_payments_table();
  } catch (exception &e) {
    ui->error_output->setText(e.what());
  }
}

void CreditWindow::generate_payments_table() {
  short size = results.size() - 2;
  ui->m_payments->clear();
  ui->m_payments->setRowCount(size);
  ui->m_payments->setMaximumHeight(400);
  ui->m_payments->setColumnCount(1);
  ui->m_payments->setColumnWidth(0, 300);

  for (int i = 0; i < size; i++) {
    QString str = QString::fromStdString(to_string(results[i]));
    str = chop_zeros(str);
    QTableWidgetItem *item = new QTableWidgetItem(str, 1);
    ui->m_payments->setItem(i, 0, item);
  }
  QString str = QString::fromStdString(to_string(results[results.size() - 2]));
  str = chop_zeros(str);
  ui->total_payment->setText(str);
  str = QString::fromStdString(to_string(results[results.size() - 1]));
  str = chop_zeros(str);
  ui->overpayment->setText(str);
}

QString CreditWindow::chop_zeros(QString s) {
  while (s.endsWith('0') && !s.endsWith(".0")) s.chop(1);
  return s;
}

tm CreditWindow::convert_to_date(string raw_date) {
  tm date;
  const char *str = raw_date.c_str();
  strptime(str, "%d.%m.%y", &date);
  return date;
}

short CreditWindow::get_frequency(string f) {
  short frequency = 0;

  if (f == "monthly")
    frequency = 1;
  else if (f == "quartaly")
    frequency = 3;
  else if (f == "yearly")
    frequency = 12;

  return frequency;
}
