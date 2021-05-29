QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lopengl32

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    simpglwidget.cpp \
    simpleton4.cpp \
    simpleton4asm.cpp \
    simpleton4disasm.cpp \
    simpxmmu.cpp \
    utils.cpp

HEADERS += \
    mainwindow.h \
    simpglwidget.h \
    simpleton4.h \
    simpleton4asm.h \
    simpleton4disasm.h \
    simpxmmu.h \
    utils.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    SimpX_en_GB.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    asm/simpx.inc \
    asm/test0.asm
