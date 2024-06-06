#ifndef QRECOGNIZER_H
#define QRECOGNIZER_H

#include <sherpa-ncnn/csrc/display.h>
#include <sherpa-ncnn/csrc/recognizer.h>
#include <sherpa-ncnn/csrc/wave-reader.h>

#include <QObject>
#include <QDebug>
#include <QDataStream>

#define SET_STRING_BY_ENV(config, key) \
if (getenv(key)) {                   \
        config = getenv(key);              \
}

#define SET_CONFIG_BY_ENV(config, key, required) \
config = "";                                   \
    SET_STRING_BY_ENV(config, key);                \
    if (!(config).empty() && required) {           \
        parsed_required_envs++;                      \
}

#define SET_INTEGER_BY_ENV(config, key)                  \
{                                                      \
        std::string val;                                     \
        SET_STRING_BY_ENV(val, "SHERPA_NCNN_ASD_ENDPOINTS"); \
        if (!val.empty() && ::atoi(val.c_str()) > 0) {       \
            config = ::atoi(val.c_str());                      \
    }                                                    \
}

// A simple display, without window support, doesn't rewrite current line.
// It only output the new text, which only works in greedy_search mode.
// It doesn't support modified_beam_search mode, which might change the
// generated text.
class SimpleDisplay : public sherpa_ncnn::Display {
public:
    SimpleDisplay(std::string label) {
        label_ = label.empty() ? "" : label + ":";
    }
    void Print(int32_t segment_id, const std::string &s) {
        if (last_segment_ != segment_id) {
            last_segment_ = segment_id;
            last_text_ = "";
            if (segment_id) {
                fprintf(stderr, "\n");
            }
            fprintf(stderr, "%s%d:", label_.c_str(), segment_id);
            if (!s.empty() && s.at(0) != ' ') {
                fprintf(stderr, " ");
            }
        }

        if (s.length() > last_text_.length()) {
            std::string tmp(s.begin() + last_text_.length(), s.end());
            fprintf(stderr, "%s", tmp.c_str());
        } else {
            fprintf(stderr, "%s", s.c_str());
        }
        last_text_ = s;
    }

private:
    std::string label_;
    std::string last_text_;
    int32_t last_segment_ = -1;
};

class QRecognizer : public QObject
{
    Q_OBJECT
public:


    explicit QRecognizer(QObject *parent = nullptr);

    bool InitRecognizer(int32_t num_threads ,QString method);
    void BuildRecognizer(QByteArray & bytes);
signals:
private:
    int32_t segment_index = 0;
    bool is_ok = false;
    float expected_sampling_rate = 16000;
    std::unique_ptr<sherpa_ncnn::Recognizer> recognizer;
    std::unique_ptr<sherpa_ncnn::Stream> stream;
    std::unique_ptr<sherpa_ncnn::Display> display;
    void SetDefaultConfigurations(sherpa_ncnn::RecognizerConfig &config);
    std::unique_ptr<sherpa_ncnn::Display> CreateDisplay();
};

#endif // QRECOGNIZER_H
