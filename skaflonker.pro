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
    actionsfighter.cpp

HEADERS += \
    skaflonker.h \
    sprite.h \
    player.h \
    action.h \
    actionsfighter.h

DESTDIR = $$PWD/






