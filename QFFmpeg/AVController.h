#ifndef AVCONTROLLER_H
#define AVCONTROLLER_H


#include <QApplication>
#include <QAudioOutput>
#include <QIODevice>
#include <QDebug>
#include <QObject>
#include <QWidget>
#include <chrono>

class AVController : public QWidget
{
    Q_OBJECT
public:
    explicit AVController(QWidget *parent = nullptr);
    ~AVController();
    void InitIODevice(int dst_nb_samples, int rate, int sample_size, int nch);
    void WriteIODevice(const char* data, qint64 len);
    void FreeIODevice();
    void StartSysClockMs();
    int64_t now_ms();
    int64_t GetSysClockMs();
    int64_t audio_frame_dur = 0; //一帧音频需要经过的时间
    int64_t video_frame_dur= 0; // 一帧视频需要经过的时间
private:
    QIODevice* outputDevice;
    QAudioOutput* audioOutput;

    int64_t player_start_time_ms = 0;

signals:
};

#endif // AVCONTROLLER_H