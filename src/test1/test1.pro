include ($$(MYSQLITE_HOME)/src/mysqlite.pri)

QT += core
QT -= gui

TEMPLATE = app

TARGET = test1
CONFIG += console

DESTDIR	      = $$MYSQLITE_BIN_PATH
UI_DIR        = $$MYSQLITE_UIC_PATH/$$TARGET
MOC_DIR       = $$MYSQLITE_MOC_PATH/$$TARGET
OBJECTS_DIR   = $$MYSQLITE_OBJ_PATH/$$TARGET

INCLUDEPATH += \
$$MYSQLITE_INCLUDE_PATH/sqlite3

HEADERS += \
        skmonitorsql.h \
        test1.pro \


SOURCES += \
        skmonitorsql.cpp \
		main.cpp
		
LIBS += -lsqlite3



