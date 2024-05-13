#include "VideoDecoder.h"

VideoDecoder::VideoDecoder(QObject *parent)
    : AVThreader{parent},
    frame_queue(new AVFrameQueue(this))
{

}

void VideoDecoder::loadParameters(AVController *controller,
                                  AVDemuxer *demuxer)
{
    this->controller=controller;
    this->demuxer=demuxer;


}
void VideoDecoder::start(Priority pri)
{
    frameFinished=false;
    decode_thd.loadParameters(demuxer->video_codecpar,demuxer->video_pkt_queue,frame_queue);
    decode_thd.clear();
    decode_thd.start();
    AVThreader::start(pri);
}

void VideoDecoder::stop()
{
    qDebug() << "VideoDecoder is about to be deleted";
    frameFinished=true;
    decode_thd.stop();
    decode_thd.clear();
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
        if (video_stream_time >controller->GetSysClockMs())
        {
            return;
        }
        if(!frame_queue->isEmpty()){
            AVFrame * frame= frame_queue->dequeue();
            if(frame){
                frame->pts= (frame->pts!=AV_NOPTS_VALUE)?frame->pts:0;
                video_stream_time = (frame->pts - demuxer->video_pts_begin) * av_q2d(demuxer->video_pts_base) * 1000;
                QImage image = QFFmpeg::AVFrame2RGBA8888(frame);
                controller->ImageUpdate(image);
                // emit updateRender(image);
                av_frame_free(&frame);
            }
        }


    }
}

