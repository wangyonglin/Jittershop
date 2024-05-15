#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H


#include <QApplication>
#include <QAudioOutput>
#include <QIODevice>
#include <QDebug>
#include <QObject>
#include <QWidget>
#include <chrono>
#include <QPaintEvent>
#include <QPainter>

class AudioOutput: public QObject
{
    Q_OBJECT
public:
    explicit AudioOutput(QObject *parent = nullptr);
    ~AudioOutput();

    void InitFormat(int dst_nb_samples, int rate, int sample_size, int nch);
    void WriteOutput(const char* data, qint64 len);
    void FreeFormat();
protected:
private:
    QIODevice* outputDevice;
    QAudioOutput* audioOutput;
    int64_t player_start_time_ms = 0;

signals:
};

#endif // AUDIOOUTPUT_H
