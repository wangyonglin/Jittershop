#ifndef QVOICEASSISTANT_H
#define QVOICEASSISTANT_H

#include <QObject>
#include "QMicrophone.h"
#include "QThreader.h"
#include "QSpeaker.h"
#include "QRecognizer.h"

class QVoiceAssistant : public QThreader
{
    Q_OBJECT
public:
    explicit QVoiceAssistant(QObject *parent = nullptr);
    void InitVoiceAssistant();
    virtual void loopStart(Priority pri = InheritPriority);
    virtual void loopStop();
private:
    QMicrophone *qMicrophone=nullptr;
    QRecognizer *qRecognizer=nullptr;
    QSpeaker *qSpeaker=nullptr;
    virtual void loopRunnable();
};

#endif // QVOICEASSISTANT_H
