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
        cout << "Parsing the file: " << this->path << endl;
        ifstream target_file_reader(this->path, std::ios::binary);
        //std::ofstream output( "C:\\myfile.gif", std::ios::binary );
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(target_file_reader), {});
        string line;

        if(buffer.size() < 36){
            std::cerr << "Invalid file!" << std::endl;
            return -1;
        }

        // Read MAGIC
        std::string magic = this->read_magic(buffer);
        std::cout << "The MAGIC: " << magic << " ";
        if(!(magic.compare("CIFF") == 0)){
            std::cout<< "Invalid file magic!" << std::endl;
            return -1;
        }
        std::cout<< "Valid file magic!" << std::endl;
        // Read header_size

        std::vector<unsigned char>* header_size_vector = this->read_header(buffer,4,8);

        int header_size = this->ByteToInteger(*header_size_vector);
        std::cout << "Header size: " << header_size << std::endl;
        if(header_size < 36 || header_size >= buffer.size()){
            std::cerr << "Invalid header size!" << std::endl;
            return -1;
        }

        // Read content_size

        int content_size = this->ByteToInteger(*this->read_header(buffer,12,8));
        std::cout << "The content size: " << content_size << std::endl;
        if(content_size < 0){
            std::cerr << "Invalid content size!" << std::endl;
            return -1;
        }

        // Read width

        int image_width = this->ByteToInteger(*this->read_header(buffer,20,8));
        std::cout << "The width: " << image_width << std::endl;
        if(image_width < 0){
            std::cerr << "Invalid image width!" << std::endl;
            return -1;
        }

        // Read height


        int image_height = this->ByteToInteger(*this->read_header(buffer,28,8));
        std::cout << "The height: " << image_height << std::endl;
        if(image_height < 0){
            std::cerr << "Invalid image width!" << std::endl;
            return -1;
        }

        if(!(content_size == 3*image_height*image_width)){
            std::cerr << "Content size doesn't match with dimensions!" << std::endl;
            return -1;
        }
        // Read caption

        // Read tags

        // Read image?
        //TODO: make file read a seperate thing to make work it with CAFF
        return 0;
    }
};

std::vector<unsigned char>* readCaption(std::vector<unsigned char> buffer){

}

#endif //CIFF_READER_HF_CIFF_PARSER_H
