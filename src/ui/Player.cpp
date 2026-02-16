#include "Player.h"
#include <QMenu>
#include <QMenuBar>

Player::Player(QWidget *parent) : QWidget(parent) {
  setObjectName("Player");
  setFixedHeight(140);
  this->setContentsMargins(0,0,0,0);
  this->setStyleSheet("background-color: #0f0f0f; color: #edffd3;"); 

  player = new QMediaPlayer(this);
  audioOutput = new QAudioOutput(this);
  player->setAudioOutput(audioOutput);
  audioOutput->setVolume(20.0); 
  
  playButton = new QPushButton("Play/Pause", this);
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(playButton);
  setLayout(layout);

  connect(playButton, &QPushButton::clicked, [this]() {
    if (player->playbackState() == QMediaPlayer::PlayingState)
      player->pause();
    else
      player->play();
  });
}
