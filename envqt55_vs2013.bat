@set QMAKESPEC=win32-msvc2013
@set QTDIR=S:\Qt\Qt5.5.1\5.5\msvc2013_64

@set PATH=%QTDIR%\bin;%QTDIR%\lib;%PATH%
@set PATH=%PATH%;S:\Qt\Qt5.5.1\Tools\QtCreator\bin;S:\Qt\Qt5.5.1\Tools\QtCreator\lib;S:\Qt\Qt5.5.1\Tools\QtCreator\lib\qtcreatorcdbext64
@REM 这是自定义的变量, 以便于后继在开发中使用
@set MYSQLITE_HOME=D:\mysqlite
@set PATH=%MYSQLITE_HOME%\bin;%MYSQLITE_HOME%\lib;%PATH%

@echo Setting environment for qt5              %QTDIR%
@echo Setting environment for QMAKESPEC        %QMAKESPEC%
@echo Setting environment for MYSQLITE_HOME    %MYSQLITE_HOME%