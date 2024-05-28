#ifndef SNOWBOYWRAPPER_H
#define SNOWBOYWRAPPER_H
#include "snowboy-detect.h"
#include <QBuffer>
#include <QByteArray>
#include <QIODevice>
#include <QVector>
#include <QDataStream>
#include <QObject>
#include <QDebug>
#include "PublicUtils/PublicRender.h"
#include "PublicUtils/PublicThreader.h"
class SnowboyWrapper : public PublicThreader
{
    Q_OBJECT
public:
    explicit SnowboyWrapper(QObject *parent = nullptr);
    ~SnowboyWrapper();
    void InitDetect(const QString &resource_filename, const QString &model_filename);
    bool RunDetection(QByteArray &audio_pcm_data);

    virtual void stop();
private:
    snowboy::SnowboyDetect *detector=nullptr;
    PublicRender audio_render;
    virtual void loopRunnable();
};

#endif // SNOWBOYWRAPPER_H
