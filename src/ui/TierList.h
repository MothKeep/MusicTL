#pragma once

#include <QFrame>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QApplication>
#include <qframe.h>
#include <qmainwindow.h>

class TierList: public QFrame{
  public:
    TierList(QWidget *parent = nullptr);
  private:
    QVBoxLayout* tierListLayout;
};

class Tier: public QFrame {
  
  public:
    Tier(const QString& name, QWidget *parent = nullptr);
  private:
    QLineEdit* titleEdit;
    QFrame* leftBox;
    QWidget* contentContainer;
    QVBoxLayout* contentLayout;
};
