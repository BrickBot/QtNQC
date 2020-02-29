purge.commands = rm -f *~

TEMPLATE        = app
LANGUAGE	= C++
CONFIG		+= qt warn_on thread debug
QMAKE_DEL_FILE	+= -r
QMAKE_CLEAN	+= qtnqc qtnqc.app
QMAKE_EXTRA_UNIX_TARGETS = purge
INCPATH		+= /usr/X11R6/include /usr/local/include
# Solaris 10 and CC.
LIBS		+= -L/usr/local/lib -lcurl -ldl -lsocket -lnsl -lz \
		   -R/usr/local/lib -lcurl -ldl -lsocket -lnsl -lz
QMAKE_CFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O2
# FreeBSD 5.5 and g++.
# LIBS		+= -L/usr/local/lib -lcurl
# QMAKE_CXXFLAGS  -= -O
# QMAKE_CXXFLAGS  += -O0 -fno-inline

HEADERS		=	break.h\
			constant.h\
                        continue.h\
			diagram_writer.h\
			dragobject.h\
			editor.h\
			file_class.h\
			function.h\
                        function_window.h\
                        gate.h\
			qtnqc_download_thread.h\
			qtnqc_limits.h\
			list_objects.h\
			loop.h\
                        object.h\
                        options_reader.h\
                        qtnqc_main.h\
			rename_window.h\
                        return.h\
			search_thread.h\
			settings.h\
                        settings_window.h\
                        subroutine.h\
			task.h\
                        tools_window.h\
                        types.h\
			variable.h\
			wire.h

SOURCES		=	break.cc\
			constant.cc\
                        continue.cc\
			diagram_writer.cc\
			dragobject.cc\
			editor.cc\
			function.cc\
                        function_window.cc\
                        gate.cc\
			list_objects.cc\
			loop.cc\
			object.cc\
                        options_reader.cc\
			qtnqc_download_thread.cc\
                        qtnqc_main.cc\
			rename_window.cc\
                        return.cc\
			search_thread.cc\
			settings.cc\
                        settings_window.cc\
                        subroutine.cc\
			task.cc\
                        tools_window.cc\
			variable.cc\
			wire.cc

TARGET		= qtnqc
PROJECTNAME	= QtNQC
