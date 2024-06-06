#include "QVoiceAssistant.h"

QVoiceAssistant::QVoiceAssistant(QObject *parent)
    : QThreader{parent},
    qMicrophone(new QMicrophone(parent)),
    qRecognizer(new QRecognizer(parent)),
    qSpeaker(new QSpeaker(parent))
{}

void QVoiceAssistant::InitVoiceAssistant()
{
    qMicrophone->InitMicrophone();
    qRecognizer->InitRecognizer(4,"greedy_search");
    qSpeaker->InitSpeaker();
}

void QVoiceAssistant::loopStart(Priority pri)
{
    QThreader::loopStart(pri);
}

void QVoiceAssistant::loopStop()
{
    QThreader::loopStop();
}

void QVoiceAssistant::loopRunnable()
{
    QByteArray bytes= qMicrophone->ReadAll();
    qRecognizer->BuildRecognizer(bytes);
   qSpeaker->PlaySpeaker(bytes);
}

