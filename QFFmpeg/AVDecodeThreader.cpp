#include "AVFrameThreader.h"

AVFrameThreader::AVFrameThreader(QObject *parent)
    : AVThreader{parent}
{}

void AVFrameThreader::loadParameters(AVCodecParameters *codecpar,AVPacketQueue * pkt_queue,AVFrameQueue *frame_queue)
{
    this->codecpar=codecpar;
    this->pkt_queue=pkt_queue;
    this->frame_queue=frame_queue;
}



void AVFrameThreader::start(Priority pri)
{
    frameFinished=false;
    codec_ctx=QFFmpeg::BuildDecoder(codecpar);
    AVThreader::start(pri);
}

void AVFrameThreader::stop()
{
    frameFinished=true;
    AVThreader::stop();
    QFFmpeg::FreeDecoder(codec_ctx);
    codec_ctx=nullptr;
}

AVCodecContext *AVFrameThreader::getCodecContext()
{
    return codec_ctx;
}

void AVFrameThreader::loopRunnable()
{
    if(state()==Running && !frameFinished){
        if(!codec_ctx)return;
        if(!pkt_queue)return;
        if(!frame_queue)return;
        if(pkt_queue->isEmpty())return;
        if(frame_queue->size()<=10){
          QFFmpeg::ReceiveDecoder(codec_ctx,pkt_queue,frame_queue);
        }

    }

}
