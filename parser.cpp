#include "parser.h"
#include <fstream>

using namespace std;

Parser::Parser(std::string path) {
    this->path = path;
}

std::string Parser::read_magic(std::vector<unsigned char> buffer,int from){
    char* magic_buffer = new char[5];
    for(int i=from;i<from+4;i++){
        magic_buffer[i]=buffer[i];
    }
    magic_buffer[4]='\0';
    std::string the_magic = magic_buffer;
    delete[] magic_buffer;
    return the_magic;
}

std::vector<unsigned char>* Parser::read_header(std::vector<unsigned char> buffer,unsigned int from,unsigned int count){
    if((long unsigned int)(from+count) > buffer.size()){
        throw std::out_of_range("Too long read!");
    }
    std::vector<unsigned char>* header_buffer = new std::vector<unsigned char>;
    for(unsigned int i=from;i<from+count;i++){
        header_buffer->push_back(buffer[i]);
    }
    return header_buffer;
}

int Parser::ByteToInteger(std::vector<unsigned char> buffer){
    unsigned int counter = 0;
    std::reverse(buffer.begin(), buffer.end());
    for (long unsigned int i = 0; i < buffer.size(); i++) {
        counter = (counter << 8) | buffer.at(i);
    }
    return counter;
}

void Parser::print_progress(float progress,float max_progress){
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

void Parser::clear_buffer_pointer(std::vector<unsigned char>* buffer){
    buffer->clear();
    delete buffer;
    buffer = NULL;
}