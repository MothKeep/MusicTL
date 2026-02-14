#include "TierList.h"
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <qboxlayout.h>
#include <qframe.h>
#include <qwidget.h>

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
    tierListLayout->insertWidget(tierListLayout->count()-2, newTier); 
  });
  
  tierListLayout->addStretch();
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

  titleEdit = new QLineEdit(name);
  titleEdit->setStyleSheet("font-size: 24px; font-weight: bold; color: white; background: transparent; border: none;");
  titleEdit->setAlignment(Qt::AlignCenter);
  leftLayout->addWidget(titleEdit);

  mainLayout->addWidget(leftBox);
  
  contentContainer = new QWidget;
  contentContainer->setStyleSheet("background-color: #333; border-radius: 6px;");
  contentLayout = new QVBoxLayout(contentContainer);
  contentLayout->setContentsMargins(5,5,5,5);
  contentLayout->setSpacing(5);
  contentLayout->addStretch();

  mainLayout->addWidget(contentContainer, 1);
};
