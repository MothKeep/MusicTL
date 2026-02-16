#include "Content.h"
#include "Player.h"
#include <QMenu>
#include <QMenuBar>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>

Content::Content(Player* player, QWidget *parent) : QFrame(parent) {
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

  auto* track = new Track("/home/artiz/Downloads/G.mp3", player);
  ContentLayout->addWidget(track);

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
Track::Track(const QString& filePath, Player* player, const QPixmap& cover, QWidget* parent)
    : QFrame(parent), m_filePath(filePath), m_player(player), m_cover(cover){
  setObjectName("Track");
  setFixedSize(90,115);
  setFrameShape(QFrame::StyledPanel);
  this->setContentsMargins(0,0,0,0);
  this->setStyleSheet("background-color: #444; border-radius: 12px;"); 
  dragging = false;
  
  auto* mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(5,5,5,5);
  mainLayout->setSpacing(5);

  TopBox = new QFrame;
  TopBox ->setFixedSize(80, 80);
  TopBox->setStyleSheet("background-color: #555; border-radius: 4px;"); 

  if(!m_cover.isNull()) {
    QLabel* coverLabel = new QLabel(TopBox);
    coverLabel->setPixmap(m_cover.scaled(80,80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    coverLabel->setAlignment(Qt::AlignCenter);
    coverLabel->setContentsMargins(0,0,0,0);
  }

  NameBox = new QFrame;
  NameBox->setFixedSize(80, 20);
  NameBox->setStyleSheet("background-color: #555; color: #edffd3; border-radius: 4px;"); 
  auto* label = new QLabel(QFileInfo(filePath).fileName(), NameBox);
  label->setAlignment(Qt::AlignCenter);
  
  mainLayout->addWidget(TopBox, 0, Qt::AlignHCenter);
  mainLayout->addWidget(NameBox, 0, Qt::AlignHCenter);
  
  playOverlay = new QPushButton(this);
  playOverlay->setText("▶");
  playOverlay->setStyleSheet("background-color: rgba(0,0,0,0.5); color: white; border-radius: 12px;");
  playOverlay->setFixedSize(30,30);
  playOverlay->move((width() - playOverlay->width())/2, 30);
  playOverlay->hide();

  connect(playOverlay, &QPushButton::clicked, this, &Track::playClicked);

};
  
void Track::enterEvent(QEnterEvent* event) {
  QFrame::enterEvent(event);  
  playOverlay->show();
}

void Track::leaveEvent(QEvent* event) {
  QFrame::leaveEvent(event);
  playOverlay->hide();
}

void Track::playClicked() {
  if (m_player) {
    QString path = m_filePath;
    if (path.startsWith("~"))
      path = path.mid(1);
    m_player->playTrack(path);
  }
}

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
