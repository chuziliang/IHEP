CXX = g++
TARGET = main
SRC = $(wildcard *.cc)
OBJ = $(patsubst %.cc, %.o, $(SRC))

CXXFLAG = -c -Wall

find_package(OpenCV REQUIRED)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ `root-config --cflags --libs`

%.o: %.cc
	$(CXX) $(CXXFLAG) $< -o $@

.PHONY: clean
clean:
	rm -f *.o $(TARGET)
