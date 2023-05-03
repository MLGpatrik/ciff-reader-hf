#ifndef CIFF_READER_HF_PARSER_H
#define CIFF_READER_HF_PARSER_H
#include <iostream>

class Parser {
protected:
    std::string path; // Path to the target file

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
