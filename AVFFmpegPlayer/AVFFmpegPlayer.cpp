#include "AVFFmpegPlayer.h"


AVFFmpegPlayer::AVFFmpegPlayer(QWidget *parent)
    :QWidget{parent},
    av_demuxer(new AVDemuxThreader(this)),
    audio_decoder(new AudioDecodeThreader(this)),
    video_decoder(new VideoDecodeThreader(this)),
    video_render(new VideoRender(this))

{
    resize(800,600);

    audio_decoder->loadParameters(av_demuxer);
    video_decoder->loadParameters(av_demuxer,video_render);
    video_render->resize(size());

}

AVFFmpegPlayer::~AVFFmpegPlayer()
{
    qDebug() << "AVFFmpegPlayer::~AVFFmpegPlayer()";
}

void AVFFmpegPlayer::play(QString url)
{
    qDebug() << "AVFFmpegPlayer::play(QString url)";
    av_demuxer->initDemuxer(url);
    av_demuxer->start();
    audio_decoder->start();
    video_decoder->start();

}

void AVFFmpegPlayer::start()
{
    av_demuxer->start();
    audio_decoder->start();
    video_decoder->start();
}

void AVFFmpegPlayer::pause()
{
    audio_decoder->pause();
    video_decoder->pause();
}
void AVFFmpegPlayer::resume()
{
    audio_decoder->resume();
    video_decoder->resume();
}


void AVFFmpegPlayer::stop()
{
    qDebug() << "AVFFmpegPlayer::stop()";

    audio_decoder->stop();
    video_decoder->stop();
     av_demuxer->stop();

}


