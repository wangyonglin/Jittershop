#include "AtomThreader.h"

AtomThreader::AtomThreader(QObject *parent)
    : QThread(parent),
    pauseFlag(false),
    stopFlag(false)
{}

AtomThreader::~AtomThreader()
{

}

AtomThreader::State AtomThreader::state() const
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

void AtomThreader::start(Priority pri)
{
    QThread::start(pri);
}

void AtomThreader::stop()
{
    if (QThread::isRunning())
    {
        stopFlag = true;
        condition.wakeAll();
        QThread::quit();
        QThread::wait();
    }
}

void AtomThreader::pause()
{
    if (QThread::isRunning())
    {
        pauseFlag = true;
    }
}

void AtomThreader::resume()
{
    if (QThread::isRunning())
    {
        pauseFlag = false;
        condition.wakeAll();
    }
}

void AtomThreader::run()
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
