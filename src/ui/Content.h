#pragma once

#include "FlowLayout.h"
#include "Player.h"
#include <QFrame>
#include <QEvent>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QFileInfo>
#include <QDrag>
#include <QMimeData>
#include <QEnterEvent>
#include <string>

class Content: public QFrame{
  Q_OBJECT
  public:
    Content(Player* player, QWidget *parent = nullptr, std::string path="");

  protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
  
  private:
    FlowLayout* ContentLayout;
    Player* m_player;
};

class Track: public QFrame{
  Q_OBJECT
  public:
    explicit Track(const QString& filePath, Player* player, QWidget *parent = nullptr);
    
    QString filePath() const { return m_filePath; }

  protected:
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
  private slots:
    void playClicked();
  private:
    QString m_filePath;
    Player* m_player;
    QPushButton* playOverlay; 
    
    QFrame* TopBox;
    QFrame* NameBox;
    
    bool dragging=false;
};
