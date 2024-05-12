#ifndef AVFRAMETHREADER_H
#define AVFRAMETHREADER_H

#include "QFFmpeg.h"
#include "AVThreader.h"
#include <QObject>

class AVFrameThreader : public AVThreader
{
    Q_OBJECT
public:
    explicit AVFrameThreader(QObject *parent = nullptr);
    void loadParameters(AVCodecParameters *codecpar,AVPacketQueue * pkt_queue,AVFrameQueue *frame_queue);
    virtual void start(Priority pri= InheritPriority);
    virtual void stop();
    AVCodecContext * getCodecContext();
public:

private:
    AVCodecContext *codec_ctx=nullptr;
    AVPacketQueue * pkt_queue=nullptr;
    AVFrameQueue *frame_queue=nullptr;
    AVCodecParameters *codecpar=nullptr;
    bool frameFinished= false;
    virtual void loopRunnable();
signals:
};

#endif // AVFRAMETHREADER_H
