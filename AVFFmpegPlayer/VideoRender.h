#ifndef VIDEORENDER_H
#define VIDEORENDER_H
#include <QPainter>
#include <QWidget>
#include <QImage>
#include <QPaintEvent>

class VideoRender : public QWidget
{
    Q_OBJECT
public:
    explicit VideoRender(QWidget *parent = nullptr);
    ~VideoRender();


protected:
    void paintEvent(QPaintEvent* event) override;
private:
    QImage iBuffer;
public slots:
    void updateRender(QImage image);
};

#endif // VIDEORENDER_H
