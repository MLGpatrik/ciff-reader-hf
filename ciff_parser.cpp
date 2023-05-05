#include "ciff_parser.h"
#include <jpeglib.h>
#include <cstdio>

void CIFF_parser::write_jpeg_image(const char* filename, unsigned char* rgb_buffer, int width, int height, int quality)
{
    // Step 1: Create a file pointer and open the output file
    FILE* outfile = fopen(filename, "wb");
    if (!outfile) {
        fprintf(stderr, "Error opening output file %s\n", filename);
        return;
    }

    // Step 2: Initialize the JPEG compression parameters
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
    jpeg_set_quality(&cinfo, quality, TRUE);

    // Step 3: Start the JPEG compression process
    jpeg_start_compress(&cinfo, TRUE);

    // Step 4: Write the image data to the JPEG file
    JSAMPLE* row_pointer[1];
    while (cinfo.next_scanline < cinfo.image_height) {
        int row = cinfo.next_scanline;
        row_pointer[0] = &rgb_buffer[row * width * 3];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    // Step 5: Finish the JPEG compression process
    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
}



/*void CIFF_parser::write_jpeg_file(unsigned char* rgb_data, int width, int height, const char* filename, int quality){
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPROW row_pointer[1];
    int row_stride;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    FILE* outfile = fopen(filename, "wb");
    if (!outfile) {
        fprintf(stderr, "Error opening output JPEG file %s\n", filename);
        return;
    }

    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);

    jpeg_start_compress(&cinfo, TRUE);

    row_stride = width * 3;

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &rgb_data[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
}*/