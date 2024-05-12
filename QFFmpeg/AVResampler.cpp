#include "AVResampler.h"

AVResampler::AVResampler(QObject *parent)
    : QObject{parent}
{}

AVResampler::~AVResampler()
{

}

struct SwrContext * AVResampler::InitFormat(AVCodecContext *dec_ctx,
                                           int dst_sample_rate,
                                           enum AVSampleFormat dst_sample_fmt,
                                           int64_t dst_ch_layout)
{
    this->__dec_ctx=dec_ctx;
    this->__dst_sample_rate=dst_sample_rate;
    this->__dst_sample_fmt=dst_sample_fmt;
    this->__dst_ch_layout=dst_ch_layout;
    int ret =0;
    /* create resampler context */
    __swr_ctx = swr_alloc();
    if (!__swr_ctx) {
        qDebug() << "Could not allocate resampler context";
        ret = AVERROR(ENOMEM);
        return nullptr;
    }
    if (__dec_ctx->channel_layout == 0)
    {
        __dec_ctx->channel_layout = av_get_default_channel_layout(__dec_ctx->channels);
    }
    /* set options */
    av_opt_set_int(__swr_ctx, "in_channel_layout",   __dec_ctx->channel_layout, 0);
    av_opt_set_int(__swr_ctx, "in_sample_rate",      __dec_ctx->sample_rate, 0);
    av_opt_set_sample_fmt(__swr_ctx, "in_sample_fmt",__dec_ctx->sample_fmt, 0);

    av_opt_set_int(__swr_ctx, "out_channel_layout",    __dst_ch_layout, 0);     //AV_CH_LAYOUT_STEREO
    av_opt_set_int(__swr_ctx, "out_sample_rate",      __dst_sample_rate, 0);   //44100
    av_opt_set_sample_fmt(__swr_ctx, "out_sample_fmt",__dst_sample_fmt, 0);    //AV_SAMPLE_FMT_S16

    /* initialize the resampling context */
    if ((ret = swr_init(__swr_ctx)) < 0) {
        qDebug() << "Failed to initialize the resampling context";
        swr_free(&__swr_ctx);
        __swr_ctx=nullptr;
        return nullptr;
    }
   // int data_size = av_get_bytes_per_sample(__dst_sample_fmt);
   // audioPlayer.SetFormat(0,__dst_sample_rate,data_size*8,__dec_ctx->channels);


    return __swr_ctx;
}

QByteArray AVResampler::BuildConvert(AVFrame *frame){
    QByteArray byteBuffer;
    if(!__swr_ctx)return byteBuffer;
    if(!frame) return byteBuffer;
    // 转码音频帧
    // 计算转码后的音频数据大小
    int dstNbSamples = av_rescale_rnd(
        swr_get_delay(__swr_ctx,__dst_sample_rate) + frame->nb_samples,
        __dst_sample_rate,__dst_sample_rate, AV_ROUND_UP);

    int dstBufferSize = av_samples_get_buffer_size(nullptr, 2, dstNbSamples,__dst_sample_fmt, 0);
    if(dstBufferSize<=0) return byteBuffer;

    QByteArray outputBuffer(dstBufferSize, 0);

    // 分配转码后的音频数据缓冲区
    uint8_t *outputBufferData = reinterpret_cast<uint8_t*>(outputBuffer.data());


    // 进行音频转码
    int numSamples = swr_convert(__swr_ctx,
                                 &outputBufferData,
                                 dstNbSamples,
                                 const_cast<const uint8_t **>(frame->data),
                                 frame->nb_samples);
    if (numSamples < 0) {
        qDebug() << "音频转码失败";
        av_freep(&outputBufferData);
    }
    else{
        byteBuffer.resize(dstBufferSize);
        return outputBuffer;
    }
    return byteBuffer;;
}

int AVResampler::BuildConvertOutput(AVFrame *frame){
    if(!__swr_ctx)return -1;
    if(!frame)return -1;

    return -1;
}

void AVResampler::FreeInitFormat(){
    if(__swr_ctx)swr_free(&__swr_ctx);
}
