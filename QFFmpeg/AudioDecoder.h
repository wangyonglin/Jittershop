#ifndef AUDIODECODER_H
#define AUDIODECODER_H
#include <QPointer>
#include "QFFmpeg.h"
#include "AVThreader.h"
#include "AVDemuxer.h"
#include "AVController.h"
#include "AudioRender.h"
#include "AVPacketQueue.h"
#include "AVFrameQueue.h"
#include "AVFrameThreader.h"


class AudioDecoder : public AVThreader
{
    Q_OBJECT
public:
    explicit AudioDecoder(QObject *parent = nullptr);

    bool initDemuxer(AVCodecParameters *codecpar);

    void loadParameters(AVController *controller,
                        AVDemuxer *demuxer,
                        AVPacketQueue *audio_pkt_queue,
                        AVFrameQueue * audio_frame_queue);

    virtual void start(Priority pri = InheritPriority);
    virtual void stop();
    virtual void pause();
    virtual void resume();
private:
    virtual void loopRunnable();
    bool frameFinished= false;
    AVController * controller=nullptr;
    int audio_stream_time =0;
    int curr_playing_ms = 0;
    AVDemuxer * demuxer=nullptr;

    AVFrameThreader *freme_threader=nullptr;
    AVPacketQueue *audio_pkt_queue=nullptr;
    AVFrameQueue * audio_frame_queue=nullptr;
    int ResampleAudio(AVFrame* frame);
    AudioRender * audio_render=nullptr;

public slots:
signals:
    void updateRender(QByteArray bytes);
};
#endif // AUDIODECODER_H
