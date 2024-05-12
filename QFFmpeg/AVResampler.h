#ifndef AVRESAMPLER_H
#define AVRESAMPLER_H
#include "QFFmpeg.h"

#include <QObject>




class AVResampler : public QObject
{
    Q_OBJECT
public:
    explicit AVResampler(QObject *parent = nullptr);
    ~AVResampler();

    struct SwrContext * InitFormat(AVCodecContext *dec_ctx,
                                           int dst_sample_rate,
                                           enum AVSampleFormat dst_sample_fmt,
                                           int64_t dst_ch_layout);

    QByteArray BuildConvert(AVFrame *frame);
    int BuildConvertOutput(AVFrame *frame);
    void FreeInitFormat();
private:
    uint8_t** __src_data;
    uint8_t** __dst_data;
    int __src_linesize, __dst_linesize;
     int __src_nb_samples, __dst_nb_samples;
    struct SwrContext * __swr_ctx=nullptr;
    AVCodecContext *__dec_ctx=nullptr;
    int __dst_sample_rate=44100;
    enum AVSampleFormat __dst_sample_fmt;
    int64_t __dst_ch_layout;
};

#endif // AVRESAMPLER_H
