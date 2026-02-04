CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# Source files
SOURCES = main.cpp rtweekend.h

# Target executable
TARGET = inOneWeekend.exe

all: $(TARGET)

# Build target from sources
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Clean build artifacts
clean:
	del $(TARGET)

# run: mingw32-make
#     ./inOneWeekend.exe >> image.ppm