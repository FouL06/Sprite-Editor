#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "frame.h"
#include "framebutton.h"
#include <QMainWindow>
#include <QMouseEvent>
#include <QImage>
#include <QPaintEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int sizeX;
    int sizeY;
    int currentFrame;
    bool toggleEraser;
    bool togglePlayback;
    int redValue;
    int greenValue;
    int blueValue;
    int fps;
    double opacityValue;
    int brushValue;
    QImage image;
    QVector<Frame> frameArray;
    QVector<FrameButton> frameButtonArray;


     void mousePressEvent(QMouseEvent *event);

public slots:
     void update();
     void showAboutMessage();

private slots:
    void on_redSpinBox_valueChanged();
    void on_greenSpinBox_valueChanged();
    void on_blueSpinBox_valueChanged();
    void on_opacitySpinBox_valueChanged();
    void on_brushSizeSpinBox_valueChanged();
    void on_fpsSpinBox_valueChanged();
    void on_EraserButton_clicked();
    void on_addFrameButton_clicked();
    void on_lightMode_triggered();
    void on_darkMode_triggered();
    void saveProject();
    void loadProject();
    void newProject();
    void on_deleteFrameButton_clicked();
    void on_frameSpinBox_valueChanged(int);
    void on_PlaybackButton_clicked();

signals:
    void signalUpdate();

private:
    Ui::MainWindow *ui;
    void updateColorPreviewFrame();
    void changeFrame(int);
    void lightMode();
    void darkMode();
    void spritePlayback();
};

#endif // MAINWINDOW_H
