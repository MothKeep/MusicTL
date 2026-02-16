#include "Settings.h"
#include <qwidget.h>
#include <QVBoxLayout>
#include <QPushButton>

Settings::Settings(QWidget *parent) : QWidget(parent) {
  setObjectName("Settings");
  setFixedWidth(40);
  setStyleSheet("background-color: #111111; color: #edffd3;");
  
  auto *layout = new QVBoxLayout(this);
 
  layout->setContentsMargins(0, 0, 0, 0);
  
  auto *quitBtn = new QPushButton("Quit", this);
  quitBtn->setFixedSize(32, 32);
  layout->addWidget(quitBtn, 0, Qt::AlignHCenter | Qt::AlignTop);

  layout->addStretch();

  connect(quitBtn, &QPushButton::clicked, qApp, &QApplication::quit);
}
