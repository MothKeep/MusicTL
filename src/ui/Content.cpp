#include "Content.h"
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QPixmap>

Content::Content(QWidget *parent) : QFrame(parent) {
  setObjectName("Content");
  this->setContentsMargins(0,0,0,0);
  this->setStyleSheet("background-color: #222222; color: #edffd3;"); 
  setAcceptDrops(true);
  
  auto* root = new QVBoxLayout(this);
  root->setContentsMargins(10,10,10,10);
  root->setSpacing(10);
  
  auto* bottomBar = new QHBoxLayout;
  bottomBar->addStretch();
  
  auto* newTrack = new QFrame;
  newTrack->setFixedHeight(80);
  newTrack->setFixedWidth(80);
  newTrack->setStyleSheet("background-color: #444; border-radius: 6px;");
  auto* newLayout = new QHBoxLayout(newTrack);
  newLayout->setContentsMargins(5,5,5,5);
  
  auto* addBtn = new QPushButton("+");
  addBtn->setStyleSheet("font-size: 49px; color: white; background-color: #555; border-radius: 6px;");
  addBtn->setFixedSize(70,70);
  newLayout->addWidget(addBtn);

  bottomBar->addWidget(newTrack);
  
  ContentLayout = new FlowLayout;
  ContentLayout->setSpacing(10);

  for(int i=0; i<10; i++){
    auto* track = new Track();
    ContentLayout->addWidget(track);
  }

  root->addLayout(ContentLayout, 1);
  root->addLayout(bottomBar);

  QObject::connect(addBtn, &QPushButton::clicked, [this](){
  });
  
}

void Content::dragEnterEvent(QDragEnterEvent* event){
  if(event->mimeData()->hasFormat("application/x-track")) event->acceptProposedAction();
}

void Content::dropEvent(QDropEvent* event){
  Track* track = qobject_cast<Track*>(event->source());
  if(track){
    if(track->parentWidget() && track->parentWidget()->layout()){
      track->parentWidget()->layout()->removeWidget(track);
    }
    track->setParent(this);
    ContentLayout->addWidget(track);
    track->show();
    event->acceptProposedAction();
  }
}

//Track
Track::Track(QWidget *parent) : QFrame(parent){
  setObjectName("Track");
  setMinimumHeight(100);
  setMinimumWidth(100);
  setFrameShape(QFrame::StyledPanel);
  this->setContentsMargins(0,0,0,0);
  this->setStyleSheet("background-color: #444; border-radius: 6px;"); 
  dragging = false;
  
  auto* mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(5,5,5,5);
  mainLayout->setSpacing(5);

  TopBox = new QFrame;
  TopBox ->setFixedHeight(80);
  TopBox ->setFixedWidth(80);
  TopBox->setStyleSheet("background-color: #555; border-radius: 4px;"); 

  auto* topLayout = new QVBoxLayout(TopBox);
  topLayout->setContentsMargins(5,5,5,5);
  topLayout->setSpacing(2);
      
  NameBox = new QFrame;
  NameBox->setFixedHeight(20);
  NameBox->setFixedWidth(80);
  NameBox->setStyleSheet("background-color: #555; color: #edffd3; border-radius: 4px;"); 

  mainLayout->addWidget(TopBox, 0, Qt::AlignHCenter);
  mainLayout->addWidget(NameBox, 0, Qt::AlignHCenter);
};
  
void Track::mousePressEvent(QMouseEvent* event){
  if(event->button() != Qt::LeftButton) return;

  QDrag *drag = new QDrag(this);
  QMimeData *mime = new QMimeData;
  mime->setData("application/x-track", QByteArray());
  drag->setMimeData(mime);

  QPixmap pixmap(size());
  render(&pixmap);
  drag->setPixmap(pixmap);
  drag->setHotSpot(event->pos());

  hide();

  if(drag->exec(Qt::MoveAction) != Qt::MoveAction){
    show();
  }
}
