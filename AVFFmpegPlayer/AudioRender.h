#ifndef AUDIORENDER_H

#define AUDIORENDER_H

extern "C" {
#include <libavutil/pixfmt.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
#include <libavutil/time.h>
#include <libswresample/swresample.h>
}

#include "AudioOutput.h"
#include <QApplication>
#include <QAudioOutput>
#include <QIODevice>
#include <QDebug>
#include <QObject>
#include <QByteArray>

class AudioRender : public AudioOutput
{
    Q_OBJECT
public:
    explicit AudioRender(QObject *parent = nullptr);
    ~AudioRender();
    int InitSwrResample(int64_t src_ch_layout,
                        int64_t dst_ch_layout,
                        int src_rate,
                        int dst_rate,
                        AVSampleFormat src_sample_fmt,
                        AVSampleFormat dst_sample_fmt,
                        int src_nb_samples);

    int WriteInput(AVFrame *frame);
    int SwrConvert();
    void FreeSwrResample();
    bool initSuccessful();

    int InitSwrResample(AVCodecContext *dec_ctx,
                        int64_t dst_ch_layout,
                        int dst_rate,
                        AVSampleFormat dst_sample_fmt);
private:
    struct SwrContext* swr_ctx;
    uint8_t** src_data_;
    uint8_t** dst_data_;
    int src_nb_channels, dst_nb_channels;
    int src_linesize, dst_linesize;
    int src_nb_samples_, dst_nb_samples_;
    enum AVSampleFormat dst_sample_fmt_;
    enum AVSampleFormat src_sample_fmt_;
};

#endif // AUDIORENDER_H
