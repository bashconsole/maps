#############################################################################
# Makefile for building mapsconfig
# Generated by tmake at 15:41, 2001/04/16
#     Project: mapsconfig
#    Template: app
#############################################################################

####### Compiler, tools and options
ifdef QTDIR
CC	=	gcc 
CXX	=	g++
CFLAGS	=	-pipe -Wall -W -O2 -DNO_DEBUG -DWITH_GUI
CXXFLAGS=	-pipe -Wall -W -O2 -DNO_DEBUG -DWITH_GUI
INCPATH	=	-I$(QTDIR)/include
LINK	=	g++
LFLAGS	=	
LIBS	=	$(SUBLIBS) -L$(QTDIR)/lib -L/usr/X11R6/lib -lqt -lXext -lX11 -lm
MOC	=	$(QTDIR)/bin/moc
UIC	=	$(QTDIR)/bin/uic
else
CC	=	gcc 
CXX	=	g++
CFLAGS	=	-pipe -Wall -W -O2 -DNO_DEBUG
CXXFLAGS=	-pipe -Wall -W -O2 -DNO_DEBUG
INCPATH	=	
LINK	=	g++
LFLAGS	=	
LIBS	=	$(SUBLIBS) -lm
endif

TAR	=	tar -cf 
GZIP	=	gzip -9f


####### Files
ifdef QTDIR
HEADERS =	createfile.h \
		dirview.h \
		mapsconf.h \
		mapswizard.h \
		param.h \
		setupdefaults.h
SOURCES =	compile.cpp \
		createfile.cpp \
		data.cpp \
		dirview.cpp \
		main.cpp \
		mapsconf.cpp \
		mapswizard.cpp \
		setupdefaults.cpp
OBJECTS =	compile.o \
		createfile.o \
		data.o \
		dirview.o \
		main.o \
		mapsconf.o \
		mapswizard.o \
		setupdefaults.o
INTERFACES =	
UICDECLS =	
UICIMPLS =	
SRCMOC	=	moc_dirview.cpp \
		moc_mapswizard.cpp
OBJMOC	=	moc_dirview.o \
		moc_mapswizard.o
else
HEADERS =	createfile.h \
		mapsconf.h \
		param.h \
		setupdefaults.h
SOURCES =	compile.cpp \
		createfile.cpp \
		data.cpp \
		main.cpp \
		mapsconf.cpp \
		setupdefaults.cpp
OBJECTS =	compile.o \
		createfile.o \
		data.o \
		main.o \
		mapsconf.o \
		setupdefaults.o
INTERFACES =	
UICDECLS =	
UICIMPLS =	
SRCMOC	=	
OBJMOC	=	
endif

DIST	=	
TARGET	=	mapsconfig
INTERFACE_DECL_PATH = .

####### Implicit rules

.SUFFIXES: .cpp .cxx .cc .C .c

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules


all: $(TARGET)

$(TARGET): $(UICDECLS) $(OBJECTS) $(OBJMOC) 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(LIBS)

moc: $(SRCMOC)

dist:
	$(TAR) mapsconfig.tar $(SOURCES) $(HEADERS) $(INTERFACES) $(DIST) help/
	$(GZIP) mapsconfig.tar

clean:
	-rm -f $(OBJECTS) $(OBJMOC) $(SRCMOC) $(UICIMPLS) $(UICDECLS) $(TARGET)
	-rm -f *~ core

####### Sub-libraries


###### Combined headers


####### Compile

compile.o: compile.cpp \
		param.h

createfile.o: createfile.cpp \
		createfile.h

data.o: data.cpp \
		param.h

dirview.o: dirview.cpp \
		dirview.h

ifdef QTDIR
main.o: main.cpp \
		mapswizard.h \
		dirview.h \
		mapsconf.h \
		param.h \
		setupdefaults.h
else
main.o: main.cpp \
		mapsconf.h \
		param.h \
		setupdefaults.h
endif

mapsconf.o: mapsconf.cpp \
		mapsconf.h \
		createfile.h \
		param.h

mapswizard.o: mapswizard.cpp \
		mapswizard.h \
		dirview.h \
		createfile.h \
		param.h

setupdefaults.o: setupdefaults.cpp \
		setupdefaults.h \
		param.h

moc_dirview.o: moc_dirview.cpp \
		dirview.h

moc_mapswizard.o: moc_mapswizard.cpp \
		mapswizard.h \
		dirview.h

moc_dirview.cpp: dirview.h
	$(MOC) dirview.h -o moc_dirview.cpp

moc_mapswizard.cpp: mapswizard.h
	$(MOC) mapswizard.h -o moc_mapswizard.cpp

