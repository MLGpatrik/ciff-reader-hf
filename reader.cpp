#include <iostream>
#include "parser.h"
#include "ciff_parser.h"
#include "caff_parser.h"

using namespace std;

int main(int argc, char *argv[]) {
        const char *logo = R"V0G0N(
 ______     ______     ______   ______   ______     ______
/\  ___\   /\  __ \   /\  ___\ /\  ___\ /\  ___\   /\  == \
\ \ \____  \ \  __ \  \ \  __\ \ \  __\ \ \  __\   \ \  __<
 \ \_____\  \ \_\ \_\  \ \_\    \ \_\    \ \_____\  \ \_\ \_\
  \/_____/   \/_/\/_/   \/_/     \/_/     \/_____/   \/_/ /_/
            )V0G0N";
        std::cout << logo << std::endl;
        if (argc == 1) {
            std::cout << "There were no parameters given, use --help to show help menu." << std::endl;
            return -1;
        }
        string input_string = argv[1];
        if (argc == 2) {
            if (input_string.compare("--help") == 0) {
                std::cout << "CAFFER: CrySyS CAFF and CIFF converter" << std::endl;
                std::cout << "Usage: reader [--ciff | --caff] [target_file]" << std::endl << std::endl;
                std::cout << "\tCommand Summary:" << std::endl << std::endl;
                std::cout << "\t\t--ciff target_file \t|\tRead CIFF target_file and exports it to a JPEG file"
                          << std::endl;
                std::cout
                        << "\t\t--caff target_file \t|\tRead CAFF target_file and exports the first image to a JPEG file"
                        << std::endl;
                std::cout << "\t\t--help \t\t\t|\tThis help message" << std::endl;
                return -1;
            } else {
                std::cout << "Invalid parameters, use --help to see the help" << std::endl;
                return -1;
            }
        }
        if (argc < 3) {
            std::cout << "You have to define --ciff or --caff and a path to the file" << std::endl;
            return -1;
        }
        if (argc > 3) {
            std::cout << "You have given too much parameters" << std::endl;
            return -1;
        }
        string target_file_path = argv[2];
        Parser *parser = NULL;
        if (input_string.compare("--ciff") == 0) {
            std::cout << "Converting CIFF to jpeg..." << std::endl;
            parser = new CIFF_parser(target_file_path);
        } else {
            if (input_string.compare("--caff") == 0) {
                std::cout << "Converting CAFF to jpeg" << std::endl;
                parser = new CAFF_parser(target_file_path);
            } else {
                std::cout << "Not a valid parameter!" << std::endl;
                return -1;
            }
        }

        if (parser != NULL) {
            int ret_val = parser->parse();
            delete parser;
            return ret_val;
        }
    return -1;
}
