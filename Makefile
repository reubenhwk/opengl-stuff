
# sudo apt install libglfw3-dev libfreetype-dev pkgconf

.PHONY: clean

TARGETS=gl test1
CFLAGS=-g -std=gnu99
CFLAGS+=$(shell pkg-config --cflags freetype2)
LDFLAGS=
LIBS=-lglfw -lGL
LIBS+=$(shell pkg-config --libs freetype2)

all: $(TARGETS)

test1: test1.o mrfont.o model.o draw.o
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

gl: gl.o mrfont.o model.o draw.o
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

%.o: %.c
	$(CC) $< -o $@ -c $(CFLAGS)

clean:
	rm -f *.o $(TARGETS)
