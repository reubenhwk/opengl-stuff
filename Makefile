
.PHONY: clean

TARGETS=gl
CFLAGS=-g -std=gnu99
LDFLAGS=
LIBS=-lglfw -lGL

gl: gl.o
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

%.o: %.c
	$(CC) $< -o $@ -c $(CFLAGS)

clean:
	rm -f *.o $(TARGETS)
