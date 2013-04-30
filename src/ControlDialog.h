#ifndef CONTROL_DIALOG_H
#define CONTROL_DIALOG_H

#include <QDialog>

class QSpinBox;
class QSlider;
class QLabel;
class QPushButton;
class QCloseEvent;

class ControlDialog : public QDialog {
    Q_OBJECT
  public:
    ControlDialog(QWidget* parent = 0);
  signals:
    void stepGeneration();
    void playAnimation();
    void pauseAnimation();
    void changeDelay(int delay);
    void dialogClosed();
    void changeZoom(int zoom);
  private slots:
    void playPauseClicked();
    void stepClicked();
    void delayChanged(int newVal);
    void zoomChanged(int newVal);
  private:
    bool playing;
    void closeEvent(QCloseEvent* event);
    QLabel* zoomLabel;
    QSpinBox* zoomSpinBox;
    QSlider* zoomSlider;
    QLabel* delayLabel;
    QSpinBox* delaySpinBox;
    QSlider* delaySlider;
    QLabel* genLabel;
    QLabel* genNumLabel;
    QPushButton* quitButton;
    QPushButton* playPauseButton;
    QPushButton* stepButton;
};

#endif
