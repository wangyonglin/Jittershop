#include "SnowboyWrapper.h"

SnowboyWrapper::SnowboyWrapper(QObject *parent)
    : PublicThreader{parent}
{
    audio_render.InitMicrophone();
    audio_render.InitSpeaker();
}

SnowboyWrapper::~SnowboyWrapper()
{
    audio_render.CloseMicrophone();
    audio_render.CloseSpeaker();
}

void SnowboyWrapper::InitDetect(const QString &resource_filename,const QString &model_filename){

    std::string sensitivity_str = "0.5";
    float audio_gain = 1;
    bool apply_frontend = false;
    // Initializes Snowboy detector->
    detector= new snowboy::SnowboyDetect(resource_filename.toStdString(), model_filename.toStdString());
    detector->SetSensitivity(sensitivity_str);
    detector->SetAudioGain(audio_gain);
    detector->ApplyFrontend(apply_frontend);
}



bool  SnowboyWrapper::RunDetection(QByteArray &audio_pcm_data) {
    std::vector<int16_t> data;
    QBuffer buffer(&audio_pcm_data);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);
    in.setByteOrder(QDataStream::LittleEndian);


    while (!in.atEnd()) {
        qint16 sample;
        in >> sample;
        data.push_back(sample);
    }

    if(data.size() != 0 && detector){
        int result = detector->RunDetection(data.data(), data.size());
        if (result > 0) {
            qDebug() << "Hotword " << result << " detected!";
            return true;
        }
    }
    return false;
}

void SnowboyWrapper::stop()
{
    PublicThreader::stop();
    // if(detector){
    //     delete detector;
    //     detector=nullptr;
    // }
}

void SnowboyWrapper::loopRunnable()
{
   QByteArray data= audio_render.ReadAll();
    RunDetection(data);
   audio_render.WritePlay(data);
}
