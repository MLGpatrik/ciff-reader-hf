CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++20
LIBS = -ljpeg
# CPPFLAGS += -DMEMTRACE This is for MEMTRACE, but you have to also add memtrace.h and memtrace.cpp to SRC and HDR, and have to import the header file everywhere
# Define the source files
SRC = ciff_parser.cpp parser.cpp reader.cpp caff_parser.cpp
HDR = ciff_parser.h parser.h caff_parser.h

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
