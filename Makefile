all: findFiles

CXX = clang++
CXXFLAGS = -g -Wall
OBJECTS = Element.o BencodeParser.o main.o

findFiles: $(OBJECTS)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJECTS)

clean:
	-rm -f findFiles *.o
