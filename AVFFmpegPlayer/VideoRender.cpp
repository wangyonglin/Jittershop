#include "VideoRender.h"

VideoRender::VideoRender(QWidget *parent)
    : QWidget{parent}
{

}

VideoRender::~VideoRender()
{

}




void VideoRender::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawLine(10, 10, 100, 100); // 画线
    painter.drawRect(10, 10, 100, 100); // 画矩形
    painter.setFont(QFont("Arial",30)); // 设置字体
    painter.drawText(10, 10, "Hello"); // 指定位置画文本
    painter.drawText(rect(), Qt::AlignCenter, "Hello"); // 窗口中间画文本
    painter.drawImage(rect(), iBuffer);
}

void VideoRender::updateRender(QImage image)
{
    iBuffer=image;
    update();
}
