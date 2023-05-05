CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++11
LIBS = -ljpeg

# Define the source files
SRC = ciff_parser.cpp parser.cpp reader.cpp
HDR = ciff_parser.h parser.h

# Define the object files
OBJ = $(SRC:.cpp=.o)

# Define the output executable
TARGET = parser

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(TARGET)

%.o: %.cpp $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)