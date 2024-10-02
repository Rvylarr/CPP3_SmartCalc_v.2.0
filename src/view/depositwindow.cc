#include "depositwindow.h"

#include "ui_depositwindow.h"

DepositWindow::DepositWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::DepositWindow) {
  ui->setupUi(this);
  QWidget::setFixedSize(1124, 888);
  ui->add_supplementions->setFixedHeight(50);
  ui->add_eliminations->setFixedHeight(50);
  ui->closure_date->setVisible(false);
  ui->date->setDate(QDate::currentDate());

  spl_count = 0;
  elm_count = 0;
}

DepositWindow::~DepositWindow() {
  while (spls.size() != 0) {
    delete spls[spls.size() - 1];
    spls.pop_back();
  }

  while (elms.size() != 0) {
    delete elms[elms.size() - 1];
    elms.pop_back();
  }
  ui->payments_list->clear();
  delete ui;
}

// --- slots --- //

void DepositWindow::on_calcButton_clicked() {
  this->close();
  emit main_window();
}

void DepositWindow::on_graphicButton_clicked() {
  this->close();
  emit main_window();
}

void DepositWindow::on_calculate_button_clicked() {
  vector<long double> amounts;
  vector<tm> dates;
  vector<string> types;
  bool capitalisation = false;

  // 1 - базовый высчет
  amounts.push_back(ui->rate->text().toDouble());
  dates.push_back(
      convert_to_date(ui->date->date().toString("d.M.yyyy").toStdString()));
  types.push_back("1");

  // 2 - налог
  amounts.push_back(ui->tax->text().toDouble());
  dates.push_back(
      convert_to_date(ui->date->date().toString("d.M.yyyy").toStdString()));
  types.push_back("2");

  // 3 - периодичность выплат
  amounts.push_back(0);
  dates.push_back(
      convert_to_date(ui->date->date().toString("d.M.yyyy").toStdString()));
  types.push_back("3" +
                  get_frequency(ui->frequency->currentText().toStdString()));

  // 4 - капитализация
  if (ui->capitalisation->isChecked()) capitalisation = true;

  // 5 - пополнения
  if (!spls.empty()) {
    for (auto i = 0; i < spl_count; i++) {
      amounts.push_back(spls[i]->get_sum());
      dates.push_back(convert_to_date(spls[i]->get_date()));
      types.push_back("5" + get_frequency(spls[i]->get_period()));
    }
  }

  // 6 - снятие
  if (!elms.empty()) {
    for (auto i = 0; i < elm_count; i++) {
      amounts.push_back(elms[i]->get_sum());
      dates.push_back(convert_to_date(elms[i]->get_date()));
      types.push_back("6" + get_frequency(elms[i]->get_period()));
    }
  }

  // 7 - досрочное закрытие
  if (ui->early_closure->isChecked()) {
    amounts.push_back(0);
    dates.push_back(convert_to_date(
        ui->closure_date->date().toString("d.M.yyyy").toStdString()));
    types.push_back("7");
  }

  try {
    ui->error_output->clear();
    string error = controller.calculate_deposit(
        &amounts, &dates, &types, ui->amount->text().toDouble(),
        ui->timeframe->text().toShort(), capitalisation);
    generate_payments_table(amounts);
  } catch (exception &e) {
    ui->error_output->setText(e.what());
  }
}

void DepositWindow::on_add_supplementions_clicked() {
  spl_count++;
  EventBox *button = new EventBox(spl_count, 5);
  connect(button->delete_button, &QPushButton::clicked, this,
          &DepositWindow::delete_supplementions);
  spls.push_back(button);
  ui->supplementions_list->addLayout(button);
}

void DepositWindow::on_add_eliminations_clicked() {
  elm_count++;
  EventBox *button = new EventBox(elm_count, 5);
  connect(button->delete_button, &QPushButton::clicked, this,
          &DepositWindow::delete_eliminations);
  elms.push_back(button);
  ui->eliminations_list->addLayout(button);
}

void DepositWindow::on_early_closure_stateChanged(int arg1) {
  arg1 == 2 ? ui->closure_date->setVisible(true)
            : ui->closure_date->setVisible(false);
}

// --- methods --- //

void DepositWindow::delete_supplementions() {
  delete spls[spls.size() - 1];
  spls.pop_back();
  spl_count--;
}

void DepositWindow::delete_eliminations() {
  delete elms[elms.size() - 1];
  elms.pop_back();
  elm_count--;
}

void DepositWindow::generate_payments_table(vector<long double> results) {
  short size = results.size() - 2;
  ui->payments_list->clear();
  ui->payments_list->setRowCount(size);
  ui->payments_list->setMaximumHeight(400);
  ui->payments_list->setColumnCount(1);
  ui->payments_list->setColumnWidth(0, 300);

  for (int i = 0; i < size; i++) {
    QString str = QString::fromStdString(to_string(results[i]));
    str = chop_zeros(str);
    QTableWidgetItem *item = new QTableWidgetItem(str, 1);
    ui->payments_list->setItem(i, 0, item);
  }
  QString str = QString::fromStdString(to_string(results[results.size() - 1]));
  str = chop_zeros(str);
  ui->total_amount->setText(str);
  str = QString::fromStdString(to_string(results[results.size() - 2]));
  str = chop_zeros(str);
  ui->tax_deduction->setText(str);
}

tm DepositWindow::convert_to_date(string raw_date) {
  tm date;
  const char *str = raw_date.c_str();
  strptime(str, "%d.%m.%y", &date);
  return date;
}

string DepositWindow::get_frequency(string f) {
  string frequency = "n";

  if (f == "one-time")
    frequency = "o";
  else if (f == "everyday")
    frequency = "d";
  else if (f == "monthly")
    frequency = "m";
  else if (f == "quartaly")
    frequency = "q";
  else if (f == "yearly")
    frequency = "y";

  return frequency;
}

QString DepositWindow::chop_zeros(QString s) {
  while (s.endsWith('0') && !s.endsWith(".0")) s.chop(1);
  return s;
}
