! include (../common.pri) {
    error( "Couldn't find the common.pri file!" )
}
! include (../gnss_sdr_metadata.pri) {
    error( "Couldn't find the gnss_sdr_metadata.pri file!" )
}

QT += testlib

QT += network
QT -= gui

TARGET = unit_tests
CONFIG += console
CONFIG -= app_bundle

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += $$PWD/../src

SOURCES += \
    ../src/iq_file_converter.cpp \
    ../src/gnss_sdr_metadata_wrapper.cpp \
    iq_file_converter_test.cpp

HEADERS += \
    iq_file_converter_test.h

LIBS *= $$GNSS_SDR_LIBS
