#ifndef CIFF_READER_HF_CAFF_PARSER_H
#define CIFF_READER_HF_CAFF_PARSER_H
#include "parser.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "ciff_parser.h"
#define ONE_IMAGE

using namespace std;

class CAFF_parser : public Parser{
private:
    int next = 0;
    int num_anim = -1;

    /**
     * Reads the block at 'from' and return the data part of the block
     * @param buffer The buffer which stores the block(s)
     * @param from The index where the block starts
     * @return Returns the block's data part in a std::vector
     */
    std::vector<unsigned char>* get_block(std::vector<unsigned char> buffer,int from);

    /**
     * Reads the ID of the block
     * @param buffer The buffer which stores the block(s)
     * @param from The index where the block starts
     * @return Returns the ID of the block
     */
    int get_block_id(std::vector<unsigned char> buffer,int from);

    /**
     * Read the length of the block
     * @param buffer The buffer which stores the block(s)
     * @param from The index where the block starts
     * @return The length of the block (in bytes)
     */
    int get_block_length(std::vector<unsigned char> buffer,int from);

    /**
     * Reads the block's data
     * @param buffer The buffer which stores the block(s)
     * @param from The index where the block starts
     * @param length The length of the block
     * @return Returns the block's data part
     */
    std::vector<unsigned char>* get_block_data(std::vector<unsigned char> buffer,int from,int length);

    /**
     * Parses the header block
     * @param buffer The buffer which holds the header block
     * @return Returns -1 if the parsing failed returns 0 if successful
     */
    int parse_caff_header(std::vector<unsigned char> buffer);

    /**
     * Parses the credits block
     * @param buffer The buffer which holds the credits block
     * @return Returns -1 if the parsing failed returns 0 if successful
     */
    int parse_credits(std::vector<unsigned char> buffer);


    /**
     * Read the block from the buffer and runs parsing_func with the block as a parameter
     * @param buffer The buffer which contains the whole CAFF file
     * @param parsing_func The function which does the block parsing
     * @return Returns 0 if parsing and reading was successful and return -1 if it failed
     */
    int parse_block(std::vector<unsigned char> buffer, int (CAFF_parser::*parsing_func)(std::vector<unsigned char>));
public:
    CAFF_parser(std::string path): Parser(path){
    }

    int parse() override{
        cout << "Parsing the file: " << this->path << endl;
        ifstream target_file_reader(this->path, std::ios::binary);
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(target_file_reader), {});
        if(buffer.size() < 87){
            std::cerr << "Invalid file!" << std::endl;
            return -1;
        }
        try {
            if (parse_block(buffer, &CAFF_parser::parse_caff_header) == -1) {
                return -1;
            }
            if (parse_block(buffer, &CAFF_parser::parse_credits) == -1) {
                return -1;
            }
        }catch (std::out_of_range const&){
            buffer.clear();
            std::cerr << "Invalid block(s), exiting...";
            return -1;
        }

        std::vector<unsigned char>* data = NULL;
        int ret_val = 0;
        for(int i = 0; i < num_anim; i++){
            if((unsigned int)next < buffer.size()){
                try{
                    data = get_block(buffer,next);
                }catch (std::out_of_range const&){
                    buffer.clear();
                    return -1;
                }
                std::cout << "The next val is: " << next << std::endl;
                std::vector<unsigned char>* ciff_block = read_header(*data,8,data->size()-8);
                next+=data->size()+9;
                clear_buffer_pointer(data);
                if(i != 0){
                    std::string path_indexed(this->path);
                    path_indexed.replace(this->path.size()-5,6,std::to_string(i)+".caff");
                    CIFF_parser ciff_p(path_indexed,ciff_block);
                    ret_val = ciff_p.parse();
                } else{
                    CIFF_parser ciff_p(this->path,ciff_block);
                    ret_val = ciff_p.parse();
                }



            } else{
                buffer.clear();
                return -1;
            }
            if( ret_val == -1){
                break;
            }
#ifdef ONE_IMAGE
            break;
#endif
        }
        buffer.clear();
        return ret_val;
    }

};


#endif //CIFF_READER_HF_CAFF_PARSER_H
