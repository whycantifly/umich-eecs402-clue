#############################################################################
# Makefile for building: umich-eecs402-clue
# Generated by qmake (2.01a) (Qt 4.7.4) on: Sat Mar 31 21:14:34 2012
# Project:  umich-eecs402-clue.pro
# Template: app
# Command: /usr/bin/qmake -o Makefile umich-eecs402-clue.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -I. -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = AccusationDialogClass.cpp \
		BoardLocationClass.cpp \
		CaseFileClass.cpp \
		ClueMainWindowClass.cpp \
		DeckClass.cpp \
		ExceptionClass.cpp \
		main.cpp \
		PlayerClass.cpp \
		SuggestionClass.cpp \
		SuggestionDialogClass.cpp moc_AccusationDialogClass.cpp \
		moc_ClueMainWindowClass.cpp \
		moc_SuggestionDialogClass.cpp
OBJECTS       = AccusationDialogClass.o \
		BoardLocationClass.o \
		CaseFileClass.o \
		ClueMainWindowClass.o \
		DeckClass.o \
		ExceptionClass.o \
		main.o \
		PlayerClass.o \
		SuggestionClass.o \
		SuggestionDialogClass.o \
		moc_AccusationDialogClass.o \
		moc_ClueMainWindowClass.o \
		moc_SuggestionDialogClass.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		umich-eecs402-clue.pro
QMAKE_TARGET  = umich-eecs402-clue
DESTDIR       = 
TARGET        = umich-eecs402-clue

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_AccusationDialogClass.h ui_ClueMainWindowClass.h ui_SuggestionDialogClass.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: umich-eecs402-clue.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/i386-linux-gnu/libQtGui.prl \
		/usr/lib/i386-linux-gnu/libQtCore.prl
	$(QMAKE) -o Makefile umich-eecs402-clue.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/i386-linux-gnu/libQtGui.prl:
/usr/lib/i386-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile umich-eecs402-clue.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/umich-eecs402-clue1.0.0 || $(MKDIR) .tmp/umich-eecs402-clue1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/umich-eecs402-clue1.0.0/ && $(COPY_FILE) --parents AccusationDialogClass.h BoardLocationClass.h cardToRoom.h cardToSuspect.h cardToWeapon.h CaseFileClass.h ClueMainWindowClass.h constants.h DeckClass.h enums.h ExceptionClass.h PlayerClass.h SuggestionClass.h SuggestionDialogClass.h .tmp/umich-eecs402-clue1.0.0/ && $(COPY_FILE) --parents AccusationDialogClass.cpp BoardLocationClass.cpp CaseFileClass.cpp ClueMainWindowClass.cpp DeckClass.cpp ExceptionClass.cpp main.cpp PlayerClass.cpp SuggestionClass.cpp SuggestionDialogClass.cpp .tmp/umich-eecs402-clue1.0.0/ && $(COPY_FILE) --parents AccusationDialogClass.ui ClueMainWindowClass.ui SuggestionDialogClass.ui .tmp/umich-eecs402-clue1.0.0/ && (cd `dirname .tmp/umich-eecs402-clue1.0.0` && $(TAR) umich-eecs402-clue1.0.0.tar umich-eecs402-clue1.0.0 && $(COMPRESS) umich-eecs402-clue1.0.0.tar) && $(MOVE) `dirname .tmp/umich-eecs402-clue1.0.0`/umich-eecs402-clue1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/umich-eecs402-clue1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_AccusationDialogClass.cpp moc_ClueMainWindowClass.cpp moc_SuggestionDialogClass.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_AccusationDialogClass.cpp moc_ClueMainWindowClass.cpp moc_SuggestionDialogClass.cpp
moc_AccusationDialogClass.cpp: ui_AccusationDialogClass.h \
		SuggestionClass.h \
		enums.h \
		DeckClass.h \
		constants.h \
		BoardLocationClass.h \
		AccusationDialogClass.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) AccusationDialogClass.h -o moc_AccusationDialogClass.cpp

moc_ClueMainWindowClass.cpp: ui_ClueMainWindowClass.h \
		constants.h \
		BoardLocationClass.h \
		enums.h \
		CaseFileClass.h \
		DeckClass.h \
		SuggestionClass.h \
		ExceptionClass.h \
		PlayerClass.h \
		ClueMainWindowClass.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) ClueMainWindowClass.h -o moc_ClueMainWindowClass.cpp

