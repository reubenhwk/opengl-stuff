
.PHONY: clean

TARGETS=gl
CFLAGS=-g -std=gnu99
CFLAGS+=$(shell pkg-config --cflags freetype2)
LDFLAGS=
LIBS=-lglfw -lGL
LIBS+=$(shell pkg-config --libs freetype2)

gl: gl.o mrfont.o
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

%.o: %.c
	$(CC) $< -o $@ -c $(CFLAGS)

clean:
	rm -f *.o $(TARGETS)
