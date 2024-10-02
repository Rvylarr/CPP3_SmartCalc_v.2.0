#include "eventbox.h"

EventBox::EventBox(short id, char type, QWidget *parent)
    : QHBoxLayout(parent), type_(type) {
  num = new QLabel();
  num->setText(QString::number(id));
  num->setFixedSize(25, 50);

  sum = new QDoubleSpinBox();
  sum->setFixedSize(150, 50);
  sum->setMaximum(99999999);
  sum->setMinimum(0);
  sum->setButtonSymbols(QAbstractSpinBox::NoButtons);

  date = new QDateEdit();
  date->setFixedSize(125, 50);
  date->setDisplayFormat("d.M.yyyy");
  date->setDate(QDate::currentDate());
  date->setButtonSymbols(QAbstractSpinBox::NoButtons);

  period = new QComboBox();
  period->setFixedSize(150, 50);
  period->addItem("one-time");
  period->addItem("everyday");
  period->addItem("monthly");
  period->addItem("quartaly");
  period->addItem("yearly");

  delete_button = new QPushButton();
  delete_button->setFixedSize(50, 50);
  delete_button->setText("X");

  this->setContentsMargins(5, 5, 0, 5);
  this->addWidget(num);
  this->addWidget(sum);
  this->addWidget(date);
  this->addWidget(period);
  this->addWidget(delete_button);
}

EventBox::~EventBox() {
  delete num;
  delete sum;
  delete date;
  delete period;
  delete delete_button;
}

char EventBox::get_type() { return type_; }

short EventBox::get_id() { return this->num->text().toShort(); }

double EventBox::get_sum() { return this->sum->text().toDouble(); }

string EventBox::get_date() {
  QString s = this->date->date().toString("d.M.yyyy");
  return s.toStdString();
}

string EventBox::get_period() {
  QString s = this->period->currentText();
  return s.toStdString();
}
