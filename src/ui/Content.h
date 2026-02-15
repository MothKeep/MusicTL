#pragma once

#include "FlowLayout.h"
#include <QFrame>
#include <QApplication>
#include <QLayout>
#include <qevent.h>
#include <qframe.h>
#include <qtmetamacros.h>
#include <qwidget.h>

class Content: public QFrame{
  Q_OBJECT
  public:
    Content(QWidget *parent = nullptr);
    FlowLayout* ContentLayout;

  protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
  
  private:
};

class Track: public QFrame{
  Q_OBJECT
  public:
    Track(QWidget *parent = nullptr);

  protected:
    void mousePressEvent(QMouseEvent* event) override;

  private:
    QFrame* TopBox;
    QFrame* NameBox;
    
    bool dragging;
};
