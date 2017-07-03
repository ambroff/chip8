#include <iostream>
#include <fstream>
#include <libgen.h>
#include <iomanip>

#include "decode.hpp"

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

    std::cout << "\t\tstart:" << std::endl;

    char next[2];
    while (!image_file.eof()) {
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

        int address{static_cast<int>(image_file.tellg()) + 0x200 - 2};

        switch ((opcode & 0xF000) >> 12) {
            case 1:
                std::cout << "\t" << "addr_" << std::dec << address << std::dec << std::endl;
                break;

            case 2:
                std::cout << "\t" << "sub_" << std::dec << address << std::dec << std::endl;
                break;

            default:
                break;
        }

        std::unique_ptr<chip8::Instruction> instruction = chip8::decode_opcode(opcode);
        if (!instruction) {
            std::cerr << "Unknown opcode: " << opcode << std::endl;
            return 1;
        }

        std::cout << std::hex<< std::setw(2) << std::setfill('0') << "0x" << opcode << std::dec << "|\t\t"
                  << instruction->toString()
                  << std::endl;
    }
}
