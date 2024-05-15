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
                ResampleAudio(frame);
                av_frame_free(&frame);
            }

        }
    }
}

int AudioDecodeThreader::ResampleAudio(AVFrame *frame)
{
    int res=-1;
    audio_render->WriteInput(frame);
    res=   audio_render->SwrConvert();

    return res;
}






void AudioDecodeThreader::loadParameters(AVDemuxThreader *demuxer,AudioRender *render)
{

    this->demuxer=demuxer;
    this->audio_render=render;

}

void AudioDecodeThreader::start(Priority pri)
{
    frameFinished=false;
    decode_thd.loadParameters(demuxer->audio_codecpar,demuxer->audio_pkt_queue,frame_queue);
    decode_thd.start();
    audio_render->InitSwrResample(decode_thd.getCodecContext(),AV_CH_LAYOUT_STEREO,44100,AV_SAMPLE_FMT_S16);
    AVThreader::start(pri);
}

void AudioDecodeThreader::stop()
{
    qDebug() << "AudioDecodeThreader is about to be deleted";
    frameFinished=true;
    decode_thd.stop();
    AVThreader::stop();
    decode_thd.clear();
    audio_render->Close();
    delete audio_render;

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



