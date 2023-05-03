#ifndef CIFF_READER_HF_CIFF_PARSER_H
#define CIFF_READER_HF_CIFF_PARSER_H
#include <iostream>
#include <fstream>
#include <vector>
#include "parser.h"

using namespace std;

class CIFF_parser : public Parser{
public:
    CIFF_parser(std::string path): Parser(path){
    }

    int parse() override{
        cout << this->path << endl;
        ifstream target_file_reader(this->path, std::ios::binary);
        //std::ofstream output( "C:\\myfile.gif", std::ios::binary );
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(target_file_reader), {});
        string line;
        return 0;
    }
};


#endif //CIFF_READER_HF_CIFF_PARSER_H
