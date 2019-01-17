
.PHONY: clean

TARGETS=pm gl
CFLAGS=-g -std=gnu99
CFLAGS+=$(shell pkg-config --cflags freetype2)
LDFLAGS=
LIBS=-lglfw -lGL
LIBS+=$(shell pkg-config --libs freetype2)

all: $(TARGETS)

pm: model.o walk_model.o
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

gl: gl.o mrfont.o model.o
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

%.o: %.c
	$(CC) $< -o $@ -c $(CFLAGS)

clean:
	rm -f *.o $(TARGETS)
