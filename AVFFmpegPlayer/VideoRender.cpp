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
    painter.drawImage(rect(), iBuffer);
}


void VideoRender::updateRender(QImage image)
{
    iBuffer=image;
    update();
}
