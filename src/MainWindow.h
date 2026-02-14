#pragma once
#include <QMainWindow>
#include "ui/Player.h"
#include "ui/Settings.h"
#include "ui/TierList.h"
#include "ui/Content.h"

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget* parent = nullptr);

private:
  Player* player;
  Settings* settings;
  TierList* tierlist;
  Content* content;
};
