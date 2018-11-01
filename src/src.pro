QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = IQFileConverter

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    iq_file_converter.h

SOURCES += \
    main.cpp \
    iq_file_converter.cpp

# Paths
ROOT = $$clean_path($$PWD/..)
DESTDIR = $$ROOT/bin
OBJECTS_DIR = $$ROOT/tmp/$$TARGET
MOC_DIR = $$OBJECTS_DIR
UI_DIR = $$OBJECTS_DIR

# Include GNSS SDR Metadata library
GNSS_SDR_METADATA_DIR = $$ROOT/src/gnss-sdr-1.0
INCLUDEPATH *= $$GNSS_SDR_METADATA_DIR/include/api
INCLUDEPATH *= $$GNSS_SDR_METADATA_DIR/include/converter

win32:CONFIG(release, debug|release): {
  GNSS_SDR_LIBS = -L$$GNSS_SDR_METADATA_DIR/build/lib/x64/release -lapi -lxml -lcnv -lAdvapi32
}
else:win32:CONFIG(debug, debug|release): {
  GNSS_SDR_LIBS = -L$$GNSS_SDR_METADATA_DIR/build/lib/x64/debug -lapi -lxml -lcnv -lAdvapi32
}
macx: {
  GNSS_SDR_LIBS = -L$$GNSS_SDR_METADATA_DIR/build/osx/ -lapi -lxml -lcnv
}
else:linux-oe-g++: {
  GNSS_SDR_LIBS = -lapi -lxml -lcnv
}
else:unix: {
  GNSS_SDR_LIBS = -L$$GNSS_SDR_METADATA_DIR/build/linux64/ -lapi -lxml -lcnv
}

LIBS *= $$GNSS_SDR_LIBS
