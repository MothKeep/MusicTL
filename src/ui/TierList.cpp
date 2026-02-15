#include "TierList.h"
#include "Content.h"
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <qboxlayout.h>
#include <qframe.h>
#include <qwidget.h>
#include <QColorDialog>
#include <QMouseEvent>
#include <QMimeData>

TierList::TierList(QWidget *parent) : QFrame(parent) {
  setObjectName("TierList");
  this->setContentsMargins(0,0,0,0);
  this->setStyleSheet("background-color: #222222; color: #edffd3;"); 
   
  tierListLayout= new QVBoxLayout(this); 
  tierListLayout->setContentsMargins(10,10,10,10);
  tierListLayout->setSpacing(10);
  

  QStringList defaultTiers = {
    "S", "A", "B", "C", "F"
  };
  for(const auto& t : defaultTiers){
    auto* tier = new Tier(t);
    tierListLayout->addWidget(tier);
  }

  auto* emptyTier = new QFrame;
  emptyTier->setFixedHeight(60);
  emptyTier->setStyleSheet("background-color: #444; border-radius: 6px;");
  auto* emptyLayout = new QHBoxLayout(emptyTier);
  emptyLayout->setContentsMargins(5,5,5,5);
  
  auto* addBtn = new QPushButton("+");
  addBtn->setStyleSheet("font-size: 24px; font-weight: bold; color: white; background-color: #555; border-radius: 6px;");
  addBtn->setFixedSize(50,50);
  emptyLayout->addStretch();
  emptyLayout->addWidget(addBtn);
  emptyLayout->addStretch();

  tierListLayout->addWidget(emptyTier);

  QObject::connect(addBtn, &QPushButton::clicked, [this](){
    auto* newTier = new Tier("New");
    tierListLayout->insertWidget(tierListLayout->count()-1, newTier); 
  });
  
  //tierListLayout->addStretch();
};

Tier::Tier(const QString& name, QWidget *parent) : QFrame(parent){
  setObjectName("Tier");
  setMinimumHeight(100);
  setFrameShape(QFrame::StyledPanel);
  this->setContentsMargins(0,0,0,0);

  auto* mainLayout = new QHBoxLayout(this);
  mainLayout->setContentsMargins(5,5,5,5);
  mainLayout->setSpacing(5);

  leftBox = new QFrame;
  leftBox->setFixedWidth(100);

  QString bgColor;
  if(name == "S") bgColor = "#ddc400";
  else if(name == "A") bgColor = "#00C853";
  else if(name == "B") bgColor = "#2196F3";
  else if(name == "C") bgColor = "#FF9800";
  else if(name == "F") bgColor = "#F44336";
  else bgColor = "#343434";
  
  leftBox->setStyleSheet(QString("background-color: %1; border-radius: 6px;").arg(bgColor));
  auto* leftLayout = new QVBoxLayout(leftBox);
  leftLayout->setContentsMargins(5,5,5,5);
  leftLayout->setSpacing(2);
      
  auto* topRow = new QHBoxLayout;
  topRow->setContentsMargins(0,0,0,0);
  topRow->setSpacing(2);
  topRow->addStretch();
  
  QPushButton* colorBtn = new QPushButton("c");
  QPushButton* deleteBtn = new QPushButton("x");
  colorBtn->setFixedSize(20,20);
  deleteBtn->setFixedSize(20,20);
  colorBtn->setStyleSheet("background:transparent; color: white;"); 
  deleteBtn->setStyleSheet("background:transparent; color: white;");
      
  topRow->addWidget(colorBtn);
  topRow->addWidget(deleteBtn);
  leftLayout->addLayout(topRow);
  
  connect(deleteBtn, &QPushButton::clicked, [this](){ this->deleteLater(); });
  connect(colorBtn, &QPushButton::clicked, [this](){
  QColor color = QColorDialog::getColor(Qt::white, this, "Choose Tier Color");
  if(color.isValid())
    leftBox->setStyleSheet(QString("background-color: %1; border-radius: 6px;").arg(color.name()));
  });

  titleEdit = new QLineEdit(name);
  titleEdit->setStyleSheet("font-size: 24px; font-weight: bold; color: white; background: transparent; border: none;");
  titleEdit->setAlignment(Qt::AlignCenter);
  leftLayout->addWidget(titleEdit);
  leftLayout->addStretch();

  mainLayout->addWidget(leftBox);
  
  contentContainer = new QWidget;
  contentContainer->setStyleSheet("background-color: #333; border-radius: 6px;");
  contentContainer->setAcceptDrops(true);
  contentLayout = new QHBoxLayout(contentContainer);
  contentLayout->setContentsMargins(5,5,5,5);
  contentLayout->setSpacing(5);
  contentLayout->addStretch();
  contentContainer->installEventFilter(this);

  mainLayout->addWidget(contentContainer, 1);
};

