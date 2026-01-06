CC = cc
CFLAGS = -g -Iinclude -fPIC `pkg-config --cflags zlib`
LDFLAGS = -shared
LIBS = `pkg-config --libs zlib`

OBJS = src/client.o src/server.o src/socket.o src/stb_ds.o

LIB = lib

SO = .so

.PHONY: all format clean
.SUFFIXES: .c .o

all: $(LIB)fishsoup$(SO)

format:
	clang-format --verbose -i `find src include "(" -name "*.c" -or -name "*.h" ")" -and -not -name "stb_*.h"` test*.c

$(LIB)fishsoup$(SO): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

testclient: testclient.c $(LIB)fishsoup$(SO)
	cc -o $@ testclient.c -Wl,-R./ -L./ -Iinclude -lfishsoup

testserver: testserver.c $(LIB)fishsoup$(SO)
	cc -o $@ testserver.c -Wl,-R./ -L./ -Iinclude -lfishsoup

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *$(SO) $(OBJS) ./testserver ./testclient
