#ifndef EVENTBOX_H
#define EVENTBOX_H

#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <string>

using namespace std;

class EventBox : public QHBoxLayout {
  Q_OBJECT

 public:
  explicit EventBox(short id, char type, QWidget *parent = 0);
  ~EventBox();

  // --- getters --- //
  char get_type();
  short get_id();
  double get_sum();
  string get_date();
  string get_period();

  QPushButton *delete_button;

 private:
  char type_;
  QLabel *num;
  QDoubleSpinBox *sum;
  QDateEdit *date;
  QComboBox *period;
};

#endif  // EVENTBOX_H
