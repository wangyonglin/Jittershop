#ifndef PUBLICTHREADER_H
#define PUBLICTHREADER_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QWaitCondition>

class PublicThreader: public QThread
{
    Q_OBJECT
public:
    explicit PublicThreader(QObject *parent = nullptr);
    ~PublicThreader() override;

    enum State
    {

        Stoped,     ///<停止状态，包括从未启动过和启动后被停止
        Running,    ///<运行状态
        Paused      ///<暂停状态
    };

    State state() const;

public slots:
    void start(Priority pri = InheritPriority);
    void stop();
    void pause();
    void resume();

protected:
    virtual void run() override final;
    virtual void loopRunnable()=0;
public:
    std::atomic_bool pauseFlag;
    volatile std::atomic_bool stopFlag;
    QMutex mutex;
    QWaitCondition condition;
};


#endif // PUBLICTHREADER_H
