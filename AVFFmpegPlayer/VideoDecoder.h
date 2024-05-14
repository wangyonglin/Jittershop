#ifndef VIDEODECODER_H
#define VIDEODECODER_H
#include "QFFmpeg.h"
#include "AVThreader.h"
#include "JitterBuffer.h"
#include "AVDemuxer.h"
#include "AVOutput.h"
#include "VideoRender.h"
#include <QObject>
#include "AVFrameQueue.h"
#include "AVDecodeThreader.h"

class VideoDecoder : public AVThreader
{
    Q_OBJECT
public:
    explicit VideoDecoder(QObject *parent = nullptr);
    void loadParameters(AVDemuxer *demuxer,
                        VideoRender *video_Render);
    virtual void start(Priority pri = InheritPriority);
    virtual void stop();
    virtual void pause();
    virtual void resume();
private:
    virtual void loopRunnable();
    bool frameFinished= false;
    int video_stream_time=0;
    AVDemuxer *demuxer=nullptr;
    VideoRender *render=nullptr;

    AVDecodeThreader decode_thd;
    AVFrameQueue *frame_queue=nullptr;

public slots:
signals:
    void updateRender(QImage image);
};

#endif // VIDEODECODER_H
