#pragma once

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput> 
#include <QAudioOutput>
#include <QPushButton>
#include <QVBoxLayout>

class Player: public QWidget{
  Q_OBJECT
  public:
    Player(QWidget *parent = nullptr);
  public slots:
    void playTrack(const QString &path) {
      player->setSource(QUrl::fromLocalFile(path));
      player->play();
    }
  private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QPushButton *playButton;
};

