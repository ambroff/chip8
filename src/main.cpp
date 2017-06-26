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

namespace {
    const uint16_t REGISTER_COUNT{16};
    const uint16_t STACK_SIZE{16};
    const uint16_t MEMORY_SIZE{4096};

    inline void not_implemented() {
        std::cerr << __PRETTY_FUNCTION__ << ": Not implemented!" << std::endl;
    }

    template<int RegisterCount, int StackSize, int MemorySize>
    struct CPU final {
        std::array<uint8_t, MemorySize> memory;
        std::array<uint8_t, RegisterCount> V;
        std::array<uint16_t, StackSize> stack;

        uint16_t pc{0x200};
        uint16_t I{0};

        std::array<bool, 64 * 32> fb;

        uint8_t delayTimer;
        uint8_t soundTimer;
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
            not_implemented();
        }

    private:
        uint16_t mAddress;
    };

    /**
     * 00E0	Clear the screen
     */
    class ClearScreenInstruction : public Instruction {
    public:
        virtual void execute(cpu_t& cpu) const override {
            std::fill(cpu.fb.begin(), cpu.fb.end(), false);
        }
    };

    /**
     * 00EE	Return from a subroutine
     */
    class ReturnInstruction : public Instruction {
    };

    /**
     * 1NNN	Jump to address NNN
     */
    class JumpInstruction : public Instruction {
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

    class Machine final {
    public:
        void reset() {
        }

        void loadProgram(std::ifstream &inputStream) {

        }
    };
}

int main() {
    // 64 x 32 pixel display
    // sprites are 8 pixels wide and 1-15 pixels tall
    const int GRAPHICS_SCALE_FACTOR{3};

    cpu_t cpu;

    std::cout << "Hello, World!" << std::endl;

    ExecuteMachineSubroutineInstruction instruction{123};
    instruction.execute(cpu);

    return 0;
}
