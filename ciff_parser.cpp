#include "ciff_parser.h"
#include <jpeglib.h>
#define MEMTRACE
#include "memtrace.h"

int CIFF_parser::write_jpeg_image(const char* filename, unsigned char* rgb_buffer, int width, int height)
{
    // Create a file pointer and open the output file
    FILE* outfile = fopen(filename, "wb");
    if (!outfile) {
        std::cerr << "Error opening output file" << std::endl;
        return -1;
    }

    // Initialize the JPEG compression parameters
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

    // Start the JPEG compression process
    jpeg_start_compress(&cinfo, TRUE);

    // Write the image data to the JPEG file
    JSAMPLE* row_pointer[1];
    while (cinfo.next_scanline < cinfo.image_height) {
        int row = cinfo.next_scanline;
        row_pointer[0] = &rgb_buffer[row * width * 3];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    // Finish the JPEG compression process
    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);

    return 0;
}
