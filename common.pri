ROOT = $$clean_path($$PWD)
DESTDIR = $$ROOT/bin
OBJECTS_DIR = $$ROOT/tmp/$$TARGET
MOC_DIR = $$OBJECTS_DIR
UI_DIR = $$OBJECTS_DIR

unix {
  QMAKE_CXXFLAGS *= -Wall -Wextra -Werror -Wno-unknown-pragmas
}
