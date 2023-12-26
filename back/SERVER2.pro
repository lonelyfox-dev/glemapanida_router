QT -= gui
QT += network
QT +=sql
QT += core5compat
QT += core
CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        api.cpp \
        config.cpp \
        dbcontroller.cpp \
        main.cpp \
        requestparser.cpp \
        shedulebuilder.cpp \
        tcplistener.cpp \
        ticket.cpp \
        topology.cpp \
        webserver.cpp
LIBS += -lws2_32
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    api.h \
    config.h \
    dbcontroller.h \
    requestparser.h \
    shedulebuilder.h \
    tcplistener.h \
    ticket.h \
    topology.h \
    webserver.h
DISTFILES += \
    /html/* \
    SERVER2.ini
