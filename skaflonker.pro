include(guyframework/guyframework.pri)

INCLUDEPATH += . \
    ./json/include

TARGET = skaflonker

SOURCES += \
    skaflonker.cpp \
    json/lib_json/json_reader.cpp \
    json/lib_json/json_value.cpp \
    json/lib_json/json_writer.cpp \
    fighter.cpp \
    sprite.cpp \
    animatedsprite.cpp

HEADERS += \
    skaflonker.h \
    fighter.h \
    sprite.h \
    animatedsprite.h

DESTDIR = $$PWD/






