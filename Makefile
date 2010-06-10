TARGET=kwingrid

MOCS=\
kwingrid.moc

OBJECTS=\
main.o \
kwingrid.o \

LIBS=-lkdecore -lkdeui -lQtCore -lQtGui -lX11 -lXext
LOPTS=-L/usr/X11R6/lib -g
COPTS=-I. -I /usr/include/kde -I /usr/include/qt4 -I /usr/include/X11 -O2 -Wall -g

all: mocs bin

clean:
	rm -f $(OBJECTS) $(MOCS) $(TARGET)

bin: $(TARGET)

mocs: $(MOCS)

%.moc: %.h
	moc $< -o $@

%.o: %.cc
	g++ $(COPTS) -c $< -o $@

%.o: %.cpp
	g++ $(COPTS) -c $< -o $@

$(TARGET): $(OBJECTS)
	g++ $(LOPTS) -o $@ $(OBJECTS) $(LIBS)
