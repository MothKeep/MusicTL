#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QFile>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QFile file("../src/style.qss");
  if (file.open(QFile::ReadOnly)){
    app.setStyleSheet(file.readAll());
  }

  MainWindow window;
  window.setWindowTitle("Music Tier List");
  window.setFixedSize(1180, 850);
  window.show();   

  return app.exec();
}
