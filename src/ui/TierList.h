#pragma once

#include <QFrame>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QApplication>
#include <qframe.h>
#include <qmainwindow.h>

class TierList: public QFrame{
  public:
    TierList(QWidget *parent = nullptr);
  private:
    QVBoxLayout* tierListLayout;
};

class Tier: public QFrame {
  Q_OBJECT

  public:
    Tier(const QString& name, QWidget *parent = nullptr);
    QHBoxLayout* contentLayout;
    QWidget* contentContainer;
  
  protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override; 
  
  private:
    QLineEdit* titleEdit;
    QFrame* leftBox;
    
    QPoint dragStartPos;
    bool dragging = false;
    QWidget* dragPlaceholder = nullptr;
    int currentPlaceholderIndex = -1;
    QPoint grabOffset;

};
