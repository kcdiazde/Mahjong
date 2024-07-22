# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Target executable
TARGET = Mahjong

# Source files
SRCS = mahjong.cc tiles.cc player.cc
HDRS = tiles.h player.h mahjong_common.h

# Object files
OBJS = $(SRCS:.cc=.o)

# Default target
all: $(TARGET)

# Rule to link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile the source files into object files
%.o: %.cc HDRS
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up the build files
clean:
	rm -f $(OBJS) $(TARGET)

# Rule to run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets to avoid conflicts with files of the same name
.PHONY: all clean run