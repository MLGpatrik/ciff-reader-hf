#ifndef CIFF_READER_HF_CAFF_PARSER_H
#define CIFF_READER_HF_CAFF_PARSER_H
#define MEMTRACE
#include "memtrace.h"
#include "parser.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "ciff_parser.h"

using namespace std;

class CAFF_parser : public Parser{
private:
    std::vector<unsigned char>* get_block(std::vector<unsigned char> buffer,int from){
        get_block_id(buffer,from);
        int length = get_block_length(buffer,from);
        return get_block_data(buffer,from,length);
    }

    int get_block_id(std::vector<unsigned char> buffer,int from){
        std::vector<unsigned char>* id_vec = read_header(buffer,from,1);
        int id = ByteToInteger(*id_vec);
        id_vec->clear();
        delete id_vec;
        std::cout << "The ID: " << id << std::endl;

        if(id < 0 || 4 <= id){
            throw std::out_of_range("Invalid ID");
        }
        return id;
    }
    int get_block_length(std::vector<unsigned char> buffer,int from){
        std::vector<unsigned char>* length_vec = read_header(buffer,from+1,8);
        int data_size = ByteToInteger(*length_vec);
        length_vec->clear();
        delete length_vec;
        std::cout << "The length is: " << data_size << std::endl;

        if(data_size < 0 || (buffer.size()) < data_size){
            throw std::out_of_range("Invalid data length!");
        }
        return data_size;
    }
    std::vector<unsigned char>* get_block_data(std::vector<unsigned char> buffer,int from,int length){
        if(buffer.size() < from+length){
            throw std::out_of_range("Too long block read!");
        }
        return read_header(buffer,from+9,length);
    }

    int parse_caff_header(std::vector<unsigned char> buffer){
        std::string magic = read_magic(buffer);
        std::cout << "The MAGIC: " << magic << " ";
        if(!(magic.compare("CAFF") == 0)){
            std::cout<< "Invalid file magic!" << std::endl;
            return -1;
        }
        std::cout<< "Valid file magic!" << std::endl;

        std::vector<unsigned char>* header_size_vector = this->read_header(buffer,4,8);

        int header_size = this->ByteToInteger(*header_size_vector);
        header_size_vector->clear();
        delete header_size_vector;
        std::cout << "Header size: " << header_size << std::endl;
        if(header_size < 0 || (long unsigned int) header_size > buffer.size()){
            std::cerr << "Invalid header size!" << std::endl;
            return -1;
        }

        std::vector<unsigned char>* num_anim_vec = this->read_header(buffer,12,8);
        int num_anim = this->ByteToInteger(*num_anim_vec);
        num_anim_vec->clear();
        delete num_anim_vec;
        std::cout << "Number of animations in the file: " << num_anim << std::endl;
        if(num_anim < 1){
            std::cerr << "Invalid animation number!" << std::endl;
            return -1;
        }
        return num_anim;
    }
    void parse_credits(std::vector<unsigned char> buffer){
        std::vector<unsigned char>* year_vec = this->read_header(buffer,0,2);
        int yy = ByteToInteger(*year_vec);
        year_vec->clear();
        delete year_vec;
        std::vector<unsigned char>* month_vec = this->read_header(buffer,2,1);
        int m = ByteToInteger(*month_vec);
        month_vec->clear();
        delete month_vec;
        std::vector<unsigned char>* day_vec = this->read_header(buffer,3,1);
        int d = ByteToInteger(*day_vec);
        day_vec->clear();
        delete day_vec;
        std::vector<unsigned char>* hour_vec = this->read_header(buffer,4,1);
        int h = ByteToInteger(*hour_vec);
        hour_vec->clear();
        delete hour_vec;
        std::vector<unsigned char>* minute_vec = this->read_header(buffer,5,1);
        int min = ByteToInteger(*minute_vec);
        minute_vec->clear();
        delete minute_vec;
        std::cout << "Creation date is: " << yy << ":" << m << ":" << d << " " << h << ":" << min << std::endl;
    }
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
        //TODO: Add try-catch at block reads: out-of-range

        //TODO: The buffer with the ciff has to be allocated with *new*
        int next = 0;
        std::vector<unsigned char>* data = NULL;
        try{
            data = get_block(buffer,next);
        }catch (std::out_of_range ex){
            buffer.clear();
            return -1;
        }

        int num_anim = parse_caff_header(*data);
        if(num_anim == -1){
            return -1;
        }
        //////
        next+=data->size()+9; // +9 is the ID+length size
        data->clear();
        delete data;
        //////
        try{
            data = get_block(buffer,next);
        }catch (std::out_of_range ex){
            buffer.clear();
            return -1;
        }
        parse_credits(*data);
        //////
        next+=data->size()+9; // +9 is the ID+length size
        data->clear();
        delete data;
        ///////
        int ret_val = 0;
        for(int i = 0; i < num_anim; i++){
            if(next < buffer.size()){
                try{
                    data = get_block(buffer,next);
                }catch (std::out_of_range ex){
                    buffer.clear();
                    return -1;
                }
                std::cout << "The next val is: " << next << std::endl;
                std::vector<unsigned char>* ciff_block = read_header(*data,8,data->size()-8);
                next+=data->size()+9;
                data->clear();
                delete data;
                if(i != 0){
                    std::string path_indexed(this->path);
                    path_indexed.replace(this->path.size()-5,6,std::to_string(i)+".caff");
                    CIFF_parser ciff_p(path_indexed,ciff_block);
                    ret_val = ciff_p.parse();
                } else{
                    CIFF_parser ciff_p(this->path,ciff_block);
                    ret_val = ciff_p.parse();
                }



            }
            if( ret_val == -1){
                break;
            }
            break;
        }
        return ret_val;
    }

};


#endif //CIFF_READER_HF_CAFF_PARSER_H
