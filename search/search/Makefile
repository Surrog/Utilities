
CC=clang-3.5
CFLAGS=-std=c++11 -stdlib=libc++ -I../../../boost 
LDFLAGS=-std=c++11 -stdlib=libc++ -L../../../boost/stage/lib -lboost_filesystem -lboost_system -lc++ -lpthread
SOURCES=main.cpp search_task.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=xsearch

all: $(SOURCES) $(EXECUTABLE)

test:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f *.o *~
