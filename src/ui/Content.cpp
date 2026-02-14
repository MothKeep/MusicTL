#include "Content.h"
#include <QMenu>
#include <QMenuBar>

Content::Content(QWidget *parent) : QFrame(parent) {
  setObjectName("Content");
  this->setContentsMargins(0,0,0,0);
  this->setStyleSheet("background-color: #222222; color: #edffd3;"); 
}
