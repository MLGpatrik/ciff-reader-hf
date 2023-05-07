#include "ciff_parser.h"
#include <jpeglib.h>



int CIFF_parser::search_for_newline(std::vector<unsigned char> buffer_, int from){
    int counter=0;
    for(long unsigned int i=from;i<buffer_.size();i++){
        if(buffer_[i] == '\n'){
            break;
        }
        counter++;
    }
    return counter;
}

std::vector<unsigned char>* CIFF_parser::read_RGB_value(std::vector<unsigned char> buffer_, int column, int row, int width, int offset){
    std::vector<unsigned char>* color_buffer = new std::vector<unsigned char>();
    if((long unsigned int)(offset + (row * width + column)*3 + 2) >= buffer_.size()){
        std::cerr << "The summa is: " << (offset + (row * width + column)*3 + 2) << " The column is: " << column << " The row is: " << row << std::endl;
        throw std::out_of_range("Too long read!");
    }
    color_buffer->push_back(buffer_[offset + (row * width + column) * 3]);        // R
    color_buffer->push_back(buffer_[offset + (row * width + column) * 3 + 1]) ;   // G
    color_buffer->push_back(buffer_[offset + (row * width + column) * 3 + 2]) ;   // B
    return color_buffer;
}

int CIFF_parser::write_jpeg_image(const char* filename, unsigned char* rgb_buffer, int width, int height){
    FILE* outfile = NULL;
    outfile = fopen(filename, "wb");
    if (!outfile) {
        std::cerr << "Error opening output file" << std::endl;
        return -1;
    }

    jpeg_compress_struct cinfo;
    jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);

    jpeg_start_compress(&cinfo, TRUE);

    JSAMPLE* row_pointer[1];
    while (cinfo.next_scanline < cinfo.image_height) {
        int row = cinfo.next_scanline;
        row_pointer[0] = &rgb_buffer[row * width * 3];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
    return 0;
}
