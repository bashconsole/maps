CC = gcc
INCLUDES = -I/usr/include/rhtvision
LIBS = /usr/lib/librhtv.a -lstdc++ -lncurses -lgpm -lm
#-lrthv  # -lsocket -lnsl
PROG = mapsconf

OBJS = $(PROG).o \
compile.o \
setupdefaults.o \
main.o \
data.o \
createfile.o \
wizardapp.o \
wizardlg.o \
dirdlg.o



all:: $(PROG) 

.cc.o:
	$(CC) -c -g $(INCLUDES) $<

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

clean::
	rm -f $(PROG) $(OBJS) 

      
