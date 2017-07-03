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

#include "cpu.hpp"
#include "instructions.hpp"

namespace chip8 {
    inline bool decode_opcode(uint16_t opcode, Instruction*& result) {
        if (opcode == 0x00E0) {
            result = new ClearScreenInstruction{};
            return true;
        }

        if (opcode == 0x00EE) {
            result = new ReturnInstruction{};
            return true;
        }

        if ((opcode & 0x1000) != 0) {
            result = new JumpInstruction{static_cast<uint16_t>(opcode & static_cast<uint16_t>(4096))};
            return true;
        }

        if ((opcode & 0xF000) == 0x6000) {
            auto reg = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto value = static_cast<uint8_t>(opcode & 0x00FF);
            result = new StoreInVxInstruction{reg, value};
            return true;
        }

        return false;
    }

    class Machine final {
    public:
        void reset() {
            mCpu.reset();
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
            uint16_t opcode{static_cast<uint16_t>(mCpu.memory[mCpu.pc] << 8 | mCpu.memory[mCpu.pc + 1])};

            Instruction *instruction = nullptr;
            if (!decode_opcode(opcode, instruction)) {
                std::cerr << "Unable to decode instruction: " << std::hex << opcode << std::endl;
                exit(1); // TODO: Use an exception here instead?
            }

            instruction->execute(mCpu);

            delete instruction;

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
