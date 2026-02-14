#include "Settings.h"
#include <QMenu>
#include <QMenuBar>

Settings::Settings(QWidget *parent) : QMainWindow(parent) {
  setObjectName("Settings");
  setFixedWidth(40);

  auto *quit = new QAction("&Quit", this);
  this->setContentsMargins(0,0,0,0);
  this->setStyleSheet("background-color: #111111; color: #edffd3;"); 

  QMenu *file = menuBar()->addMenu("File");
  file->addAction(quit);

  connect(quit, &QAction::triggered, qApp, QApplication::quit);
}
