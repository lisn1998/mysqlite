include ($$(MYSQLITE_HOME)/src/mysqlite.pri)

QT += core
QT -= gui

CONFIG += c++11
TEMPLATE = lib
TARGET = sqlite3
CONFIG += console

DESTDIR	      = $$MYSQLITE_LIB_PATH
DLLDESTDIR    = $$MYSQLITE_LIB_PATH
UI_DIR        = $$MYSQLITE_UIC_PATH/$$TARGET
MOC_DIR       = $$MYSQLITE_MOC_PATH/$$TARGET
OBJECTS_DIR   = $$MYSQLITE_OBJ_PATH/$$TARGET

HEADERS += \
        $$MYSQLITE_INCLUDE_PATH/sqlite3/sqlite3.h \
        sqlite3ext.h \
        sqlite3.pro \


SOURCES += \
        sqlite3.c \
		
# 里面有shell.c、sqlite3.c、sqlite3.h和sqlite3ext.h这么几个文件，其中shell.c我们不需要，其他的都是要用的
#		shell.c \

#if !defined(__SQLITESESSION_H_) && defined(SQLITE_ENABLE_SESSION)


DEFINES += SQLITE_ENABLE_FTS3
DEFINES += SQLITE_ENABLE_FTS5
DEFINES += SQLITE_ENABLE_RTREE
DEFINES += SQLITE_ENABLE_DBSTAT_VTAB
DEFINES += SQLITE_ENABLE_JSON1
DEFINES += SQLITE_ENABLE_RBU
DEFINES += SQLITE_ENABLE_SESSION




