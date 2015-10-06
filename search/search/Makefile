
CC=clang-3.5
CFLAGS=-std=c++11 -g3 -stdlib=libc++
LDFLAGS=-std=c++11 -lboost_filesystem -lboost_system -lpthread -lc++
SOURCES=main.cpp search_task.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=xsearch

all: $(SOURCES) $(EXECUTABLE)

re: clean all

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f *.o *~
