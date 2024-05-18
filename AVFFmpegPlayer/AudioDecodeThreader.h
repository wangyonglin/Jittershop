#ifndef AUDIODECODETHREADER_H
#define AUDIODECODETHREADER_H
#include <QPointer>
#include "QFFmpeg.h"
#include "AVThreader.h"
#include "AVDemuxThreader.h"
#include "AudioRender.h"
#include "AVPacketQueue.h"
#include "AVFrameQueue.h"
#include "AVDecodeThreader.h"
#include "AVResample.h"

class AudioDecodeThreader : public AVThreader
{
    Q_OBJECT
public:
    explicit AudioDecodeThreader(QObject *parent = nullptr);
    ~AudioDecodeThreader();
    bool initDemuxer(AVCodecParameters *codecpar);

    void loadParameters(AVDemuxThreader *demuxer);

    virtual void start(Priority pri = InheritPriority);
    virtual void stop();
    virtual void pause();
    virtual void resume();
private:
    virtual void loopRunnable();
    AVFrameQueue *frame_queue=nullptr;
    AVDecodeThreader decode_thd;
    bool frameFinished= false;

    int audio_stream_time =0;
    int curr_playing_ms = 0;
    AVDemuxThreader * demuxer=nullptr;
    AVResample av_resample;
    AudioRender audio_render;
public slots:
signals:
};
#endif // AUDIODECODETHREADER_H
