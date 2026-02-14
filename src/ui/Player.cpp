#include "Player.h"
#include <QMenu>
#include <QMenuBar>

Player::Player(QWidget *parent) : QFrame(parent) {
  setObjectName("Player");
  setFixedHeight(180);
  this->setContentsMargins(0,0,0,0);
  this->setStyleSheet("background-color: #0f0f0f; color: #edffd3;"); 
}
