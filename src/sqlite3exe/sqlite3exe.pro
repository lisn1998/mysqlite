include ($$(MYSQLITE_HOME)/src/mysqlite.pri)

QT += core
QT -= gui

CONFIG += c++11
TEMPLATE = app
TARGET = sqlite3exe
CONFIG += console

DESTDIR	      = $$MYSQLITE_BIN_PATH
UI_DIR        = $$MYSQLITE_UIC_PATH/$$TARGET
MOC_DIR       = $$MYSQLITE_MOC_PATH/$$TARGET
OBJECTS_DIR   = $$MYSQLITE_OBJ_PATH/$$TARGET

INCLUDEPATH += \
$$MYSQLITE_INCLUDE_PATH/sqlite3

HEADERS += \
sqlite3exe.pro \

#		$$MYSQLITE_INCLUDE_PATH/sqlite3/sqlite3.h \
#		sqlite3ext.h \
        


SOURCES += \
        shell.c \


LIBS += -lsqlite3

#		sqlite3.c \




