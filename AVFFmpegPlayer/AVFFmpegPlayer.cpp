#include "AVFFmpegPlayer.h"


AVFFmpegPlayer::AVFFmpegPlayer(QWidget *parent)
    :AVController{parent},
    demuxer(new AVDemuxer(this)),
    audioDecoder(new AudioDecoder(this)),
    audio_render(new AudioRender(this)),
    videoDecoder(new VideoDecoder(this))
    //videoRender(new VideoRender(this)),

{
    resize(800,600);

    audioDecoder->loadParameters(this,demuxer);
    videoDecoder->loadParameters(this,demuxer);
  // videoRender->resize(size());

}

AVFFmpegPlayer::~AVFFmpegPlayer()
{
    qDebug() << "AVFFmpegPlayer::~AVFFmpegPlayer()";
}

void AVFFmpegPlayer::play(QString url)
{

    qDebug() << "AVFFmpegPlayer::play(QString url)";
    StartSysClockMs();
    demuxer->buildDemuxer(url);
    demuxer->start();
    audioDecoder->start();
    videoDecoder->start();

}

void AVFFmpegPlayer::start()
{
    demuxer->start();
    audioDecoder->start();
    videoDecoder->start();
}

void AVFFmpegPlayer::pause()
{
    audioDecoder->pause();
    videoDecoder->pause();
}
void AVFFmpegPlayer::resume()
{
    audioDecoder->resume();
    videoDecoder->resume();
}


void AVFFmpegPlayer::stop()
{
    qDebug() << "AVFFmpegPlayer::stop()";
    demuxer->stop();
    audioDecoder->stop();
    videoDecoder->stop();

}


