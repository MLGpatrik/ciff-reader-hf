#include "caff_parser.h"


std::vector<unsigned char>* CAFF_parser::get_block(std::vector<unsigned char> buffer,int from){
    get_block_id(buffer,from);
    int length = get_block_length(buffer,from);
    return get_block_data(buffer,from,length);
}

int CAFF_parser::get_block_id(std::vector<unsigned char> buffer,int from){
    std::vector<unsigned char>* id_vec = read_header(buffer,from,1);
    int id = ByteToInteger(*id_vec);
    clear_buffer_pointer(id_vec);
    std::cout << "The ID: " << id << std::endl;

    if(id < 0 || 4 <= id){
        throw std::out_of_range("Invalid ID");
    }
    return id;
}

int CAFF_parser::get_block_length(std::vector<unsigned char> buffer,int from){
    std::vector<unsigned char>* length_vec = read_header(buffer,from+1,8);
    int data_size = ByteToInteger(*length_vec);
    clear_buffer_pointer(length_vec);
    std::cout << "The length is: " << data_size << std::endl;

    if(data_size < 0 || (buffer.size()) < (unsigned int)data_size){
        throw std::out_of_range("Invalid data length!");
    }
    return data_size;
}

std::vector<unsigned char>* CAFF_parser::get_block_data(std::vector<unsigned char> buffer,int from,int length){
    if(buffer.size() < (unsigned int)(from+length)){
        throw std::out_of_range("Too long block read!");
    }
    return read_header(buffer,from+9,length);
}

int CAFF_parser::parse_caff_header(std::vector<unsigned char> buffer){
    std::string magic = read_magic(buffer);
    std::cout << "The MAGIC: " << magic << " ";
    if(!(magic.compare("CAFF") == 0)){
        std::cout<< "Invalid file magic!" << std::endl;
        return -1;
    }
    std::cout<< "Valid file magic!" << std::endl;

    std::vector<unsigned char>* header_size_vector = this->read_header(buffer,4,8);

    int header_size = this->ByteToInteger(*header_size_vector);
    clear_buffer_pointer(header_size_vector);
    std::cout << "Header size: " << header_size << std::endl;
    if(header_size < 0 || (long unsigned int) header_size > buffer.size()){
        std::cerr << "Invalid header size!" << std::endl;
        return -1;
    }

    std::vector<unsigned char>* num_anim_vec = this->read_header(buffer,12,8);
    num_anim = this->ByteToInteger(*num_anim_vec);
    clear_buffer_pointer(num_anim_vec);
    std::cout << "Number of animations in the file: " << num_anim << std::endl;
    if(num_anim < 1){
        std::cerr << "Invalid animation number!" << std::endl;
        return -1;
    }
    return num_anim;
}

int CAFF_parser::parse_credits(std::vector<unsigned char> buffer){
    std::vector<unsigned char>* year_vec = this->read_header(buffer,0,2);
    int yy = ByteToInteger(*year_vec);
    clear_buffer_pointer(year_vec);
    std::vector<unsigned char>* month_vec = this->read_header(buffer,2,1);
    int m = ByteToInteger(*month_vec);
    clear_buffer_pointer(month_vec);
    std::vector<unsigned char>* day_vec = this->read_header(buffer,3,1);
    int d = ByteToInteger(*day_vec);
    clear_buffer_pointer(day_vec);
    std::vector<unsigned char>* hour_vec = this->read_header(buffer,4,1);
    int h = ByteToInteger(*hour_vec);
    clear_buffer_pointer(hour_vec);
    std::vector<unsigned char>* minute_vec = this->read_header(buffer,5,1);
    int min = ByteToInteger(*minute_vec);
    clear_buffer_pointer(minute_vec);
    std::cout << "Creation date is: " << yy << ":" << m << ":" << d << " " << h << ":" << min << std::endl;
    return 0;
}

int CAFF_parser::parse_block(std::vector<unsigned char> buffer, int (CAFF_parser::*parsing_func)(std::vector<unsigned char>)){
    std::vector<unsigned char>* data = NULL;
    try{
        data = get_block(buffer,next);
    }catch (std::out_of_range const&){
        clear_buffer_pointer(data);
        buffer.clear();
        return -1;
    }
    if((this->*parsing_func)(*data) == -1){
        clear_buffer_pointer(data);
        buffer.clear();
        return -1;
    }
    next+=data->size()+9; // +9 is the ID+length size
    clear_buffer_pointer(data);
    return 0;
}
