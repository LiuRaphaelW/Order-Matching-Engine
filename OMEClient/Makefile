# Makefile for the Client Project
CXX = g++
CXXFLAGS = -std=c++23 -Wall -I../quickfix/include
LDFLAGS = -L../quickfix/include -lquickfix

# Executable name
CLIENT = OMEClient

# Source files
SRC = OMEClient.cpp Application.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(CLIENT)

# Build the client executable
$(CLIENT): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean up object and executable files
clean:
	rm -f *.o $(CLIENT)
