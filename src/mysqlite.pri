# ����ͨ���������� MYSQLITE_HOME ָ������Ŀ¼
# ���� isEmpty ��������ֱ�ӶԻ������������жϣ������Ƚ������һ����ʱ������
MYSQLITE_HOME = $$(MYSQLITE_HOME)
isEmpty(MYSQLITE_HOME) {
	error('MYSQLITE_HOME'�����������뱻����.)
}

MYSQLITE_BIN_PATH = $$MYSQLITE_HOME/bin
MYSQLITE_LIB_PATH = $$MYSQLITE_HOME/lib
MYSQLITE_SRC_PATH = $$MYSQLITE_HOME/src
MYSQLITE_OBJ_PATH = $$MYSQLITE_HOME/obj
MYSQLITE_UIC_PATH = $$MYSQLITE_HOME/obj/uic
MYSQLITE_MOC_PATH = $$MYSQLITE_HOME/obj/moc
MYSQLITE_INCLUDE_PATH = $$MYSQLITE_HOME/src/include

LIBS += -L$$MYSQLITE_LIB_PATH
# ֧��c++11
CONFIG += c++11
# #########################��ͬϵͳ�ĺ궨��###########################################
win32{
	DEFINES += WIN32
    DEFINES -= UNIX
}

unix{
    DEFINES -= WIN32
	DEFINES += UNIX
}
# #########################����ϵͳʹ�õı���汾����###########################################
# ���δָ�� BUILDTYPE ����������ȱʡ�� debug ����
BUILDTYPE = $$(BUILDTYPE)
isEmpty(BUILDTYPE) {	
	message('BUILDTYPE'��������δ���ã�ʹ��ȱʡֵdebug.)
	BUILDTYPE = debug
}

# ��BUILDTYPE���ý��м򵥵Ĵ�����
# BUILDTYPE��ֵ����Ϊ��ֵ
!count(BUILDTYPE,1){
	error(���ܶ���ʹ��һ����Ч�ı���汾���ͣ�����BUILDTYPE������������.)
}
!equals(BUILDTYPE,all){
    !equals(BUILDTYPE,debug){
        !equals(BUILDTYPE,release){
            error(δ��֧�ֵĵı���汾���ͣ�����BUILDTYPE������������.)
        }
    }
}

# ��ͬ����汾��ص�����
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
# ָ�������к궨��
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


# #########################����ϵͳʹ�õı���λ������###########################################
# ����ϵͳʹ�õı���λ������
# ���δָ�� BUILDBIT ����������ȱʡ�� 64 λ����
BUILDBIT = $$(BUILDBIT)
isEmpty(BUILDBIT) {	
	message('BUILDBIT'��������δ���ã�ʹ��ȱʡֵ 64 λ.)
	BUILDBIT = 64
}
# ��BUILDBIT���ý��м򵥵Ĵ�����
# BUILDBIT��ֵ����Ϊ��ֵ
!count(BUILDBIT,1){
	error(���ܶ���ʹ��һ����Ч�ı���λ�����ͣ�����BUILDBIT������������.)
}
!equals(BUILDBIT,32){
	!equals(BUILDBIT,64){
		error(δ��֧�ֵĵı���λ�����ͣ�����BUILDBIT������������.)
	}
}

# ��ͬ����汾��ص�����
equals(BUILDBIT,32){
	# ��չ 32 λ������
	CONFIG *= x86

	DEFINES *= BUILDBIT32
}
equals(BUILDBIT,64){
	# ��չ 64 λ������
	CONFIG *= x64
	DEFINES *= BUILDBIT64
}

