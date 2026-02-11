#include "Settings.h"
#include <QMenu>
#include <QMenuBar>

Settings::Settings(QWidget *parent) : QMainWindow(parent) {
  auto *quit = new QAction("&Quit", this);

  QMenu *file = menuBar()->addMenu("&File");
  file->addAction(quit);

  connect(quit, &QAction::triggered, qApp, QApplication::quit);
}
