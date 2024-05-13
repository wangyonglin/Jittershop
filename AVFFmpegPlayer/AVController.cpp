#include "AVController.h"

AVController::AVController(QWidget *parent)
    : QWidget{parent}
{

}

AVController::~AVController()
{

}

void AVController::ImageUpdate(QImage imageUpdate)
{
    __imageUpdate=imageUpdate;
    update();
}

void AVController::InitIODevice(int dst_nb_samples, int rate, int sample_size, int nch)
{

    QAudioFormat format;
    format.setSampleRate(rate); // 采样率
    format.setChannelCount(nch);   // 声道数
    format.setSampleSize(sample_size);    // 采样大小
    format.setCodec("audio/pcm"); // 音频编码格式
    format.setByteOrder(QAudioFormat::LittleEndian); // 字节顺序
    format.setSampleType(QAudioFormat::SignedInt);  // 采样类型

    int len = dst_nb_samples * format.channelCount() * format.sampleSize()/8;
    Q_UNUSED(len);
    // 创建 QAudioOutput 对象
    audioOutput = new QAudioOutput(format);
    //audioOutput->setBufferSize(len * 100);
    audioOutput->setVolume(1.0); // 设置音量（0.0 - 1.0）

    // 打开音频输出
    outputDevice = audioOutput->start();
}

void AVController::WriteIODevice(const char *data, qint64 len)
{
    outputDevice->write(data, len);

}

void AVController::FreeIODevice()
{
     audioOutput->stop();
}

void AVController::StartSysClockMs()
{
    if (this->player_start_time_ms == 0)
    {
        this->player_start_time_ms = now_ms();
    }
}

void AVController::clearStartTime()
{
    player_start_time_ms=0;
}

int64_t AVController::now_ms()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

int64_t AVController::GetSysClockMs()
{
    if (this->player_start_time_ms != 0)
    {
        return  now_ms() - this->player_start_time_ms;
    }
    return 0;
}

void AVController::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawLine(10, 10, 100, 100); // 画线
    painter.drawRect(10, 10, 100, 100); // 画矩形
    painter.setFont(QFont("Arial",30)); // 设置字体
    painter.drawText(10, 10, "Hello"); // 指定位置画文本
    painter.drawText(rect(), Qt::AlignCenter, "Hello"); // 窗口中间画文本
    painter.drawImage(rect(), __imageUpdate);
}
