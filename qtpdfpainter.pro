QT += core gui quickcontrols2 charts svg printsupport

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    chartpainter.h \
    chartpainterlineseries.h \
    imagecontent.h \
    pdfreport.h \
    abstractpainter.h \
    tablecell.h \
    tablecellcontent.h \
    tablepainter.h \
    tablerow.h \
    textcontent.h \

SOURCES += \
    chartpainter.cpp \
    chartpainterlineseries.cpp \
    imagecontent.cpp \
    pdfreport.cpp \
    tablecell.cpp \
    tablecellcontent.cpp \
    tablepainter.cpp \
    tablerow.cpp \
    textcontent.cpp \
    main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
