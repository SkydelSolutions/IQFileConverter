! include (../common.pri) {
    error( "Couldn't find the common.pri file!" )
}
! include (../gnss_sdr_metadata.pri) {
    error( "Couldn't find the gnss_sdr_metadata.pri file!" )
}

QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = IQFileConverter

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    iq_file_converter.h \
    gnss_sdr_metadata_wrapper.h

SOURCES += \
    main.cpp \
    iq_file_converter.cpp \
    gnss_sdr_metadata_wrapper.cpp

LIBS *= $$GNSS_SDR_LIBS
