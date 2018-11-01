QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = IQFileConverter

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp

ROOT = $$clean_path($$PWD/..)
DESTDIR = $$ROOT/bin
OBJECTS_DIR = $$ROOT/tmp/$$TARGET
MOC_DIR = $$OBJECTS_DIR
UI_DIR = $$OBJECTS_DIR
