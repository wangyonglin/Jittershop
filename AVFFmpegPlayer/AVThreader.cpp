#include "AVThreader.h"

AVThreader::AVThreader(QObject *parent)
    : QThread(parent),
    pauseFlag(false),
    stopFlag(false)
{}
AVThreader::~AVThreader()
{

    //stop();
}

AVThreader::State AVThreader::state() const
{
    State s = Stoped;
    if (!QThread::isRunning())
    {
        s = Stoped;
    }
    else if (QThread::isRunning() && pauseFlag)
    {
        s = Paused;
    }
    else if (QThread::isRunning() && (!pauseFlag))
    {
        s = Running;
    }
    return s;
}

void AVThreader::start(Priority pri)
{
    QThread::start(pri);
}

void AVThreader::stop()
{
    if (QThread::isRunning())
    {
        stopFlag = true;
        condition.wakeAll();
        QThread::quit();
        QThread::wait();
    }
}

void AVThreader::pause()
{
    if (QThread::isRunning())
    {
        pauseFlag = true;
    }
}

void AVThreader::resume()
{
    if (QThread::isRunning())
    {
        pauseFlag = false;
        condition.wakeAll();
    }
}

void AVThreader::run()
{
    qDebug() << "enter thread : " << QThread::currentThreadId();
    while (!stopFlag)
    {
        loopRunnable(); // do something...

        if (pauseFlag)
        {
            mutex.lock();
            condition.wait(&mutex);
            mutex.unlock();
        }
    }
    pauseFlag = false;
    stopFlag = false;

    qDebug() << "exit thread : " << QThread::currentThreadId();
}
