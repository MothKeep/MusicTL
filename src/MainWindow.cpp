#include "MainWindow.h"
#include "ui/Player.h"
#include "ui/Settings.h"
#include "ui/TierList.h"
#include "ui/Content.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>

MainWindow::MainWindow(QWidget* parent, std::string path) : QMainWindow(parent){
  QWidget* central = new QWidget(this);
  setCentralWidget(central);

  auto* root = new QHBoxLayout(central);
  root->setContentsMargins(0,0,0,0);
  root->setSpacing(0);
  
  QWidget* right = new QWidget(central);
  auto* rightLayout = new QVBoxLayout(right);
  rightLayout->setContentsMargins(0,0,0,0);
  rightLayout->setSpacing(0);

  QWidget* top = new QWidget(right);
  auto* topLayout = new QHBoxLayout(top);
  topLayout->setContentsMargins(0,0,0,0);
  topLayout->setSpacing(4);
  top->setStyleSheet("background-color: #0f0f0f;");
  player = new Player(right);
  settings = new Settings(central);
  tierlist = new TierList(top);
  content = new Content(player,top,path);
  
  auto *TierScroll = new QScrollArea;
  TierScroll->setWidget(tierlist);
  TierScroll->setWidgetResizable(true);
  TierScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  TierScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  TierScroll->setStyleSheet("");

  root->addWidget(settings);
  root->addWidget(right);
  rightLayout->addWidget(top);
  rightLayout->addWidget(player);
  topLayout->addWidget(TierScroll, 64);
  topLayout->addWidget(content, 36);
}
