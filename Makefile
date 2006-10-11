TARGET=kwingrid

MOCS=\
kwingrid.moc

OBJECTS=\
main.o \
kwingrid.o \
kwingrid_iface_skel.o \
kwingrid_iface_stub.o 

LIBS=-lkdecore -lkdeui -lDCOP -lqt-mt -lX11 -lXext
LOPTS=-L/usr/X11R6/lib -g
COPTS=-I. -I /usr/include/kde -I /usr/include/qt3 -I /usr/X11R6/include/X11 -O2 -Wall -g

all: mocs bin

clean:
	rm -f $(OBJECTS) $(MOCS) *_skel.cpp *_stub.h *_stub.cpp $(TARGET)

bin: $(TARGET)

mocs: $(MOCS)

%.moc: %.h
	moc $< -o $@

%.o: %.cc
	g++ $(COPTS) -c $< -o $@

%.o: %.cpp
	g++ $(COPTS) -c $< -o $@

%.idl: %.h
	dcopidl $< > $@

%_skel.cpp %_stub.h %_stub.cpp: %.idl
	dcopidl2cpp $<

$(TARGET): $(OBJECTS)
	g++ $(LOPTS) -o $@ $(OBJECTS) $(LIBS)


