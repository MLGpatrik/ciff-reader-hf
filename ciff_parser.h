#ifndef CIFF_READER_HF_CIFF_PARSER_H
#define CIFF_READER_HF_CIFF_PARSER_H
#define MEMTRACE
#include <iostream>
#include <fstream>
#include <vector>
#include "parser.h"
#include <filesystem>
#include "memtrace.h"


using namespace std;
namespace fs = std::filesystem;

class CIFF_parser : public Parser{
private:
    std::vector<unsigned char>* buffer;
public:
    CIFF_parser(std::string path,std::vector<unsigned char>* buffer=NULL): Parser(path){
        this->buffer = buffer;
    }

    int search_for_newline(std::vector<unsigned char> buffer, int from=36){
        int counter=0;
        for(long unsigned int i=from;i<buffer.size();i++){
            if(buffer[i] == '\n'){
                break;
            }
            counter++;
        }
        return counter;
    }

    std::vector<unsigned char>* read_RGB_value(std::vector<unsigned char> buffer, int column, int row, int width, int offset){
        std::vector<unsigned char>* color_buffer = new std::vector<unsigned char>;
        if((long unsigned int)(offset + (row * width + column)*3 + 2) >= buffer.size()){
            std::cerr << "The summ is: " << (offset + (row * width + column)*3 + 2) << " The column is: " << column << " The row is: " << row << std::endl;
            throw std::out_of_range("Too long read!");
        }
        color_buffer->push_back(buffer[offset + (row * width + column) * 3]);        // R
        color_buffer->push_back(buffer[offset + (row * width + column) * 3 + 1]) ;   // G
        color_buffer->push_back(buffer[offset + (row * width + column) * 3 + 2]) ;   // B
        return color_buffer;
    }

    int write_jpeg_image(const char* filename, unsigned char* rgb_buffer, int width, int height);

    int parse() override{
        if(buffer == NULL){
            cout << "Parsing the file: " << this->path << endl;
            ifstream target_file_reader(this->path, std::ios::binary);
            this->buffer = new std::vector<unsigned char>(std::istreambuf_iterator<char>(target_file_reader), {});
        } else{
            cout << "Parsing CIFF image from CAFF file... " << endl;
        }
        string line;

        if(buffer->size() < 36){
            std::cerr << "Invalid file!" << std::endl;
            return -1;
        }

        // Read MAGIC
        std::string magic = this->read_magic(*buffer);
        std::cout << "The MAGIC: " << magic << " ";
        if(!(magic.compare("CIFF") == 0)){
            std::cout<< "Invalid file magic!" << std::endl;
            return -1;
        }
        std::cout<< "Valid file magic!" << std::endl;
        // Read header_size

        std::vector<unsigned char>* header_size_vector = this->read_header(*buffer,4,8);

        int header_size = this->ByteToInteger(*header_size_vector);
        std::cout << "Header size: " << header_size << std::endl;
        if(header_size < 36 || (long unsigned int) header_size >= buffer->size()){
            std::cerr << "Invalid header size!" << std::endl;
            return -1;
        }

        // Read content_size
        std::vector<unsigned char>* cont_size_vec = this->read_header(*buffer,12,8);
        int content_size = this->ByteToInteger(*cont_size_vec);
        std::cout << "The content size: " << content_size << std::endl;
        if(content_size < 0){
            std::cerr << "Invalid content size!" << std::endl;
            return -1;
        }

        // Read width
        std::vector<unsigned char>* width_vec = this->read_header(*buffer,20,8);
        int image_width = this->ByteToInteger(*width_vec);
        std::cout << "The width: " << image_width << std::endl;
        if(image_width < 0){
            std::cerr << "Invalid image width!" << std::endl;
            return -1;
        }

        // Read height

        std::vector<unsigned char>* height_vec = this->read_header(*buffer,28,8);
        int image_height = this->ByteToInteger(*height_vec);
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
        std::vector<unsigned char>* caption = this->read_header(*buffer,36,search_for_newline(*buffer,36));
        caption->push_back('\0');

        // Read tags

        std::vector<unsigned char>* tags = this->read_header(*buffer,36+caption->size(),header_size-36+caption->size()); // +1 is the \n it's not in the caption vector

        std::cout << "Tags first letter: " << tags->at(0) << std::endl;


        // Allocate memory for the RGB buffer
        std::cout << "The size of the file is: " << buffer->size() << std::endl;

        unsigned char* rgb_buffer = new unsigned char[image_width * image_height * 3];
        
        
        int y = 0;
        for (int x = 0; x < image_width; ++x) {                
            int index = (y * image_width + x) * 3;
            std::vector<unsigned char>* color_at_position = this->read_RGB_value(*buffer,x,y,image_width,header_size);
            rgb_buffer[index] = color_at_position->at(0);
            rgb_buffer[index + 1] = color_at_position->at(1);
            rgb_buffer[index + 2] = color_at_position->at(2);
            print_progress((float)x,(float)image_width);
            delete color_at_position;
        }
        print_progress((float)image_width,(float)image_width);

        std::cout << std::endl;
        std::cout << "Writing to file..." << std::endl;
        string filename = fs::path( this->path ).filename();
        filename.replace(filename.size()-4,4,"jpg");
        int ret_value = write_jpeg_image(filename.c_str(), rgb_buffer, image_width, image_height);

        delete[] rgb_buffer;
        buffer->clear();
        delete buffer;
        header_size_vector->clear();
        delete header_size_vector;
        cont_size_vec->clear();
        delete cont_size_vec;
        width_vec->clear();
        delete width_vec;
        height_vec->clear();
        delete height_vec;
        caption->clear();
        delete caption;
        tags->clear();
        delete tags;
        return ret_value;
    }


};

#endif //CIFF_READER_HF_CIFF_PARSER_H
