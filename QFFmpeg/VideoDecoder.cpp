#include "VideoDecoder.h"

VideoDecoder::VideoDecoder(QObject *parent)
    : AVThreader{parent},

    freme_threader(new AVFrameThreader(this))
{

}

void VideoDecoder::loadParameters(AVController *controller,
                                  AVDemuxer *demuxer,
                                  AVPacketQueue *video_pkt_queue,
                                   AVFrameQueue * video_frame_queue)
{
    this->controller=controller;
    this->demuxer=demuxer;
    this->video_pkt_queue=video_pkt_queue;
    this->video_frame_queue=video_frame_queue;

}
void VideoDecoder::start(Priority pri)
{
    frameFinished=false;
    freme_threader->loadParameters(demuxer->video_codecpar,video_pkt_queue,video_frame_queue);
    freme_threader->start();
    AVThreader::start(pri);
}

void VideoDecoder::stop()
{
    qDebug() << "VideoDecoder is about to be deleted";
    frameFinished=true;
    freme_threader->stop();
    video_frame_queue->clear();
    AVThreader::stop();
}

void VideoDecoder::pause()
{
    AVThreader::pause();
}

void VideoDecoder::resume()
{
    AVThreader::resume();
}

void VideoDecoder::loopRunnable()
{

    if(state()==Running && !frameFinished){

        int64_t systime_ms=   controller->GetSysClockMs();

        if (video_stream_time >systime_ms)
        {
           // QThread::msleep(5);
            return;
        }
        if(!video_frame_queue->isEmpty()){
            AVFrame* frame= video_frame_queue->dequeue();
            if(!frame) return;
            frame->pts= (frame->pts!=AV_NOPTS_VALUE)?frame->pts:0;
            video_stream_time = (frame->pts - demuxer->video_pts_begin) * av_q2d(demuxer->video_pts_base) * 1000;
            QImage image = QFFmpeg::AVFrame2RGBA8888(frame);
            emit updateRender(image);
            av_frame_free(&frame);
        }
    }
}

