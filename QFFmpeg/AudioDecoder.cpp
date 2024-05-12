#include "AudioDecoder.h"
#include <memory>
#include <iostream>


AudioDecoder::AudioDecoder(QObject *parent)
    : AVThreader{parent},
    freme_threader(new AVFrameThreader(this))
{


}


void AudioDecoder::loopRunnable()
{

    if(!demuxer)return;
    if( state()==Running && !frameFinished){

        int64_t systime_ms=   controller->GetSysClockMs();
        if (audio_stream_time >systime_ms)
        {
            return;
        }
        if(!audio_frame_queue->isEmpty()){

            AVFrame* frame= audio_frame_queue->dequeue();
            if(!frame) return;
            frame->pts= (frame->pts!=AV_NOPTS_VALUE)?frame->pts:0;
            audio_stream_time = (frame->pts - demuxer->audio_pts_begin) * av_q2d(demuxer->audio_pts_base) * 1000;
            ResampleAudio(frame);
            av_frame_free(&frame);
        }
    }
}

int AudioDecoder::ResampleAudio(AVFrame *frame)
{

    // 把AVFrame里面的数据拷贝到，预备的src_data里面
    int res=-1;
    if (!audio_render)
    {
        //  qDebug() << "AVFrame里面的数据拷贝到，预备的src_data里面";
      // swrResample = new SwrResample();
    audio_render= new AudioRender(this,controller);

        //创建重采样信息
        int src_ch_layout = freme_threader->getCodecContext()->channel_layout;
        int src_rate = freme_threader->getCodecContext()->sample_rate;
        enum AVSampleFormat src_sample_fmt = freme_threader->getCodecContext()->sample_fmt;


        //aac编码一般是这个,实际这个值只能从解码后的数据里面获取，所有这个初始化过程可以放在解码出第一帧的时候
      //  int src_nb_samples = frame->nb_samples;

        audio_render->InitSwrResample(src_ch_layout, AV_CH_LAYOUT_STEREO,
                          src_rate, 44100,
                          src_sample_fmt, AV_SAMPLE_FMT_S16,
                          frame->nb_samples);


    }

    audio_render->WriteInput(frame);
    res=   audio_render->SwrConvert();

    return res;
}






void AudioDecoder::loadParameters(AVController *controller,
                                  AVDemuxer *demuxer,
                                  AVPacketQueue *audio_pkt_queue,
                                  AVFrameQueue * audio_frame_queue
                                  )
{
    this->controller=controller;
    this->demuxer=demuxer;
    this->audio_pkt_queue=audio_pkt_queue;
    this->audio_frame_queue=audio_frame_queue;


}

void AudioDecoder::start(Priority pri)
{
    frameFinished=false;

    freme_threader->loadParameters(demuxer->audio_codecpar,audio_pkt_queue,audio_frame_queue);

    freme_threader->start();
    AVThreader::start(pri);
}

void AudioDecoder::stop()
{
    qDebug() << "AudioDecoder is about to be deleted";
    frameFinished=true;
    delete audio_render;
    audio_frame_queue->clear();
    freme_threader->stop();
    AVThreader::stop();

}

void AudioDecoder::pause()
{
    AVThreader::pause();
}

void AudioDecoder::resume()
{
    AVThreader::resume();
}



