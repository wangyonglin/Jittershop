#include "AVFFmpegPlayer.h"


AVFFmpegPlayer::AVFFmpegPlayer(QWidget *parent)
    :AVController{parent},
    demuxer(new AVDemuxer(this)),
    audioDecoder(new AudioDecoder(this)),
    audio_render(new AudioRender(this)),
    videoDecoder(new VideoDecoder(this)),
    videoRender(new VideoRender(this)),
    audio_pkt_queue(new AVPacketQueue(this)),
    video_pkt_queue(new AVPacketQueue(this)),
    audio_frame_queue(new AVFrameQueue(this)),
    video_frame_queue(new AVFrameQueue(this))
{

    resize(800,600);
    StartSysClockMs();
    connect(videoDecoder,&VideoDecoder::updateRender,videoRender,&VideoRender::updateRender);
    demuxer->loadParameters(audio_pkt_queue,video_pkt_queue);
    audioDecoder->loadParameters(this,demuxer,audio_pkt_queue,audio_frame_queue);
    videoDecoder->loadParameters(this,demuxer,video_pkt_queue,video_frame_queue);
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


