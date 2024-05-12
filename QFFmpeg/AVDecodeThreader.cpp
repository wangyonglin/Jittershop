#include "AVDecodeThreader.h"

AVDecodeThreader::AVDecodeThreader(QObject *parent)
    : AVThreader{parent}
{}

void AVDecodeThreader::loadParameters(AVCodecParameters *codecpar,AVPacketQueue * pkt_queue,AVFrameQueue *frame_queue)
{
    this->codecpar=codecpar;
    this->pkt_queue=pkt_queue;
    this->frame_queue=frame_queue;
}



void AVDecodeThreader::start(Priority pri)
{
    frameFinished=false;
    codec_ctx=OpenDecoder(codecpar);
    AVThreader::start(pri);
}

void AVDecodeThreader::stop()
{
    frameFinished=true;
    AVThreader::stop();
    CloseDecoder(codec_ctx);
    codec_ctx=nullptr;
}

void AVDecodeThreader::clear()
{
    frame_queue->clear();
}

AVCodecContext *AVDecodeThreader::getCodecContext()
{
    return codec_ctx;
}

void AVDecodeThreader::loopRunnable()
{
    if(state()==Running && !frameFinished){
        if(!codec_ctx)return;
        if(!pkt_queue)return;
        if(!frame_queue)return;
        if(pkt_queue->isEmpty())return;
        if(frame_queue->size()<=10){
          BuildDecoder(codec_ctx,pkt_queue,frame_queue);
        }

    }

}
AVCodecContext * AVDecodeThreader::OpenDecoder(AVCodecParameters *codecpar)
{
    AVCodecContext *codec_ctx = avcodec_alloc_context3(NULL);
    if(!codec_ctx)return nullptr;
    int read_ret = avcodec_parameters_to_context(codec_ctx, codecpar);
    if(read_ret < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, read_ret);
        qDebug() << "avcodec_parameters_to_context failed" << errmsg;
        avcodec_free_context(&codec_ctx);
        return nullptr;
    }
    // h264
    // h264_qsv  AV_CODEC_ID_H264
    //    avcodec_find_decoder_by_name()

    //    if(AV_CODEC_ID_H264 == codec_ctx_->codec_id)
    //        codec = avcodec_find_decoder_by_name("h264_qsv");
    //    else
    AVCodec * codec = avcodec_find_decoder(codec_ctx->codec_id); //作业： 硬件解码
    if(!codec) {
        qDebug() << "avcodec_find_decoder failed";
        avcodec_free_context(&codec_ctx);
        return nullptr;
    }

    read_ret = avcodec_open2(codec_ctx, codec, NULL);
    if(read_ret < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, read_ret);
        qDebug() << "avcodec_open2 failed" << errmsg;
        avcodec_free_context(&codec_ctx);
        return nullptr;
    }
    return codec_ctx;
}

void AVDecodeThreader::CloseDecoder(AVCodecContext *codec_ctx)
{
    if(codec_ctx){
        avcodec_free_context(&codec_ctx);
        codec_ctx=nullptr;
    }
}

void AVDecodeThreader::BuildDecoder(AVCodecContext *codec_ctx,AVPacketQueue *pkt_queue,AVFrameQueue *frame_queue)
{
    if(!codec_ctx)return;
    if(pkt_queue->isEmpty())return;
    AVPacket * pkt= pkt_queue->dequeue();
    if(!pkt)return;
    int ret = avcodec_send_packet(codec_ctx, pkt);
    av_packet_free(&pkt);
    if (ret < 0)
    {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avcodec_send_packet failed" << errmsg;
        return ;
    }
    AVFrame* frame = av_frame_alloc();
    if(!frame)return;
    ret = avcodec_receive_frame(codec_ctx, frame);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
        return ;
    }else if (ret < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avcodec_receive_frame failed" << errmsg;
        return ;
    }
    frame_queue->enqueue(frame);
    av_frame_free(&frame);
}
