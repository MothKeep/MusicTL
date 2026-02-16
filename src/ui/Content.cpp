#include "Content.h"
#include "Player.h"
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include <QPainterPath>
#include <QFileDialog>
#include <QDirIterator>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/attachedpictureframe.h>

QPixmap extractAlbumCover(const QString& filePath){
  TagLib::MPEG::File file(filePath.toUtf8().constData());
  if (!file.isValid()) return QPixmap();

  TagLib::ID3v2::Tag* tag = file.ID3v2Tag();
  if (!tag) return QPixmap();

  auto frames = tag->frameListMap()["APIC"];
  if (frames.isEmpty()) return QPixmap();

  auto* picFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frames.front());
  if (!picFrame) return QPixmap();
  
  QByteArray imgData(
    picFrame->picture().data(),
    picFrame->picture().size()
  );

  QPixmap cover;
  cover.loadFromData(imgData);
 
  return cover;
}

Content::Content(Player* player, QWidget *parent, std::string path) : QFrame(parent), m_player(player) {
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
  
  auto* flowContainer = new QWidget(this);
  ContentLayout = new FlowLayout(flowContainer);
  ContentLayout->setContentsMargins(0,0,0,0);
  ContentLayout->setSpacing(10);

  root->addWidget(flowContainer, 1);

  root->addLayout(ContentLayout, 1);
  root->addStretch(); 
  root->addLayout(bottomBar);

  QDirIterator it(
    QString::fromStdString(path),
    {"*.mp3", "*.wav", "*.flac", "*.ogg"},
    QDir::Files,
    QDirIterator::Subdirectories
  );

  while (it.hasNext()) {
    QString filePath = it.next();

    auto* track = new Track(filePath, m_player, this);
    ContentLayout->addWidget(track);
  }

  QObject::connect(addBtn, &QPushButton::clicked, [this](){
    QString filePath = QFileDialog::getOpenFileName(this, "Add Music Track", QDir::homePath(), "Audio Files (*.mp3 *.wav *.flac *.ogg)");

    if (filePath.isEmpty())
      return;

    auto* track = new Track(filePath, m_player, this);
    ContentLayout->addWidget(track);
    track->show();
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
Track::Track(const QString& filePath, Player* player, QWidget* parent)
    : QFrame(parent), m_filePath(filePath), m_player(player){ 
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
  QPixmap cover = extractAlbumCover(filePath);

  if(!cover.isNull()) {
    QPixmap scaled = cover.scaled(80, 80, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QLabel* coverLabel = new QLabel(TopBox);
    
    QPixmap rounded(scaled.size());
    rounded.fill(Qt::transparent);

    QPainter p(&rounded);
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rounded.rect(), 8, 8);
    p.setClipPath(path);
    p.drawPixmap(0, 0, scaled);
    
    coverLabel->setPixmap(rounded);
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
