#include "ControlDialog.h"
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCloseEvent>
/* TEST */
#include <iostream>

ControlDialog::ControlDialog(QWidget* parent) 
  : QDialog(parent) {
  playing = false;
  setWindowTitle("Controls");
  zoomLabel = new QLabel(tr("Zoom factor:"));
  zoomSpinBox = new QSpinBox;
  zoomSpinBox->setRange(1, 30);
  zoomSlider = new QSlider(Qt::Horizontal);
  zoomSlider->setRange(1, 30);

  delayLabel = new QLabel(tr("Delay:"));
  delaySpinBox = new QSpinBox;
  delaySpinBox->setRange(0, 2000);
  delaySlider = new QSlider(Qt::Horizontal);
  delaySlider->setRange(0, 2000);

  genLabel = new QLabel(tr("Generation:"));
  genNumLabel = new QLabel(tr("0"));

  quitButton = new QPushButton(tr("Quit"));
  playPauseButton = new QPushButton(tr("Play"));
  connect(playPauseButton, SIGNAL(clicked()), this, SLOT(playPauseClicked()));

  stepButton = new QPushButton(tr("Step"));
  connect(stepButton, SIGNAL(clicked()), this, SLOT(stepClicked()));

  connect(zoomSpinBox, SIGNAL(valueChanged(int)), zoomSlider, SLOT(setValue(int)));
  connect(zoomSlider, SIGNAL(valueChanged(int)), zoomSpinBox, SLOT(setValue(int)));
  connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoomChanged(int)));
  connect(quitButton, SIGNAL(clicked()), this, SIGNAL(dialogClosed()));
  connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
  zoomSpinBox->setValue(0);

  connect(delaySpinBox, SIGNAL(valueChanged(int)), delaySlider, SLOT(setValue(int)));
  connect(delaySlider, SIGNAL(valueChanged(int)), delaySpinBox, SLOT(setValue(int)));
  connect(delaySlider, SIGNAL(valueChanged(int)), this, SIGNAL(changeDelay(int)));
  connect(delaySlider, SIGNAL(valueChanged(int)), this, SLOT(delayChanged(int)));
  delaySpinBox->setValue(100);

  QHBoxLayout* zoomLayout = new QHBoxLayout;
  zoomLayout->addWidget(zoomLabel);
  zoomLayout->addWidget(zoomSpinBox);
  zoomLayout->addWidget(zoomSlider);

  QHBoxLayout* delayLayout = new QHBoxLayout;
  delayLayout->addWidget(delayLabel);
  delayLayout->addWidget(delaySpinBox);
  delayLayout->addWidget(delaySlider);

  QHBoxLayout* genLayout = new QHBoxLayout;
  genLayout->addWidget(genLabel);
  genLayout->addWidget(genNumLabel);

  QHBoxLayout* buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(quitButton);
  buttonsLayout->addWidget(playPauseButton);
  buttonsLayout->addWidget(stepButton);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(zoomLayout);
  mainLayout->addLayout(delayLayout);
  mainLayout->addLayout(genLayout);
  mainLayout->addLayout(buttonsLayout);

  setLayout(mainLayout);
  setFixedHeight(sizeHint().height());
}

void ControlDialog::closeEvent(QCloseEvent* event) {
  emit dialogClosed();
  event->accept();
}

void ControlDialog::playPauseClicked() {
  if(playing) {
    emit pauseAnimation();
    playPauseButton->setText("Play");
  }
  else {
    emit playAnimation();
    playPauseButton->setText("Pause");
  }
  playing = !playing; 
}

void ControlDialog::delayChanged(int newVal) {
  emit changeDelay(newVal);
}

void ControlDialog::zoomChanged(int newVal) {
  emit changeZoom(newVal);
}

void ControlDialog::stepClicked() {
  emit stepGeneration();
}
