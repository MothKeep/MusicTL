#include "MainWindow.h"
#include "ui/Player.h"
#include "ui/Settings.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent){
  QWidget* central = new QWidget(this);
  setCentralWidget(central);

  auto* root = new QHBoxLayout(central);
  
  QWidget* right = new QWidget;
  auto* rightLayout = new QVBoxLayout(right);
  
  QWidget* top = new QWidget;
  auto* topLayout = new QHBoxLayout(top);
  player = new Player;
  settings = new Settings;

  root->addWidget(settings, 30);
  root->addWidget(right);
  rightLayout->addWidget(top);
  rightLayout->addWidget(player);
  //topLayout->addWidget(tierlist);
  //topLayout->addWidget(content);
}
