#ifndef CIFF_READER_HF_CIFF_PARSER_H
#define CIFF_READER_HF_CIFF_PARSER_H
#include <iostream>
#include <fstream>
#include <vector>
#include "parser.h"
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class CIFF_parser : public Parser{
private:
    std::vector<unsigned char>* buffer;
public:
    explicit CIFF_parser(std::string path,std::vector<unsigned char>* buffer=NULL): Parser(path){
        this->buffer = buffer;
    }

    /**
     * Reads byte from the buffer until it finds a newline character (\n)
     * @param buffer The buffer which stores the characters
     * @param from Reads the characters from this index
     * @return The std::vector which holds the characters
     */
    int search_for_newline(std::vector<unsigned char> buffer, int from=36);

    /**
     * Reads RGB value for the pixel
     * @param buffer The buffer which holds the RGB values
     * @param column The pixel's x coordinate
     * @param row  The pixel's y coordinate
     * @param width The width of the image
     * @param offset The offset of the RGB values in the buffer
     * @return The std::vector which hold the RGB values for the pixel (The order is Red Green Blue)
     */
    std::vector<unsigned char>* read_RGB_value(std::vector<unsigned char> buffer, int column, int row, int width, int offset);

    /**
     * Writes the parsed image to  JPEG file
     * @param filename The name of the file
     * @param rgb_buffer The buffer which holds the RGB values
     * @param width The width of the image
     * @param height The height of the image
     * @return If 0 the write the write was successful otherwise it failed
     */
    int write_jpeg_image(const char* filename, unsigned char* rgb_buffer, int width, int height);

    int parse() override{
        if(buffer == NULL){
            cout << "Parsing the file: " << this->path << endl;
            ifstream target_file_reader(this->path, std::ios::binary);
            this->buffer = new std::vector<unsigned char>(std::istreambuf_iterator<char>(target_file_reader), {});
        } else{
            cout << "Parsing CIFF image from CAFF file... " << endl;
        }

        if(buffer->size() < 36){
            clear_buffer_pointer(buffer);
            std::cerr << "Invalid file!" << std::endl;
            return -1;
        }

        // Read MAGIC
        std::string magic = this->read_magic(*buffer);
        std::cout << "The MAGIC: " << magic << " ";
        if(!(magic.compare("CIFF") == 0)){
            clear_buffer_pointer(buffer);
            std::cout<< "Invalid file magic!" << std::endl;
            return -1;
        }
        std::cout<< "Valid file magic!" << std::endl;
        // Read header_size

        std::vector<unsigned char>* header_size_vector = this->read_header(*buffer,4,8);
        int header_size = this->ByteToInteger(*header_size_vector);
        clear_buffer_pointer(header_size_vector);
        std::cout << "Header size: " << header_size << std::endl;
        if(header_size < 36 || (long unsigned int) header_size >= buffer->size()){
            clear_buffer_pointer(buffer);
            std::cerr << "Invalid header size!" << std::endl;
            return -1;
        }

        // Read content_size
        std::vector<unsigned char>* cont_size_vec = this->read_header(*buffer,12,8);
        int content_size = this->ByteToInteger(*cont_size_vec);
        clear_buffer_pointer(cont_size_vec);
        std::cout << "The content size: " << content_size << std::endl;
        if(content_size < 0){
            clear_buffer_pointer(buffer);
            std::cerr << "Invalid content size!" << std::endl;
            return -1;
        }

        // Read width
        std::vector<unsigned char>* width_vec = this->read_header(*buffer,20,8);
        int image_width = this->ByteToInteger(*width_vec);
        clear_buffer_pointer(width_vec);
        std::cout << "The width: " << image_width << std::endl;
        if(image_width < 0){
            clear_buffer_pointer(buffer);
            std::cerr << "Invalid image width!" << std::endl;
            return -1;
        }

        // Read height
        std::vector<unsigned char>* height_vec = this->read_header(*buffer,28,8);
        int image_height = this->ByteToInteger(*height_vec);
        clear_buffer_pointer(height_vec);
        std::cout << "The height: " << image_height << std::endl;
        if(image_height < 0){
            clear_buffer_pointer(buffer);
            std::cerr << "Invalid image width!" << std::endl;
            return -1;
        }

        if(!(content_size == 3*image_height*image_width)){
            clear_buffer_pointer(buffer);
            std::cerr << "Content size doesn't match with dimensions!" << std::endl;
            return -1;
        }

        // Read caption
        std::vector<unsigned char>* caption = this->read_header(*buffer,36,search_for_newline(*buffer,36));
        caption->push_back('\0');
        std::cout << "The caption is: " << std::string(caption->begin(), caption->end()) << std::endl;
        // Read tags

        std::vector<unsigned char>* tags = this->read_header(*buffer,(unsigned int)((unsigned int)36+caption->size()),(unsigned int)(header_size-(unsigned int)36-caption->size())); // +1 is the \n it's not in the caption vector
        clear_buffer_pointer(caption);
        std::replace(tags->begin(),tags->end(),'\0',',');
        tags->push_back('\0');
        std::cout << "Tags: " << std::string(tags->begin(), tags->end()) << std::endl;
        clear_buffer_pointer(tags);

        // Allocate memory for the RGB buffer
        std::cout << "The size of the file is: " << buffer->size() << std::endl;

        if((unsigned int)(header_size+content_size) > buffer->size()){
            std::cerr << "Header size + content size bigger than buffer size!" << std::endl;
            clear_buffer_pointer(buffer);
            return -1;
        }

        std::vector<unsigned char>* rgb = read_header(*buffer,header_size,content_size);

        std::cout << "Writing to file..." << std::endl;
        string filename = fs::path( this->path ).filename();
        if((filename.find(string(".ciff")) != std::string::npos) || (filename.find(string(".caff")) != std::string::npos)){
            filename.replace(filename.size()-4,4,"jpg");
        }else{
            filename.append(string(".jpg"));
        }

        int ret_value = write_jpeg_image(filename.c_str(), &(rgb->at(0)), image_width, image_height);
        clear_buffer_pointer(buffer);
        return ret_value;
    }

};

#endif //CIFF_READER_HF_CIFF_PARSER_H
