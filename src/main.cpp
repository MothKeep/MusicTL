#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QFile>

#include "MainWindow.h"

QString loadStyleSheet(const QString& path){
  QFile file(path);
  if (!file.open(QFile::ReadOnly))
    return "";

  return QString::fromUtf8(file.readAll());
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QString style;
  style += loadStyleSheet("../src/styles/style.qss");
  style += loadStyleSheet("../src/styles/scrollBar.qss");

  app.setStyleSheet(style);

  MainWindow window;
  window.setWindowTitle("Music Tier List");
  window.setFixedSize(1180, 850);
  window.show();   

  return app.exec();
}
