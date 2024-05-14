#include "AudioDecoder.h"
#include <memory>
#include <iostream>


AudioDecoder::AudioDecoder(QObject *parent)
    : AVThreader(parent),
    frame_queue(new AVFrameQueue(this))
{


}


void AudioDecoder::loopRunnable()
{
    if(!demuxer)return;
    if(state()==Running && !frameFinished){
        if (audio_stream_time > demuxer->getCurrentTimer())
        {
            return;
        }
        if(!frame_queue->isEmpty()){
            AVFrame * frame= frame_queue->dequeue();
            if(frame){
                frame->pts= (frame->pts!=AV_NOPTS_VALUE)?frame->pts:0;
                audio_stream_time = (frame->pts - demuxer->audio_pts_begin) * av_q2d(demuxer->audio_pts_base) * 1000;
                ResampleAudio(frame);
                av_frame_free(&frame);
            }

        }
    }
}

int AudioDecoder::ResampleAudio(AVFrame *frame)
{

    // 把AVFrame里面的数据拷贝到，预备的src_data里面
    int res=-1;
    // if (!audio_render->initSuccessful())
    // {
    //     //创建重采样信息
    //     int src_ch_layout = decode_thd.getCodecContext()->channel_layout;
    //     int src_rate = decode_thd.getCodecContext()->sample_rate;
    //     enum AVSampleFormat src_sample_fmt = decode_thd.getCodecContext()->sample_fmt;


    //     //aac编码一般是这个,实际这个值只能从解码后的数据里面获取，所有这个初始化过程可以放在解码出第一帧的时候
    //     //  int src_nb_samples = frame->nb_samples;

    //     audio_render->InitSwrResample(src_ch_layout, AV_CH_LAYOUT_STEREO,
    //                                   src_rate, 44100,
    //                                   src_sample_fmt, AV_SAMPLE_FMT_S16,
    //                                   decode_thd.getCodecContext()->frame_size);


    // }

    audio_render->WriteInput(frame);
    res=   audio_render->SwrConvert();

    return res;
}






void AudioDecoder::loadParameters(AVDemuxer *demuxer,AudioRender *render)
{

    this->demuxer=demuxer;
    this->audio_render=render;

}

void AudioDecoder::start(Priority pri)
{
    frameFinished=false;
    decode_thd.loadParameters(demuxer->audio_codecpar,demuxer->audio_pkt_queue,frame_queue);
    decode_thd.clear();
    decode_thd.start();
    audio_render->InitSwrResample(decode_thd.getCodecContext(),AV_CH_LAYOUT_STEREO,44100,AV_SAMPLE_FMT_S16);
    AVThreader::start(pri);
}

void AudioDecoder::stop()
{
    qDebug() << "AudioDecoder is about to be deleted";
    frameFinished=true;
    delete audio_render;
    decode_thd.stop();
    decode_thd.clear();
    AVThreader::stop();
    audio_render->Close();

}

void AudioDecoder::pause()
{
    AVThreader::pause();
}

void AudioDecoder::resume()
{
    AVThreader::resume();
}



