#ifndef AVDEMUXTHREADER_H
#define AVDEMUXTHREADER_H
#include "AudioRender.h"
#include "QFFmpeg.h"
#include "AVThreader.h"
#include "JitterBuffer.h"
#include <QObject>
#include <QDebug>
#include <QString>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "AVPacketQueue.h"


class AVDemuxThreader : public AVThreader
{
    Q_OBJECT
public:
    explicit AVDemuxThreader(QObject *parent = nullptr);
    ~AVDemuxThreader();
    void loadParameters(AVPacketQueue * audio_pkt_queue,AVPacketQueue * video_pkt_queue);
    bool initDemuxer(QString url);

    virtual void start(Priority pri = InheritPriority);
    virtual void stop();
    virtual void pause();
    virtual void resume();

    int64_t getCurrentTimer();
private:
    void initStartTimer();
    void clearStartTimer();
    int64_t now_ms();
    virtual void loopRunnable();
    int64_t audio_frame_dur = 0; //一帧音频需要经过的时间
    int64_t video_frame_dur= 0; // 一帧视频需要经过的时间
    int64_t player_start_time_ms = 0;
    bool frameFinished =false;
    QString strurl;
    AVFormatContext * ifmt_ctx=nullptr;
   // char * strurl=nullptr;
    int64_t expand_size=0;

public:
    AVPacketQueue * audio_pkt_queue=nullptr;
    AVPacketQueue * video_pkt_queue=nullptr;
    int audio_stream_index=-1;
    int video_stream_index=-1;
    AVCodecParameters * audio_codecpar=nullptr;
    AVCodecParameters * video_codecpar=nullptr;
    AVRational video_pts_base = {0,0};
    AVRational audio_pts_base = {0,0};
    int64_t audio_pts_begin = 0;
    int64_t video_pts_begin = 0;

signals:
    void audioBuffer(AVPacket * pkt,AVRational timebase);
    void videoBuffer(AVPacket * pkt,AVRational timebase);
public slots:

};

#endif // AVDEMUXTHREADER_H
