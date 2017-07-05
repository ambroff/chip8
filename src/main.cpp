/**
 * Chip8 Interpreter. Made following documentation here:
 *
 *  - http://mattmik.com/files/chip8/mastering/chip8.html
 *  - http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <cstring>

#include "cpu.hpp"
#include "decode.hpp"
#include "font.hpp"

namespace chip8 {
    class Machine final {
    public:
        void reset() {
            mCpu.reset();

            // Load the font
            std::copy(FONT.begin(), FONT.end(), mCpu.memory.begin());
        }

        void loadProgram(std::ifstream &inputStream) {
            reset();
            std::copy(
                std::istream_iterator<uint8_t>(inputStream),
                std::istream_iterator<uint8_t>(),
                mCpu.memory.begin() + 0x200);
        }

        /**
         * Execute one instruction.
         */
        void step() {
            // Read the next opcode from main memory
            uint16_t opcode{static_cast<uint16_t>(mCpu.memory[mCpu.pc] << 8 | mCpu.memory[mCpu.pc + 1])};

            // Decode the instruction
            std::unique_ptr<Instruction> instruction = decode_opcode(opcode);
            if (!instruction) {
                std::cerr << "Unable to decode instruction: " << std::hex << opcode << std::endl;
                exit(1); // TODO: Use an exception here instead?
            }

            // Actually execute the instruction
            instruction->execute(mCpu);

            // Each instruction is two bytes long, so we need to advance by two bytes.
            mCpu.pc += 2;
        }

        void dumpCore(std::ostream& outputStream) {
            mCpu.dumpState(outputStream);
        }

    private:
        cpu_t mCpu;
    };
}

int main() {
    const std::string program{"data/games/BREAKOUT"};

    // 64 x 32 pixel display
    // sprites are 8 pixels wide and 1-15 pixels tall
    //const int GRAPHICS_SCALE_FACTOR{3};

    std::ifstream inputStream;
    inputStream.open(program, std::ios::binary);
    if (!inputStream.is_open()) {
        std::cerr << "Unable to open file: " << std::strerror(errno) << std::endl;
        return 1;
    }

    chip8::Machine machine;
    machine.loadProgram(inputStream);
    machine.step();

    machine.dumpCore(std::cout);

    return 0;
}
