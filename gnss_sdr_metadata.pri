GNSS_SDR_METADATA_DIR = $$ROOT/src/gnss-sdr-1.0
INCLUDEPATH *= $$GNSS_SDR_METADATA_DIR/include/api
INCLUDEPATH *= $$GNSS_SDR_METADATA_DIR/include/converter

win32:CONFIG(release, debug|release): {
  GNSS_SDR_LIBS = -L$$GNSS_SDR_METADATA_DIR/build/lib/x64/release -lapi -lxml -lcnv
}
else:win32:CONFIG(debug, debug|release): {
  GNSS_SDR_LIBS = -L$$GNSS_SDR_METADATA_DIR/build/lib/x64/debug -lapi -lxml -lcnv
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
