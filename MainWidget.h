#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QApplication>
#include "AVFFmpegPlayer/AVFFmpegPlayer.h"
#include "SnowboyWrapper/SnowboyWrapper.h"
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
    SnowboyWrapper *snowboy_wrapper=nullptr;
};
#endif // MAINWIDGET_H
