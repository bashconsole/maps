CC = gcc
INCLUDES = -I../include 
LIBS = -lm   # -lsocket -lnsl

PROG = mapsconf
OBJS = $(PROG).o \
main.o  \
createfile.o \
compile.o \
data.o \
setupdefaults.o



all:: $(PROG)

.cc.o:
	$(CC) -c -g $(INCLUDES) $<

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

clean::
	rm -f $(PROG) $(OBJS)
