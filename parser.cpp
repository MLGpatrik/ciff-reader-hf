#include "parser.h"
#include<fstream>

using namespace std;

Parser::Parser(std::string path) {
    this->path = path;
}

std::string Parser::read_magic(std::vector<unsigned char> buffer){
    char* magic_buffer = new char[5];
    for(int i=0;i<4;i++){
        magic_buffer[i]=buffer[i];
    }
    magic_buffer[4]='\0';
    std::string the_magic = magic_buffer;
    return the_magic;
}

std::vector<unsigned char>* Parser::read_header(std::vector<unsigned char> buffer,int from,int count){
    if((long unsigned int)(from+count) >= buffer.size()){
        throw new std::out_of_range("Too long read!");
    }
    std::vector<unsigned char>* header_buffer = new std::vector<unsigned char>;
    for(int i=from;i<from+count;i++){
        header_buffer->push_back(buffer[i]);
    }
    return header_buffer;
}