#ifndef PUBLICRENDER_H
#define PUBLICRENDER_H

#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QFile>

#include <QAudio>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>
#include <QAudioFormat>
#include <QAudioInput>
#include <QBuffer>
#include <QByteArray>
#include <QIODevice>
#include <QVector>
#include <QDataStream>
#include <QObject>

#include "PublicThreader.h"

class PublicRender : public QObject
{
    Q_OBJECT
public:
    explicit PublicRender(QObject *parent = nullptr);
    QByteArray ReadAll();
    void WritePlay(QByteArray &bytes);
    bool InitSpeaker();
    void CloseSpeaker();
    bool InitMicrophone();
    void CloseMicrophone();
private: /*   QT audio play  */
    QAudioFormat qAudioFormat;
    QAudioOutput *out;
    QIODevice *audioIO;
    QTimer *audioPlayTimer;
    QThread *timerTHread;
  //  bool initQtAudioForPlay();
private: /*   QT audio capture  */
    QAudioFormat format;
    QAudioInput* audioInput;
    QIODevice *qIODevice;
    QFile file; 			// 可以输出到文件，咱不用，直接播

private: /*   QT audio capture  */


public slots:
    void onReadyRead(); /*采集并填充*/
};

#endif // PUBLICRENDER_H
