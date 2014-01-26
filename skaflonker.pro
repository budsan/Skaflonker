include(guyframework/guyframework.pri)

INCLUDEPATH += . \
    ./json/include

TARGET = skaflonker

SOURCES += \
    skaflonker.cpp \
    json/lib_json/json_reader.cpp \
    json/lib_json/json_value.cpp \
    json/lib_json/json_writer.cpp \
    sprite.cpp \
    player.cpp \
    action.cpp \
    actionsfighter.cpp \
    ingame.cpp \
    standardsprite.cpp \
    box.cpp \
    animatedsprite.cpp \
    splash.cpp \
    environmentevent.cpp \
    lavaevent.cpp

HEADERS += \
    skaflonker.h \
    sprite.h \
    player.h \
    action.h \
    actionsfighter.h \
    ingame.h \
    standardsprite.h \
    box.h \
    animatedsprite.h \
    splash.h \
    environmentevent.h \
    lavaevent.h

DESTDIR = $$PWD/






