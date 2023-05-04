#ifndef CIFF_READER_HF_PARSER_H
#define CIFF_READER_HF_PARSER_H
#include <iostream>
#include <vector>

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

        for (int i = 0; i < buffer.size(); i++) {
            counter = (counter << 8) | buffer.at(i);
        }
        return counter;
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
