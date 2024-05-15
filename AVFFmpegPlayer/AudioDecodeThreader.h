#ifndef AUDIODECODETHREADER_H
#define AUDIODECODETHREADER_H
#include <QPointer>
#include "QFFmpeg.h"
#include "AVThreader.h"
#include "AVDemuxThreader.h"
#include "AudioOutput.h"
#include "AudioRender.h"
#include "AVPacketQueue.h"
#include "AVFrameQueue.h"
#include "AVDecodeThreader.h"


class AudioDecodeThreader : public AVThreader
{
    Q_OBJECT
public:
    explicit AudioDecodeThreader(QObject *parent = nullptr);
    ~AudioDecodeThreader();
    bool initDemuxer(AVCodecParameters *codecpar);

    void loadParameters(AVDemuxThreader *demuxer,AudioRender *render);

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

    int ResampleAudio(AVFrame* frame);
    AudioRender * audio_render=nullptr;

public slots:
signals:
    void updateRender(QByteArray bytes);
};
#endif // AUDIODECODETHREADER_H
