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
#include <iterator>
#include <cstring>
#include <iomanip>

namespace {
    const uint16_t REGISTER_COUNT{16};
    const uint16_t STACK_SIZE{16};
    const uint16_t MEMORY_SIZE{4096};

    template<int RegisterCount, int StackSize, int MemorySize>
    struct CPU final {
        std::array<uint8_t, MemorySize> memory;
        std::array<uint8_t, RegisterCount> V;
        std::array<uint16_t, StackSize> stack;

        uint16_t pc{0x200};
        uint16_t I{0};

        std::array<bool, 64 * 32> fb;

        uint8_t delayTimer{0};
        uint8_t soundTimer{0};

        void reset() {
            std::fill(V.begin(), V.end(), 0);
            std::fill(fb.begin(), fb.end(), false);
            std::fill(stack.begin(), stack.end(), 0);
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
                outputStream << "V" << i << ": 0x" << std::hex << static_cast<int>(V[i]) << std::endl;
            }

            outputStream << std::endl;

            outputStream << "delayTimer:\t" << std::dec << static_cast<int>(delayTimer) << std::endl;
            outputStream << "soundTimer:\t" << std::dec << static_cast<int>(soundTimer) << std::endl;
            outputStream << std::endl;

            outputStream << "Stack:" << std::endl;
            for (int i = 0; i < stack.size(); i++) {
                outputStream << std::hex << std::setfill('0') << std::setw(2) << stack[i] << ' ';
            }
            outputStream << std::endl;

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

    typedef CPU<REGISTER_COUNT, STACK_SIZE, MEMORY_SIZE> cpu_t;

    class Instruction {
    public:
        virtual ~Instruction() {}

        virtual void execute(cpu_t &cpu) const = 0;
    };

    /**
     * 0NNN	Execute machine language subroutine at address NNN
     */
    class ExecuteMachineSubroutineInstruction : public Instruction {
    public:
        ExecuteMachineSubroutineInstruction(uint16_t address)
            : mAddress(address) {
        }

        virtual void execute(cpu_t &cpu) const override {
            throw std::exception{};
        }

    private:
        uint16_t mAddress;
    };

    /**
     * 00E0	Clear the screen
     */
    class ClearScreenInstruction : public Instruction {
    public:
        void execute(cpu_t& cpu) const override {
            std::fill(cpu.fb.begin(), cpu.fb.end(), false);
        }
    };

    /**
     * 00EE	Return from a subroutine
     */
    class ReturnInstruction : public Instruction {
    public:
        void execute(cpu_t& cpu) const override {
            cpu.pc = cpu.stack[cpu.I--];
        }
    };

    /**
     * 1NNN	Jump to address NNN
     */
    class JumpInstruction : public Instruction {
    public:
        JumpInstruction(const uint16_t address)
            : mTargetAddress(address)
        {
        }

        void execute(cpu_t& cpu) const override {
            cpu.pc = mTargetAddress;
        }

    private:
        uint16_t mTargetAddress;
    };

    /**
     * 2NNN	Execute subroutine starting at address NNN
     */
    class ExecuteSubroutineInstruction : public Instruction {
    };

    /**
     * 3XNN	Skip the following instruction if the value of register VX equals NN
     */
    class SkipIfVxEqualsInstruction : public Instruction {
    };

    /**
     * 4XNN	Skip the following instruction if the value of register VX is not equal to NN
     */
    class SkipIfVxNotEqualInstruction : public Instruction {
    };

    /**
     * 5XY0	Skip the following instruction if the value of register VX is equal to the value of register VY
     */
    class SkipIfVxEqualVyInstruction : public Instruction {
    };

    /**
     * 6XNN	Store number NN in register VX
     */
    class StoreInVxInstruction : public Instruction {
    public:
        StoreInVxInstruction(const uint8_t reg, const uint8_t value)
            : mRegIdx(reg),
              mValue(value)
        {
        }

        void execute(cpu_t& cpu) const override {
            cpu.V[mRegIdx] = mValue;
        }

    private:
        uint8_t mRegIdx;
        uint8_t mValue;
    };

    /**
     * 7XNN	Add the value NN to register VX
     */
    class IncrementInstruction : public Instruction {
    };

    /**
     * 8XY0	Store the value of register VY in register VX
     */
    class StoreVxInVyInstruction : public Instruction {
    };

    /**
     * 8XY1	Set VX to VX OR VY
     */
    class BitwiseOrInstruction : public Instruction {
    };

    /**
     * 8XY2	Set VX to VX AND VY
     */
    class BitwiseAndInstruction : public Instruction {
    };

    /**
     * 8XY3	Set VX to VX XOR VY
     */
    class BitwiseXorInstruction : public Instruction {
    };

