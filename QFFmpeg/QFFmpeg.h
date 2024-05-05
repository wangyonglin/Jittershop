#ifndef QFFMPEG_H
#define QFFMPEG_H

#include <QObject>

class QFFmpeg : public QObject
{
    Q_OBJECT
public:
    explicit QFFmpeg(QObject *parent = nullptr);

signals:
};

#endif // QFFMPEG_H