moc_SuggestionDialogClass.cpp: ui_SuggestionDialogClass.h \
		SuggestionClass.h \
		enums.h \
		DeckClass.h \
		constants.h \
		BoardLocationClass.h \
		ClueMainWindowClass.h \
		ui_ClueMainWindowClass.h \
		CaseFileClass.h \
		ExceptionClass.h \
		PlayerClass.h \
		SuggestionDialogClass.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) SuggestionDialogClass.h -o moc_SuggestionDialogClass.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_AccusationDialogClass.h ui_ClueMainWindowClass.h ui_SuggestionDialogClass.h
compiler_uic_clean:
	-$(DEL_FILE) ui_AccusationDialogClass.h ui_ClueMainWindowClass.h ui_SuggestionDialogClass.h
ui_AccusationDialogClass.h: AccusationDialogClass.ui
	/usr/bin/uic-qt4 AccusationDialogClass.ui -o ui_AccusationDialogClass.h

ui_ClueMainWindowClass.h: ClueMainWindowClass.ui
	/usr/bin/uic-qt4 ClueMainWindowClass.ui -o ui_ClueMainWindowClass.h

ui_SuggestionDialogClass.h: SuggestionDialogClass.ui
	/usr/bin/uic-qt4 SuggestionDialogClass.ui -o ui_SuggestionDialogClass.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

AccusationDialogClass.o: AccusationDialogClass.cpp AccusationDialogClass.h \
		ui_AccusationDialogClass.h \
		SuggestionClass.h \
		enums.h \
		DeckClass.h \
		constants.h \
		BoardLocationClass.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o AccusationDialogClass.o AccusationDialogClass.cpp

BoardLocationClass.o: BoardLocationClass.cpp BoardLocationClass.h \
		enums.h \
		constants.h \
		ExceptionClass.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o BoardLocationClass.o BoardLocationClass.cpp

CaseFileClass.o: CaseFileClass.cpp CaseFileClass.h \
		enums.h \
		DeckClass.h \
		constants.h \
		BoardLocationClass.h \
		SuggestionClass.h \
		cardToSuspect.h \
		ExceptionClass.h \
		cardToWeapon.h \
		cardToRoom.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o CaseFileClass.o CaseFileClass.cpp

ClueMainWindowClass.o: ClueMainWindowClass.cpp ClueMainWindowClass.h \
		ui_ClueMainWindowClass.h \
		constants.h \
		BoardLocationClass.h \
		enums.h \
		CaseFileClass.h \
		DeckClass.h \
		SuggestionClass.h \
		ExceptionClass.h \
		PlayerClass.h \
		AccusationDialogClass.h \
		ui_AccusationDialogClass.h \
		SuggestionDialogClass.h \
		ui_SuggestionDialogClass.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ClueMainWindowClass.o ClueMainWindowClass.cpp

DeckClass.o: DeckClass.cpp DeckClass.h \
		constants.h \
		BoardLocationClass.h \
		enums.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o DeckClass.o DeckClass.cpp

ExceptionClass.o: ExceptionClass.cpp ExceptionClass.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ExceptionClass.o ExceptionClass.cpp

main.o: main.cpp ClueMainWindowClass.h \
		ui_ClueMainWindowClass.h \
		constants.h \
		BoardLocationClass.h \
		enums.h \
		CaseFileClass.h \
		DeckClass.h \
		SuggestionClass.h \
		ExceptionClass.h \
		PlayerClass.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

PlayerClass.o: PlayerClass.cpp PlayerClass.h \
		constants.h \
		BoardLocationClass.h \
		enums.h \
		ExceptionClass.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o PlayerClass.o PlayerClass.cpp

SuggestionClass.o: SuggestionClass.cpp SuggestionClass.h \
		enums.h \
		DeckClass.h \
		constants.h \
		BoardLocationClass.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o SuggestionClass.o SuggestionClass.cpp

SuggestionDialogClass.o: SuggestionDialogClass.cpp ClueMainWindowClass.h \
		ui_ClueMainWindowClass.h \
		constants.h \
		BoardLocationClass.h \
		enums.h \
		CaseFileClass.h \
		DeckClass.h \
		SuggestionClass.h \
		ExceptionClass.h \
		PlayerClass.h \
		SuggestionDialogClass.h \
		ui_SuggestionDialogClass.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o SuggestionDialogClass.o SuggestionDialogClass.cpp

moc_AccusationDialogClass.o: moc_AccusationDialogClass.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_AccusationDialogClass.o moc_AccusationDialogClass.cpp

moc_ClueMainWindowClass.o: moc_ClueMainWindowClass.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ClueMainWindowClass.o moc_ClueMainWindowClass.cpp

moc_SuggestionDialogClass.o: moc_SuggestionDialogClass.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_SuggestionDialogClass.o moc_SuggestionDialogClass.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:
