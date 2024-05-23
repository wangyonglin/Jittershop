#ifndef AVDEMUXER_H
#define AVDEMUXER_H
#include "Threader.h"
#include <QObject>

#include "AVController.h"
class AVDemuxer : public Threader
{
    Q_OBJECT
public:
    explicit AVDemuxer(QObject *parent = nullptr);

public:
    bool frameFinished=true;
    AVController*controller;
public slots:
    virtual void start(Priority pri = InheritPriority);
    virtual void stop();
    virtual void pause();
    virtual void resume();
    AVController* initParameters(AVController *controller);
    void freeParameters(AVController *controller);
private:
    virtual void loopRunnable();

};

#endif // AVDEMUXER_H