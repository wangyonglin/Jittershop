QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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

unix|win32: LIBS += -lopencv_core
unix|win32: LIBS += -lopencv_highgui
unix|win32: LIBS += -lopencv_imgcodecs
unix|win32: LIBS += -lopencv_imgproc

unix|win32: LIBS += -lavcodec
unix|win32: LIBS += -lavdevice
unix|win32: LIBS += -lavfilter
unix|win32: LIBS += -lavformat
unix|win32: LIBS += -lavutil
unix|win32: LIBS += -lpostproc
unix|win32: LIBS += -lswscale
unix|win32: LIBS += -lswresample

unix|win32: LIBS += -lSDL2

DISTFILES += \
    README.md
