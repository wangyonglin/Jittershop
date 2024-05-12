#include "MainWidget.h"


MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)

{
    player=new AVFFmpegPlayer(this);

    player->play("/home/wangyonglin/视频/252977_video.mp4");
}

MainWidget::~MainWidget() {
    delete player;
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) { // 检查是否按下了Esc键

        // 弹出确认对话框
        if (QMessageBox::question(this, "退出", "确定要退出吗？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            // 如果用户选择"是"，则继续关闭窗口
            player->stop();
            QApplication::exit(); // 退出程序
            event->accept();
        } else {
            // 如果用户选择"否"，则取消关闭操作
            event->ignore();
        }
    }else if(event->key() == Qt::Key_F11){
        player->pause();
    }else if(event->key() == Qt::Key_F12){
        player->resume();
    }
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    // 弹出确认对话框
    if (QMessageBox::question(this, "退出", "确定要退出吗？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        // 如果用户选择"是"，则继续关闭窗口
        player->stop();
        QApplication::exit(); // 退出程序
        event->accept();
    } else {
        // 如果用户选择"否"，则取消关闭操作
        event->ignore();
    }
}
