@echo off
@REM 由于sqllite不会生成lib无法被其他项目引用, 就必须这样搞了
cd %MYSQLITE_HOME%\src\sqlite3
qmake
qmake -tp vc
nmake clean
nmake
cd %MYSQLITE_HOME%\obj\sqlite3
lib sqlite3.obj
move /Y %MYSQLITE_HOME%\obj\sqlite3\sqlite3.lib %MYSQLITE_HOME%\lib\

@REM #######################################################################
cd %MYSQLITE_HOME%\src\sqlite3exe
qmake
qmake -tp vc
nmake clean
nmake
@REM #######################################################################
cd %MYSQLITE_HOME%\src\test1
qmake
qmake -tp vc
nmake clean
nmake


cd  %MYSQLITE_HOME%
@echo on