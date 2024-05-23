#include "AudioDecodeThreader.h"
#include <memory>
#include <iostream>


AudioDecodeThreader::AudioDecodeThreader(QObject *parent)
    : AVThreader(parent),
    frame_queue(new AVFrameQueue(this))
{


}

AudioDecodeThreader::~AudioDecodeThreader()
{

}


void AudioDecodeThreader::loopRunnable()
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
                QByteArray bytes= av_resample.BuiledConvert(frame);
                audio_render.WriteOutput(bytes);
                av_frame_free(&frame);
            }

        }
    }
}


void AudioDecodeThreader::loadParameters(AVDemuxThreader *demuxer)
{

    this->demuxer=demuxer;

}

void AudioDecodeThreader::start(Priority pri)
{
    frameFinished=false;
    decode_thd.loadParameters(demuxer->audio_codecpar,demuxer->audio_pkt_queue,frame_queue);
    decode_thd.start();
    av_resample.InitAVResample(decode_thd.getCodecContext(),AV_CH_LAYOUT_STEREO,44100,AV_SAMPLE_FMT_S16);
    int data_size = av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
    audio_render.InitFormat(0,44100,data_size*8,2);
    AVThreader::start(pri);
}

void AudioDecodeThreader::stop()
{
    qDebug() << "AudioDecodeThreader is about to be deleted";
    frameFinished=true;
    decode_thd.stop();
    AVThreader::stop();
    decode_thd.clear();
    av_resample.FreeAVResample();
    audio_render.FreeFormat();
}

void AudioDecodeThreader::pause()
{
    decode_thd.pause();
    AVThreader::pause();

}

void AudioDecodeThreader::resume()
{
    decode_thd.resume();
    AVThreader::resume();
}



