#ifndef AVFFMPEGPLAYER_H
#define AVFFMPEGPLAYER_H

#include "AVDemuxer.h"
#include "AudioDecoder.h"
#include "AVController.h"
#include "AudioRender.h"
#include "VideoDecoder.h"
#include "VideoRender.h"

#include <QWidget>
#include <QObject>
#include <QString>

class AVFFmpegPlayer : public AVController
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
    AVDemuxer * demuxer=nullptr;
    AudioDecoder * audioDecoder=nullptr;
    AudioRender * audio_render=nullptr;
    VideoDecoder *videoDecoder=nullptr;
    VideoRender *videoRender=nullptr;

signals:
    void finished();
public slots:

};

#endif // AVFFMPEGPLAYER_H
