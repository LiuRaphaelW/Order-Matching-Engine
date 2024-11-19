CXX = g++
CXXFLAGS = -std=c++23 -Wall -I../quickfix/include
LDFLAGS = -L../quickfix/include -lquickfix

SERVER = OMEProgram

SRC = Order.h OMEProgram.cpp Application.cpp Market.h OrderMatcher.h Market.cpp IDGenerator.h

OBJ = $(SRC:.cpp=.o)

all: $(SERVER)

$(SERVER): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

 
# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean up object and executable files
clean:
	rm -f *.o $(SERVER)

