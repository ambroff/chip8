#pragma once

#include <array>
#include <ostream>
#include <iomanip>

#include "boundedstack.hpp"

namespace chip8 {
    const uint16_t REGISTER_COUNT{16};
    const uint16_t STACK_SIZE{16};
    const uint16_t MEMORY_SIZE{4096};

    template<int RegisterCount, int StackSize, int MemorySize>
    struct CPU final {
        std::array<uint8_t, MemorySize> memory;
        std::array<uint8_t, RegisterCount> V;
        boundedstack<uint16_t, StackSize> stack;

        uint16_t pc{0x200};
        uint16_t I{0};

        std::array<bool, 64 * 32> fb;

        uint8_t delayTimer{0};
        uint8_t soundTimer{0};

        void reset() {
            std::fill(V.begin(), V.end(), 0);
            std::fill(fb.begin(), fb.end(), false);
            stack.clear();
            std::fill(memory.begin(), memory.end(), 0);
            pc = 0x200;
            I = 0;
            delayTimer = 0;
            soundTimer = 0;
        }

        void dumpState(std::ostream& outputStream) {
            outputStream << "PC:\t0x" << std::hex << pc << std::endl;
            outputStream << "I:\t0x" << std::hex << I << std::endl;

            outputStream << std::endl;

            for (int i = 0; i < RegisterCount; i++) {
                outputStream << "V" << std::dec << std::setfill('0') << std::setw(2) << i
                             << ": 0x" << std::hex << static_cast<int>(V[i]) << std::endl;
            }

            outputStream << std::endl;

            outputStream << "delayTimer:\t" << std::dec << static_cast<int>(delayTimer) << std::endl;
            outputStream << "soundTimer:\t" << std::dec << static_cast<int>(soundTimer) << std::endl;
            outputStream << std::endl;

//            outputStream << "Stack:" << std::endl;
//            for (int i = 0; i < stack.size(); i++) {
//                outputStream << std::hex << std::setfill('0') << std::setw(2) << stack[i] << ' ';
//            }
//            outputStream << std::endl;

            outputStream << std::endl << "Frame buffer:" << std::endl;
            for (int i = 1; i <= fb.size(); i++) {
                int pixel = fb[i-1] ? 1 : 0;
                outputStream << pixel;
                if (i % 64 == 0) {
                    outputStream << std::endl;
                }
            }
            outputStream << std::endl;

//            outputStream << "Main memory:" << std::endl;
//            for (int i = 1; i < memory.size(); i++) {
//                outputStream << std::hex << std::setfill('0') << std::setw(2)
//                             << static_cast<int>(memory[i - 1])
//                             << (i % 32 == 0 ? '\n' : ' ');
//            }
//            outputStream << std::endl;
        }
    };

    using cpu_t = CPU<REGISTER_COUNT, STACK_SIZE, MEMORY_SIZE>;
}
