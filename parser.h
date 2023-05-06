#ifndef CIFF_READER_HF_PARSER_H
#define CIFF_READER_HF_PARSER_H
#define MEMTRACE
#include <iostream>
#include <vector>
#include <algorithm>
#include "memtrace.h"

class Parser {
protected:
    std::string path; // Path to the target file

    /**
     * Reads the magic of the file
     * @param buffer The buffer which contains the binary file
     * @return Returns the magic
     */
    std::string read_magic(std::vector<unsigned char> buffer);

    /**
     * Reads the header from the given index to the count
     * @param buffer The buffer which contains the file
     * @param from Read from what byte (counting from 0.)
     * @param count How many bytes to read
     * @return The byte vector, which contains the desired bytes
     */
    std::vector<unsigned char>* read_header(std::vector<unsigned char> buffer,int from,int count);

    /**
     * Converts the hexadecimal numbers to integer
     * @param buffer The buffer which holds the bytes
     * @return The value of the buffer
     */
    int ByteToInteger(std::vector<unsigned char> buffer){
        unsigned int counter = 0;
        std::reverse(buffer.begin(), buffer.end());
        for (long unsigned int i = 0; i < buffer.size(); i++) {
            counter = (counter << 8) | buffer.at(i);
        }
        return counter;
    }

    /**
     * Prints a progressbar
     * @param progress The current progress value
     * @param max_progress The maximum progress value (where the 100% is)
     */
    void print_progress(float progress,float max_progress){
        int barWidth = 60;
        std::cout << "[";
        int pos = barWidth * (progress/max_progress);
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << ((progress/max_progress)* 100.0) << " %           \r";
        std::cout.flush();
    }
public:
    /**
     * The constructor reads the file and saves the file handler in a variable
     * @param path The path to the CAFF or CIFF file
     */
    Parser(std::string path);

    /**
     * Reads picture data from the file and converts it to a jpeg
     * @return If the return value is 0 then the parsing was successful, else it was unsuccessful
     */
    virtual int parse() = 0;
};


#endif //CIFF_READER_HF_PARSER_H
