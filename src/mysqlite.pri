# 需先通过环境变量 MYSQLITE_HOME 指定开发目录
# 由于 isEmpty 函数不能直接对环境变量进行判断，所以先将其放入一个临时变量中
MYSQLITE_HOME = $$(MYSQLITE_HOME)
isEmpty(MYSQLITE_HOME) {
	error('MYSQLITE_HOME'环境变量必须被定义.)
}

MYSQLITE_BIN_PATH = $$MYSQLITE_HOME/bin
MYSQLITE_LIB_PATH = $$MYSQLITE_HOME/lib
MYSQLITE_SRC_PATH = $$MYSQLITE_HOME/src
MYSQLITE_OBJ_PATH = $$MYSQLITE_HOME/obj
MYSQLITE_UIC_PATH = $$MYSQLITE_HOME/obj/uic
MYSQLITE_MOC_PATH = $$MYSQLITE_HOME/obj/moc
MYSQLITE_INCLUDE_PATH = $$MYSQLITE_HOME/src/include

LIBS += -L$$MYSQLITE_LIB_PATH
# 支持c++11
CONFIG += c++11
# #########################不同系统的宏定义###########################################
win32{
	DEFINES += WIN32
    DEFINES -= UNIX
}

unix{
    DEFINES -= WIN32
	DEFINES += UNIX
}
# #########################配置系统使用的编译版本类型###########################################
# 如果未指定 BUILDTYPE 环境变量，缺省按 debug 处理
BUILDTYPE = $$(BUILDTYPE)
isEmpty(BUILDTYPE) {	
	message('BUILDTYPE'环境变量未设置，使用缺省值debug.)
	BUILDTYPE = debug
}

# 对BUILDTYPE设置进行简单的错误检查
# BUILDTYPE的值必须为单值
!count(BUILDTYPE,1){
	error(仅能定义使用一种有效的编译版本类型，请检查BUILDTYPE环境变量设置.)
}
!equals(BUILDTYPE,all){
    !equals(BUILDTYPE,debug){
        !equals(BUILDTYPE,release){
            error(未被支持的的编译版本类型，请检查BUILDTYPE环境变量设置.)
        }
    }
}

# 不同编译版本相关的配置
equals(BUILDTYPE,debug){
	CONFIG += debug
	CONFIG -= release
}
equals(BUILDTYPE,release){
	CONFIG += release
	CONFIG -= debug
}
equals(BUILDTYPE,all){
	CONFIG += debug_and_release build_all
}
# 指定代码中宏定义
debug_and_release {
	CONFIG(debug, debug|release) {
	    DEFINES += DEBUG
	}
	CONFIG(release, debug|release) {
	    DEFINES += RELEASE
	}
} else {
	debug:DEFINES += DEBUG
	release:DEFINES += RELEASE
}


# #########################配置系统使用的编译位数类型###########################################
# 配置系统使用的编译位数类型
# 如果未指定 BUILDBIT 环境变量，缺省按 64 位处理
BUILDBIT = $$(BUILDBIT)
isEmpty(BUILDBIT) {	
	message('BUILDBIT'环境变量未设置，使用缺省值 64 位.)
	BUILDBIT = 64
}
# 对BUILDBIT设置进行简单的错误检查
# BUILDBIT的值必须为单值
!count(BUILDBIT,1){
	error(仅能定义使用一种有效的编译位数类型，请检查BUILDBIT环境变量设置.)
}
!equals(BUILDBIT,32){
	!equals(BUILDBIT,64){
		error(未被支持的的编译位数类型，请检查BUILDBIT环境变量设置.)
	}
}

# 不同编译版本相关的配置
equals(BUILDBIT,32){
	# 扩展 32 位配置项
	CONFIG *= x86

	DEFINES *= BUILDBIT32
}
equals(BUILDBIT,64){
	# 扩展 64 位配置项
	CONFIG *= x64
	DEFINES *= BUILDBIT64
}

