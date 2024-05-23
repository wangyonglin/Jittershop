#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include <QObject>
#include "Threader.h"
#include "VideoRender.h"
#include "AVController.h"

class VideoDecoder : public Threader
{
    Q_OBJECT
public:
    explicit VideoDecoder(QWidget *parent = nullptr);
    void resize(const QSize &size);
    void resize(int w, int h);
    bool frameFinished= true;
private:
    VideoRender * render;
    virtual void loopRunnable();
    AVController * controller;
    void BuildDecoder(AVCodecContext *codec_ctx, AVPacketQueue *pkt_queue, AVFrameQueue *frame_queue);
    QImage AVFrame2RGBA8888(AVFrame *frame);
public slots:
    virtual void start(Priority pri = InheritPriority);
    virtual void stop();
    virtual void pause();
    virtual void resume();
    void freeParameters( AVController * controller);
    AVController *initParameters( AVController * controller);
};

#endif // VIDEODECODER_H
