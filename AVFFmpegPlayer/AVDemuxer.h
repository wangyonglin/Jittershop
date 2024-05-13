#ifndef AVDEMUXER_H
#define AVDEMUXER_H
#include "AVController.h"
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


class AVDemuxer : public AVThreader
{
    Q_OBJECT
public:
    explicit AVDemuxer(QObject *parent = nullptr);
    ~AVDemuxer();
    void loadParameters(AVPacketQueue * audio_pkt_queue,AVPacketQueue * video_pkt_queue);
    bool buildDemuxer(QString url);
    virtual void start(Priority pri = InheritPriority);
    virtual void stop();
    virtual void clear();

private:
    virtual void loopRunnable();
    bool frameFinished =false;
    QString url;
    AVFormatContext * ifmt_ctx=nullptr;
    char * strurl=nullptr;
    int64_t audio_frame_dur=0;
    int64_t video_frame_dur=0;
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

#endif // AVDEMUXER_H
