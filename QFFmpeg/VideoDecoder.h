#ifndef VIDEODECODER_H
#define VIDEODECODER_H
#include "QFFmpeg.h"
#include "AVThreader.h"
#include "JitterBuffer.h"
#include "AVDemuxer.h"
#include "AVController.h"
#include "VideoRender.h"
#include <QObject>
#include "AVFrameQueue.h"
#include "AVDecodeThreader.h"

class VideoDecoder : public AVThreader
{
    Q_OBJECT
public:
    explicit VideoDecoder(QObject *parent = nullptr);
    void loadParameters(AVController *controller,
                        AVDemuxer *demuxer);
    virtual void start(Priority pri = InheritPriority);
    virtual void stop();
    virtual void pause();
    virtual void resume();
private:
    virtual void loopRunnable();
    bool frameFinished= false;
    int video_stream_time=0;
    AVController * controller=nullptr;
    AVDemuxer *demuxer=nullptr;

    AVDecodeThreader decode_thd;
    AVFrameQueue *frame_queue=nullptr;

public slots:
signals:
    void updateRender(QImage image);
};

#endif // VIDEODECODER_H
