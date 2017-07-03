#include <iostream>
#include <fstream>
#include <libgen.h>
#include <iomanip>

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << basename(argv[0]) << " IMAGE_FILE" << std::endl;
        return 1;
    }

    std::ifstream image_file;
    image_file.open(argv[1], std::ios::binary);
    if (!image_file.is_open()) {
        std::cerr << "ERROR: Unable to open "<< argv[1] << ": " << std::strerror(errno) << std::endl;
        return 1;
    }

    char next[2];
    while (!image_file.eof()) {
        //image_file.read(reinterpret_cast<char *>(&opcode), sizeof(uint16_t));
        image_file.read(next, 2);

        if (image_file.fail()) {
            if (image_file.eof()) {
                break;
            } else {
                std::cerr << "ERROR: Failed to read next opcode at offset: " << image_file.tellg() << std::endl;
                return 1;
            }
        }

        uint16_t opcode{static_cast<uint16_t>(next[0] << 8 | next[1])};

        switch ((opcode & 0xF000) >> 12) {
            case 1:
                break;
            case 2:
        }

        std::cout << "OpCode: " << std::hex<< std::setw(2) << std::setfill('0')
                  << "0x" << opcode << std::dec << std::endl;
    }
}
