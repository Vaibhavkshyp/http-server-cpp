# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Linker Flags
LDFLAGS = -lws2_32

# Executable Name
TARGET = server

# Source Files
SRC = \
	src/main.cpp \
	src/HttpServer.cpp \
	src/HttpRequestParser.cpp \
	src/HttpResponse.cpp \
	src/StaticFileHandler.cpp

# Object Files
OBJ = $(SRC:.cpp=.o)

# Default Target
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run
run: $(TARGET)
	./$(TARGET)

# Clean
clean:
	rm -f src/*.o server server.exe

.PHONY: all run clean