void Tier::mousePressEvent(QMouseEvent* event) {
  if (event->button() != Qt::LeftButton)
    return;

  dragging = true;

  grabOffset = event->pos();

  auto* layout = qobject_cast<QVBoxLayout*>(parentWidget()->layout());
  if (!layout) return;

  int index = layout->indexOf(this);

  dragPlaceholder = new QWidget;
  dragPlaceholder->setFixedHeight(height());
  layout->insertWidget(index, dragPlaceholder);

  layout->removeWidget(this);

  setParent(parentWidget());
  setAttribute(Qt::WA_TransparentForMouseEvents);
  raise();
}

void Tier::mouseMoveEvent(QMouseEvent* event) {
  if (!dragging) return;

  QPoint globalPos = event->globalPosition().toPoint();
  QPoint parentPos = parentWidget()->mapFromGlobal(globalPos - grabOffset);
  parentPos.setX(this->x()); 
  move(parentPos);

  auto* layout = qobject_cast<QVBoxLayout*>(parentWidget()->layout());
  if (!layout) return;

  int mouseY = globalPos.y();
  int targetIndex = layout->count() - 1; 

  QList<QWidget*> realTiers;
  for (int i = 0; i < layout->count() - 1; ++i) {
    QWidget* w = layout->itemAt(i)->widget();
    if (w && w != dragPlaceholder)
      realTiers.append(w);
  }

  for (int i = 0; i < realTiers.size(); ++i) {
    QWidget* w = realTiers[i];
    QRect geom = w->geometry();
        
    int thresholdY = geom.top() + geom.height() / 2;
    if (mouseY < w->mapToGlobal(QPoint(0, geom.height() / 2)).y()) {
      targetIndex = i;
      break;
    }
  }

  targetIndex = qBound(0, targetIndex, layout->count() - 2);

  if (targetIndex != currentPlaceholderIndex) {
    layout->removeWidget(dragPlaceholder);
    layout->insertWidget(targetIndex, dragPlaceholder);
    currentPlaceholderIndex = targetIndex;
  }
}

void Tier::mouseReleaseEvent(QMouseEvent*) {
  if (!dragging) return;
  dragging = false;

  auto* layout = qobject_cast<QVBoxLayout*>(parentWidget()->layout());
  if (!layout) return;

  int index = layout->indexOf(dragPlaceholder);
  layout->removeWidget(dragPlaceholder);
  dragPlaceholder->deleteLater();
  dragPlaceholder = nullptr;

  setAttribute(Qt::WA_TransparentForMouseEvents, false);
  layout->insertWidget(index, this);
}


void Tier::dragEnterEvent(QDragEnterEvent* event){
  if(event->mimeData()->hasFormat("application/x-track")) event->acceptProposedAction();
}

void Tier::dropEvent(QDropEvent* event){
  Track* track = qobject_cast<Track*>(event->source());
  if(track){
    if(track->parentWidget() && track->parentWidget()->layout()){
      QLayout* oldLayout = track->parentWidget()->layout();
      oldLayout->removeWidget(track);
    }
    track->setParent(contentContainer);
    contentLayout->addWidget(track);
    track->show();
    event->acceptProposedAction();
  }
}

bool Tier::eventFilter(QObject* obj, QEvent* event) {
  if(obj == contentContainer) {
    if(event->type() == QEvent::DragEnter) {
      QDragEnterEvent* dragEvent = static_cast<QDragEnterEvent*>(event);
      if(dragEvent->mimeData()->hasFormat("application/x-track"))
        dragEvent->acceptProposedAction();
      return true;
    }
    if(event->type() == QEvent::Drop) {
      QDropEvent* dropEvent = static_cast<QDropEvent*>(event);
      Track* track = qobject_cast<Track*>(dropEvent->source());
      if(track){
        if(track->parentWidget() && track->parentWidget()->layout())
          track->parentWidget()->layout()->removeWidget(track);

        track->setParent(contentContainer); 
        contentLayout->insertWidget(contentLayout->count() - 1, track); 
        track->show();
        dropEvent->acceptProposedAction();
      }
      return true;
    }
  }
  return QFrame::eventFilter(obj, event);
}
