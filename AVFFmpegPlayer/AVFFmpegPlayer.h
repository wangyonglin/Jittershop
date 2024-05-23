#ifndef AVFFMPEGPLAYER_H
#define AVFFMPEGPLAYER_H


#include "AudioDecoder.h"
#include "AudioRender.h"

#include "VideoRender.h"
#include <QWidget>
#include <QObject>
#include <QString>
#include "Threader.h"

#include "AVDemuxer.h"
#include "VideoDecoder.h"
class AVFFmpegPlayer : public QWidget
{
    Q_OBJECT
public:

    explicit AVFFmpegPlayer(QWidget *parent = nullptr);
    ~AVFFmpegPlayer();

    virtual void resizeEvent(QResizeEvent *event);
    void play(const QString &url);
    void free();
private:
    AVDemuxer *av_demux;
    AudioDecoder *audio_dec;
    VideoDecoder *video_dec;
    AVController * controller;

signals:
    void start(Threader::Priority pri = Threader::InheritPriority);
public slots:

};

#endif // AVFFMPEGPLAYER_H
