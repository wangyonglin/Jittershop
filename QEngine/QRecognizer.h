#ifndef QRECOGNIZER_H
#define QRECOGNIZER_H

#include <sherpa-ncnn/csrc/display.h>
#include <sherpa-ncnn/csrc/recognizer.h>
#include <sherpa-ncnn/csrc/wave-reader.h>

#include <QObject>
#include <QDebug>
#include <QDataStream>

class QRecognizer : public QObject
{
    Q_OBJECT
public:


    explicit QRecognizer(QObject *parent = nullptr);

    bool InitRecognizer(int32_t num_threads ,QString method);
    void BuildRecognizer(QByteArray & bytes);
signals:
private:
    bool is_ok = false;
    float expected_sampling_rate = 16000;
    std::unique_ptr<sherpa_ncnn::Recognizer> recognizer;
    std::unique_ptr<sherpa_ncnn::Stream> stream;
    void SetDefaultConfigurations(sherpa_ncnn::RecognizerConfig &config);
};

#endif // QRECOGNIZER_H