    /**
     * 8XY4	Add the value of register VY to register VX
     * Set VF to 01 if a carry occurs
     * Set VF to 00 if a carry does not occur
     */
    class AddInstruction : public Instruction {
    };

    /**
     * 8XY5	Subtract the value of register VY from register VX
     * Set VF to 00 if a borrow occurs
     * Set VF to 01 if a borrow does not occur
     */
    class DecrementInstruction : public Instruction {
    };

    /**
     * 8XY6	Store the value of register VY shifted right one bit in register VX
     * Set register VF to the least significant bit prior to the shift
     */
    class ShiftRightInstruction : public Instruction {
    };

    /**
     * 8XY7	Set register VX to the value of VY minus VX
     * Set VF to 00 if a borrow occurs
     * Set VF to 01 if a borrow does not occur
     */
    class SubtractInstruction : public Instruction {
    };

    /**
     * 8XYE	Store the value of register VY shifted left one bit in register VX
     * Set register VF to the most significant bit prior to the shift
     */
    class ShiftLeftInstruction : public Instruction {
    };

    /**
     * 9XY0	Skip the following instruction if the value of register VX is not equal to the value of register VY
     */
    class SkipIfVxNotEqualVyInstruction : public Instruction {
    };

    /**
     * ANNN	Store memory address NNN in register I
     */
    class StoreAddressInstruction : public Instruction {
    };

    /**
     * BNNN	Jump to address NNN + V0
     */
    class JumpToAddressInstruction : public Instruction {
    };

    /**
     * CXNN	Set VX to a random number with a mask of NN
     */
    class StoreRandomWithMaskInstruction : public Instruction {
    };

    /**
     * DXYN	Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I
     * Set VF to 01 if any set pixels are changed to unset, and 00 otherwise
     */
    class DrawSpriteInstruction : public Instruction {
    };

    /**
     * EX9E	Skip the following instruction if the key corresponding to the hex value currently stored in register VX is
     * pressed
     */
    class SkipIfKeyPressedInstruction : public Instruction {
    };

    /**
     * EXA1	Skip the following instruction if the key corresponding to the hex value currently stored in register VX
     * is not pressed
     */
    class SkipIfKeyNotPressedInstruction : public Instruction {
    };

    /**
     * FX07	Store the current value of the delay timer in register VX
     */
    class SetTimerInstruction : public Instruction {
    };

    /**
     * FX0A	Wait for a keypress and store the result in register VX
     */
    class WaitForKeypressInstruction : public Instruction {
    };

    /**
     * FX15	Set the delay timer to the value of register VX
     */
    class SetDelayTimerInstruction : public Instruction {
    };

    /**
     * FX18	Set the sound timer to the value of register VX
     */
    class SetSoundTimerInstruction : public Instruction {
    };

    /**
     * FX1E	Add the value stored in register VX to register I
     */
    class AddToIInstruction : public Instruction {
    };

    /**
     * FX29	Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
     */
    class StoreInIInstruction : public Instruction {
    };

    /**
     * FX33	Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I+1, and I+2
     */
    class StoreDecimalInstruction : public Instruction {
    };

    /**
     * FX55	Store the values of registers V0 to VX inclusive in memory starting at address I
     * I is set to I + X + 1 after operation
     */
    class StoreRegistersInstruction : public Instruction {
    };

    /**
     * FX65	Fill registers V0 to VX inclusive with the values stored in memory starting at address I
     * I is set to I + X + 1 after operation
     */
    class RestoreRegistersInstruction : public Instruction {
    };

    inline bool decode_opcode(uint16_t opcode, Instruction*& result) {
        if (opcode == 0x00E0) {
            result = new ClearScreenInstruction{};
            return true;
        } else if (opcode == 0x00EE) {
            result = new ReturnInstruction{};
            return true;
        } else if (opcode & 0x1000) {
            result = new JumpInstruction{static_cast<uint16_t>(opcode & static_cast<uint16_t>(4096))};
            return true;
        } else if (opcode & 0x6000) {
            uint8_t reg = static_cast<uint8_t>(opcode & (~4096 | ~3));
            uint8_t value = static_cast<uint8_t>(opcode & (~4096 | ~4));
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
            uint16_t opcode{mCpu.memory[mCpu.pc] << 8 | mCpu.memory[mCpu.pc + 1]};

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
    const int GRAPHICS_SCALE_FACTOR{3};

    std::ifstream inputStream;
    inputStream.open(program, std::ios::binary);
    if (!inputStream.is_open()) {
        std::cerr << "Unable to open file: " << std::strerror(errno) << std::endl;
        return 1;
    }

    Machine machine;
    machine.loadProgram(inputStream);
    machine.step();

    machine.dumpCore(std::cout);

    return 0;
}
