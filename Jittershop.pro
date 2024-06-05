QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/QEngine/QEngine.pri)
include($$PWD/AVFFmpegPlayer/AVFFmpegPlayer.pri)


SOURCES += \
    main.cpp \
    MainWidget.cpp

HEADERS += \
    MainWidget.h

TRANSLATIONS += \
    Jittershop_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# unix|win32: LIBS += -lopencv_core
# unix|win32: LIBS += -lopencv_highgui
# unix|win32: LIBS += -lopencv_imgcodecs
# unix|win32: LIBS += -lopencv_imgproc

contains(DEFINES, unix)
{
    INCLUDEPATH +=\
        $$PWD/include

    LIBS += -lavcodec
    LIBS += -lavdevice
    LIBS += -lavfilter
    LIBS += -lavformat
    LIBS += -lavutil
    LIBS += -lswscale
    LIBS += -lswresample
    LIBS  += -lcblas
    LIBS  += -Wall -lpthread
}

contains(DEFINES, unix)
{
    INCLUDEPATH +=  $$PWD/vendor
    INCLUDEPATH += $$PWD/vendor/ncnn-build/src
    INCLUDEPATH += $$PWD/vendor/ncnn-src/src

    SOURCES +=  $$PWD/vendor/ncnn-src/src/mat.cpp
    SOURCES +=  $$PWD/vendor/ncnn-src/src/net.cpp

    LIBS +=  $$PWD/vendor/lib/libncnn.a
    LIBS +=  $$PWD/vendor/lib/libsherpa-ncnn-c-api.a
    LIBS +=  $$PWD/vendor/lib/libsherpa-ncnn-core.a
    LIBS +=  $$PWD/vendor/lib/libkaldi-native-fbank-core.a
    LIBS +=  $$PWD/vendor/lib/libsherpa-ncnn-portaudio_static.a
    LIBS  += -Wall -lpthread  -lgomp -lcblas
}

DISTFILES += \
    README.md
