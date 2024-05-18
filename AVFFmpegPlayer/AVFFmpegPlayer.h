#ifndef AVFFMPEGPLAYER_H
#define AVFFMPEGPLAYER_H

#include "AVDemuxThreader.h"
#include "AudioDecodeThreader.h"
#include "AudioRender.h"
#include "VideoDecodeThreader.h"
#include "VideoRender.h"

#include <QWidget>
#include <QObject>
#include <QString>

class AVFFmpegPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit AVFFmpegPlayer(QWidget *parent = nullptr);
    ~AVFFmpegPlayer();
    void play(QString url);
    void start();
    void pause();
    void resume();
    void stop();
private:
    AVDemuxThreader * av_demuxer=nullptr;
    AudioDecodeThreader * audio_decoder=nullptr;
    VideoDecodeThreader *video_decoder=nullptr;
    VideoRender *video_render=nullptr;

signals:
    void finished();
public slots:

};

#endif // AVFFMPEGPLAYER_H
