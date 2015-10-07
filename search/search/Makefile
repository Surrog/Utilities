
CC=g++
CFLAGS=-std=c++11 -g3
LDFLAGS=-std=c++11  -lboost_filesystem -lboost_system -lpthread
SOURCES=main.cpp search_task.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=xsearch

all: $(SOURCES) $(EXECUTABLE)

test:
	$(CC) $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $(EXECUTABLE)

re: clean all

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f *.o *~
