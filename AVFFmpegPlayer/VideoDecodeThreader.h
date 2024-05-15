#ifndef VIDEODECODETHREADER_H
#define VIDEODECODETHREADER_H
#include "QFFmpeg.h"
#include "AVThreader.h"
#include "JitterBuffer.h"
#include "AVDemuxThreader.h"
#include "AudioOutput.h"
#include "VideoRender.h"
#include <QObject>
#include "AVFrameQueue.h"
#include "AVDecodeThreader.h"

class VideoDecodeThreader : public AVThreader
{
    Q_OBJECT
public:
    explicit VideoDecodeThreader(QObject *parent = nullptr);
    ~VideoDecodeThreader();
    void loadParameters(AVDemuxThreader *demuxer,
                        VideoRender *video_Render);
    virtual void start(Priority pri = InheritPriority);
    virtual void stop();
    virtual void pause();
    virtual void resume();
private:
    virtual void loopRunnable();
    bool frameFinished= false;
    int video_stream_time=0;
    AVDemuxThreader *demuxer=nullptr;
    VideoRender *render=nullptr;

    AVDecodeThreader decode_thd;
    AVFrameQueue *frame_queue=nullptr;

public slots:
signals:
    void updateRender(QImage image);
};

#endif // VIDEODECODETHREADER_H
