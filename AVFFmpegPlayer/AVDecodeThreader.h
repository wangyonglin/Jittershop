#ifndef AVDECODETHREADER_H
#define AVDECODETHREADER_H

#include "QFFmpeg.h"
#include "AVThreader.h"
#include <QObject>

class AVDecodeThreader : public AVThreader
{
    Q_OBJECT
public:
    explicit AVDecodeThreader(QObject *parent = nullptr);
    void loadParameters(AVCodecParameters *codecpar,AVPacketQueue * pkt_queue,AVFrameQueue *frame_queue);
    virtual void start(Priority pri= InheritPriority);
    virtual void stop();
    virtual void clear();
    AVCodecContext * getCodecContext();
public:

private:
    AVCodecContext *codec_ctx=nullptr;
    AVPacketQueue * pkt_queue=nullptr;
    AVFrameQueue *frame_queue=nullptr;
    AVCodecParameters *codecpar=nullptr;
    bool frameFinished= false;
    virtual void loopRunnable();
    AVCodecContext *OpenDecoder(AVCodecParameters *codecpar);
    void CloseDecoder(AVCodecContext *codec_ctx);
    void BuildDecoder(AVCodecContext *codec_ctx, AVPacketQueue *pkt_queue, AVFrameQueue *frame_queue);

signals:
    void finished();
};

#endif // AVDECODETHREADER_H
