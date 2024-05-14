#include "AVFFmpegPlayer.h"


AVFFmpegPlayer::AVFFmpegPlayer(QWidget *parent)
    :QWidget{parent},
    demuxer(new AVDemuxer(this)),
    audioDecoder(new AudioDecoder(this)),
    audio_render(new AudioRender(this)),
    videoDecoder(new VideoDecoder(this)),
    videoRender(new VideoRender(this))

{
    resize(800,600);

    audioDecoder->loadParameters(demuxer,audio_render);
    videoDecoder->loadParameters(demuxer,videoRender);
    videoRender->resize(size());

}

AVFFmpegPlayer::~AVFFmpegPlayer()
{
    qDebug() << "AVFFmpegPlayer::~AVFFmpegPlayer()";
}

void AVFFmpegPlayer::play(QString url)
{
    qDebug() << "AVFFmpegPlayer::play(QString url)";
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


