TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

LIBS += -L"D:\SFML\QTSFML\SFML-2.3.2\lib"

CONFIG(release, debug|release):
LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system

CONFIG(debug, debug|release):
LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += "D:\SFML\QTSFML\SFML-2.3.2\include"

SOURCES += main.cpp \
    entity.cpp \
<<<<<<< HEAD
    player.cpp
=======
    player.cpp \
    enemy.cpp
>>>>>>> develop

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    map.h \
    entity.h \
<<<<<<< HEAD
    player.h
=======
    player.h \
    enemy.h
>>>>>>> develop

