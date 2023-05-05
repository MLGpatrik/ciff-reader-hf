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
            throw new std::out_of_range("Too long read!");
        }
        color_buffer->push_back(buffer[offset + (row * width + column) * 3]);        // R
        color_buffer->push_back(buffer[offset + (row * width + column) * 3 + 1]) ;   // G
        color_buffer->push_back(buffer[offset + (row * width + column) * 3 + 2]) ;   // B
        return color_buffer;
    }

    //void write_jpeg_file(unsigned char* rgb_data, int width, int height, const char* filename, int quality);
    int write_jpeg_image(const char* filename, unsigned char* rgb_buffer, int width, int height, int quality);


    void print_progress(float progress,float max_progress){
        int barWidth = 70;

            std::cout << "[";
            int pos = barWidth * (progress/max_progress);
            for (int i = 0; i < barWidth; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else std::cout << " ";
            }
            std::cout << "] " << ((progress/max_progress)* 100.0) << " %           \r";
            std::cout.flush();
            //std::cout << std::endl;
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
        if(header_size < 36 || (long unsigned int) header_size >= buffer.size()){
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
        std::vector<unsigned char>* caption = this->read_header(buffer,36,search_for_newline(buffer,36));
        caption->push_back('\0');

        // Read tags

        std::vector<unsigned char>* tags = this->read_header(buffer,36+caption->size(),header_size-36+caption->size()); // +1 is the \n it's not in the caption vector

        std::cout << "Tags first letter: " << tags->at(0) << std::endl;


        // Allocate memory for the RGB buffer
        std::cout << "The size of the file is: " << buffer.size() << std::endl;

        unsigned char* rgb_buffer = new unsigned char[image_width * image_height * 3];
        
        
        int y = 0;
        for (int x = 0; x < image_width; ++x) {                
            int index = (y * image_width + x) * 3;
            std::vector<unsigned char>* color_at_position = this->read_RGB_value(buffer,x,y,image_width,header_size);
            rgb_buffer[index] = color_at_position->at(0);
            rgb_buffer[index + 1] = color_at_position->at(1);
            rgb_buffer[index + 2] = color_at_position->at(2);
              
            print_progress((float)x,(float)image_width);    
                //std::cout << "The first character: " << color_at_position->at(0) << std::endl;
                //break;
        }                        
        
            //break; //TODO: remove this
        //}

        std::cout << std::endl;
        std::cout << "Writing to file..." << std::endl;
        int ret_value = write_jpeg_image("output3.jpg", rgb_buffer, image_width, image_height, 90);

        delete[] rgb_buffer;



        //header_size_vector = null;
        //TODO: filename same as the original filename + jpg
        //TODO: make file read a separate thing to make work it with CAFF
        //TODO: delte unused stuff
        return ret_value;
    }


};

#endif //CIFF_READER_HF_CIFF_PARSER_H
