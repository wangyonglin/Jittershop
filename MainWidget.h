#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QApplication>
#include "AVFFmpegPlayer/AVFFmpegPlayer.h"
#include "QEngine/QVoiceAssistant.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void keyPressEvent(QKeyEvent *event);
    virtual void closeEvent(QCloseEvent *event) override;
private:
    AVFFmpegPlayer * player=nullptr;
    QVoiceAssistant *voice=nullptr;
};
#endif // MAINWIDGET_H
