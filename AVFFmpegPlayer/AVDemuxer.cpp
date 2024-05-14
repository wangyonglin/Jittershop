#include "AVDemuxer.h"

AVDemuxer::AVDemuxer(QObject *parent)
    : AVThreader{parent},
    audio_pkt_queue(new AVPacketQueue(this)),
    video_pkt_queue(new AVPacketQueue(this))
{
}

AVDemuxer::~AVDemuxer()
{
    qDebug() << "~AVDemuxer()";
    delete audio_pkt_queue;
    audio_pkt_queue=nullptr;
    delete video_pkt_queue;
    video_pkt_queue=nullptr;
}

bool AVDemuxer::buildDemuxer(QString url)
{
    strurl=url.toLocal8Bit().data();
    if(ifmt_ctx)avformat_close_input(&ifmt_ctx);
    int read_ret = avformat_open_input(&ifmt_ctx, strurl, NULL, NULL);
    if(read_ret < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, read_ret);
        qDebug() << "avformat_open_input failed" << errmsg;
        return false;
    }

    read_ret = avformat_find_stream_info(ifmt_ctx, NULL);
    if(read_ret < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, read_ret);
        qDebug() << "avformat_find_stream_info failed" << errmsg;
        avformat_close_input(&ifmt_ctx);
        return false;
    }
    av_dump_format(ifmt_ctx, 0, strurl, 0);
    audio_stream_index=av_find_best_stream(ifmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    video_stream_index=av_find_best_stream(ifmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if(audio_stream_index>=0){
        audio_codecpar=ifmt_ctx->streams[audio_stream_index]->codecpar;
        audio_pts_base=ifmt_ctx->streams[audio_stream_index]->time_base;
        audio_pts_begin = ifmt_ctx->streams[audio_stream_index]->start_time;
    }
    if(video_stream_index>=0)
    {
        video_codecpar=ifmt_ctx->streams[video_stream_index]->codecpar;
        video_pts_base=ifmt_ctx->streams[video_stream_index]->time_base;
        video_pts_begin = ifmt_ctx->streams[video_stream_index]->start_time;
    }
    qInfo() << ("AVDemuxer buildDemuxer leave");

    return true;
}


void AVDemuxer::start(Priority pri)
{
    frameFinished=false;
    initStartTimer();
    AVThreader::start(pri);
}

void AVDemuxer::stop()
{
    frameFinished= true;
    AVThreader::stop();
    clearStartTimer();
}

void AVDemuxer::clear()
{
    audio_pkt_queue->clear();
    video_pkt_queue->clear();
}

void AVDemuxer::loopRunnable()
{
    if(state() ==Running && !frameFinished){
        if(audio_pkt_queue->size() > 10 || video_pkt_queue->size() >10)return;
        AVPacket* pkt = av_packet_alloc();
        av_packet_unref(pkt);
        int read_ret = av_read_frame(ifmt_ctx, pkt);
        if (read_ret < 0) {
            char errmsg[AV_ERROR_MAX_STRING_SIZE];
            av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, read_ret);
            if (read_ret == AVERROR_EOF)
            {
                qDebug() << "Reached end of file" << errmsg;
                av_packet_unref(pkt);
                frameFinished=true;
                return;
            }
            else
            {
                qDebug() << "Error while reading frames" << errmsg;
                return;
            }
        }
        if (pkt->stream_index == audio_stream_index)
        {
            audio_pkt_queue->enqueue(pkt);
            // qDebug() << "audio pkt size:" <<audio_pkt_queue->size();

        }
        else if(pkt->stream_index == video_stream_index)
        {
            video_pkt_queue->enqueue(pkt);
            //  qDebug() << "video pkt size:" <<video_pkt_queue->size();
        }
        av_packet_free(&pkt);
    }

}

void AVDemuxer::initStartTimer()
{
    if (this->player_start_time_ms == 0)
    {
        this->player_start_time_ms = now_ms();
    }
}

void AVDemuxer::clearStartTimer()
{
    this->player_start_time_ms = 0;
}



int64_t AVDemuxer::now_ms()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

int64_t AVDemuxer::getCurrentTimer()
{
    if (this->player_start_time_ms != 0)
    {
        return  now_ms() - this->player_start_time_ms;
    }
    return 0;
}
