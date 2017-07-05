#pragma once

#include <sstream>

#include "cpu.hpp"

namespace chip8 {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "ClangTidyInspection"
    class Instruction {
    public:
        virtual ~Instruction() = default;
        virtual void execute(cpu_t &cpu) const = 0;
        virtual std::string toString() const = 0;
    };
#pragma clang diagnostic pop

    /**
     * 0NNN	Execute machine language subroutine at address NNN
     */
    class SystemCallInstruction : public Instruction {
    public:
        explicit SystemCallInstruction(uint16_t address)
            : mAddress(address) {
        }

        void execute(cpu_t &cpu) const override {
        }

        std::string toString() const override {
            std::ostringstream s;
            s << "SYS 0x" << std::hex << mAddress;
            return s.str();
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

        std::string toString() const override {
            return "CLS";
        }
    };

    /**
     * 00EE	Return from a subroutine
     */
    class ReturnInstruction : public Instruction {
    public:
        void execute(cpu_t& cpu) const override {
            cpu.pc = cpu.stack.top();
            cpu.stack.pop();
        }

        std::string toString() const override {
            return "RET";
        }
    };

    /**
     * 1NNN	Jump to address NNN
     */
    class JumpInstruction : public Instruction {
    public:
        explicit JumpInstruction(const uint16_t address)
            : mTargetAddress(address)
        {
        }

        void execute(cpu_t& cpu) const override {
            cpu.pc = mTargetAddress;
        }

        std::string toString() const override {
            std::ostringstream s;
            s << "JMP 0x" << std::hex << mTargetAddress;
            return s.str();
        }

    private:
        uint16_t mTargetAddress;
    };

    /**
     * 2NNN	Execute subroutine starting at address NNN
     */
    class CallInstruction : public Instruction {
    public:
        CallInstruction(uint16_t targetAddress)
            : mTargetAddress(targetAddress)
        {
        }

        std::string toString() const override {
            std::ostringstream s;
            s << "CALL 0x" << std::hex << mTargetAddress;
            return s.str();
        }

        void execute(cpu_t &cpu) const override {
            cpu.stack.push(cpu.pc);
            // FIXME: Validate target address and throw an error?
            cpu.pc = mTargetAddress;
        }

    private:
        uint16_t mTargetAddress;
    };

    /**
     * 3XNN	Skip the following instruction if the value of register VX equals NN
     */
    class SkipIfVxEqualsInstruction : public Instruction {
    public:
        SkipIfVxEqualsInstruction(uint8_t reg, uint8_t value)
            : mRegister(reg),
              mValue(value)
        {
        }

        void execute(cpu_t &cpu) const override {
            if (cpu.V[mRegister] == mValue) {
                cpu.pc += 2;
            }
        }

        std::string toString() const override {
            std::ostringstream s;
            s << "SKE V" << std::to_string(mRegister) << ", 0x" << std::hex << static_cast<int>(mValue);
            return s.str();
        }

    private:
        uint8_t mRegister;
        uint8_t mValue;
    };

    /**
     * 4XNN	Skip the following instruction if the value of register VX is not equal to NN
     */
    class SkipIfVxNotEqualInstruction : public Instruction {
    public:
        SkipIfVxNotEqualInstruction(uint8_t reg, uint8_t value)
            : mRegister(reg),
              mValue(value)
        {
        }

        void execute(cpu_t &cpu) const override {
            if (cpu.V[mRegister] != mValue) {
                cpu.pc += 2;
            }
        }

        std::string toString() const override {
            std::ostringstream s;
            s << "SKNE V" << std::to_string(mRegister) << ", 0x" << std::hex << static_cast<int>(mValue);
            return s.str();
        }

    private:
        uint8_t mRegister;
        uint8_t mValue;
    };

    /**
     * 5XY0	Skip the following instruction if the value of register VX is equal to the value of register VY
     */
    class SkipIfVxEqualsVyInstruction : public Instruction {
    public:
        SkipIfVxEqualsVyInstruction(uint8_t reg_x, uint8_t reg_y)
            : mRegisterX(reg_x),
              mRegisterY(reg_y)
        {
        }

        void execute(cpu_t &cpu) const override {
            if (cpu.V[mRegisterX] == cpu.V[mRegisterY]) {
                cpu.pc += 2;
            }
        }

        std::string toString() const override {
            return "SKRE V" + std::to_string(mRegisterX) + ", V" + std::to_string(mRegisterY);
        }

    private:
        uint8_t mRegisterX;
        uint8_t mRegisterY;
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

        std::string toString() const override {
            std::ostringstream s;
            s << "MOV V" << std::to_string(mRegIdx) << ", 0x" << std::hex << static_cast<int>(mValue);
            return s.str();
        }

    private:
        uint8_t mRegIdx;
        uint8_t mValue;
    };

    /**
     * 7XNN	Add the value NN to register VX
     */
    class IncrementInstruction : public Instruction {
    public:
        IncrementInstruction(uint8_t reg, uint8_t value)
            : mRegister(reg),
              mValue(value)
        {
        }

        void execute(cpu_t& cpu) const override {
            cpu.V[mRegister] += mValue;
        }

        std::string toString() const override {
            std::ostringstream s;
            s << "INC V" << std::to_string(mRegister) + ", 0x" << std::hex << static_cast<int>(mValue);
            return s.str();
        }

    private:
        uint8_t mRegister;
        uint8_t mValue;
    };

    /**
     * 8XY0	Store the value of register VY in register VX
     */
    class MoveInstruction : public Instruction {
    public:
        MoveInstruction(uint8_t reg_x, uint8_t reg_y)
            : mRegisterX(reg_x),
              mRegisterY(reg_y)
        {
        }

        void execute(cpu_t &cpu) const override {
            cpu.V[mRegisterX] = cpu.V[mRegisterY];
        }

        std::string toString() const override {
            return "MOV V" + std::to_string(mRegisterX) + ", V" + std::to_string(mRegisterY);
        }

    private:
        uint8_t mRegisterX;
        uint8_t mRegisterY;
    };

    /**
     * 8XY1	Set VX to VX OR VY
     */
    class BitwiseOrInstruction : public Instruction {
    public:
        BitwiseOrInstruction(uint8_t x, uint8_t y)
            : mRegisterX(x),
              mRegisterY(y)
        {
        }

        void execute(cpu_t& cpu) const override {
            cpu.V[mRegisterX] = cpu.V[mRegisterX] | cpu.V[mRegisterY];
        }

        std::string toString() const override {
            return "OR V" + std::to_string(mRegisterX) + ", V" + std::to_string(mRegisterY);
        }

    private:
        uint8_t mRegisterX;
        uint8_t mRegisterY;
    };

    /**
     * 8XY2	Set VX to VX AND VY
     */
    class BitwiseAndInstruction : public Instruction {
    public:
        BitwiseAndInstruction(uint8_t x, uint8_t y)
            : mRegisterX(x),
              mRegisterY(y)
        {
        }

        void execute(cpu_t& cpu) const override {
            cpu.V[mRegisterX] = cpu.V[mRegisterX] & cpu.V[mRegisterY];
        }

        std::string toString() const override {
            return "AND V" + std::to_string(mRegisterX) + ", V" + std::to_string(mRegisterY);
        }

    private:
        uint8_t mRegisterX;
        uint8_t mRegisterY;
    };

    /**
     * 8XY3	Set VX to VX XOR VY
     */
    class BitwiseXorInstruction : public Instruction {
    public:
        BitwiseXorInstruction(uint8_t x, uint8_t y)
            : mRegisterX(x),
              mRegisterY(y)
        {
        }

        void execute(cpu_t& cpu) const override {
            cpu.V[mRegisterX] = cpu.V[mRegisterX] ^ cpu.V[mRegisterY];
        }

        std::string toString() const override {
            return "XOR V" + std::to_string(mRegisterX) + ", V" + std::to_string(mRegisterY);
        }

    private:
        uint8_t mRegisterX;
        uint8_t mRegisterY;
    };

    /**
     * 8XY4	Add the value of register VY to register VX
     * Set VF to 01 if a carry occurs
     * Set VF to 00 if a carry does not occur
     */
    class AddInstruction : public Instruction {
    public:
        AddInstruction(uint8_t reg_x, uint8_t reg_y)
            : mRegisterX(reg_x),
              mRegisterY(reg_y)
        {
        }

    private:
        uint8_t mRegisterX;
        uint8_t mRegisterY;
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
    public:
        SubtractInstruction(uint8_t x, uint8_t y)
            : mRegisterX(x),
              mRegisterY(y)
        {
        }

        void execute(cpu_t& cpu) const override {
            cpu.V[mRegisterX] = cpu.V[mRegisterY] - cpu.V[mRegisterX];
            cpu.V[15] = 1; // TODO: Set to 0 if borrow occurs
        }

        std::string toString() const override {
            return "SUB V" + std::to_string(mRegisterX) + ", V" + std::to_string(mRegisterY);
        }

    private:
        uint8_t mRegisterX;
        uint8_t mRegisterY;
    };

    /**
     * 8XYE	Store the value of register VY shifted left one bit in register VX
     * Set register VF to the most significant bit prior to the shift
     */
    class ShiftLeftInstruction : public Instruction {
    public:
        ShiftLeftInstruction(uint8_t reg_x, uint8_t reg_y)
            : mRegisterX(reg_x),
              mRegisterY(reg_y)
        {
        }

    private:
        uint8_t mRegisterX;
        uint8_t mRegisterY;
    };

    /**
     * 9XY0	Skip the following instruction if the value of register VX is not equal to the value of register VY
     */
    class SkipIfVxNotEqualsVyInstruction : public Instruction {
    public:
        SkipIfVxNotEqualsVyInstruction(uint8_t reg_x, uint8_t reg_y)
            : mRegisterX(reg_x),
              mRegisterY(reg_y)
        {
        }

        void execute(cpu_t &cpu) const override {
            if (cpu.V[mRegisterX] != cpu.V[mRegisterY]) {
                cpu.pc += 2;
            }
        }

        std::string toString() const override {
            return "SKRNE V" + std::to_string(mRegisterX) + ", V" + std::to_string(mRegisterY);
        }

    private:
        uint8_t mRegisterX;
        uint8_t mRegisterY;
    };

    /**
     * ANNN	Store memory address NNN in register I
     */
    class LoadIndexInstruction : public Instruction {
    public:
        LoadIndexInstruction(uint16_t address)
            : mAddress(address)
        {
        }

        void execute(cpu_t &cpu) const override {
            cpu.I = mAddress;
        }

        std::string toString() const override {
            std::ostringstream s;
            s << "LOADI 0x" << std::hex << static_cast<int>(mAddress);
            return s.str();
        }

    private:
        uint16_t mAddress;
    };

    /**
     * BNNN	Jump to address NNN + V0
     */
    class JumpIndexInstruction : public Instruction {
    public:
        JumpIndexInstruction(uint16_t target_address)
            : mTargetAddress(target_address)
        {
        }

        void execute(cpu_t &cpu) const override {
            cpu.pc = cpu.V[0] + mTargetAddress;
        }

        std::string toString() const override {
            std::ostringstream s;
            s << "JUMPI 0x" << std::hex << static_cast<int>(mTargetAddress);
            return s.str();
        }

    private:
        uint16_t mTargetAddress;
    };

    /**
     * CXNN	Set VX to a random number with a mask of NN
     */
    class StoreRandomWithMaskInstruction : public Instruction {
    public:
        StoreRandomWithMaskInstruction(uint8_t reg, uint8_t value)
            : mRegister(reg),
              mValue(value)
        {
        }

    private:
        uint8_t mRegister;
        uint8_t mValue;
    };

    /**
     * DXYN	Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I
     * Set VF to 01 if any set pixels are changed to unset, and 00 otherwise
     */
    class DrawSpriteInstruction : public Instruction {
    public:
        DrawSpriteInstruction(uint8_t reg_x, uint8_t reg_y, uint8_t length)
            : mRegisterX(reg_x),
              mRegisterY(reg_y),
              mLength(length)
        {
        }

    private:
        uint8_t mRegisterX;
        uint8_t mRegisterY;
        uint8_t mLength;
    };

    /**
     * EX9E	Skip the following instruction if the key corresponding to the hex value currently stored in register VX is
     * pressed
     */
    class SkipIfKeyPressedInstruction : public Instruction {
    public:
        SkipIfKeyPressedInstruction(uint8_t reg)
            : mRegister(reg)
        {
        }

    private:
        uint8_t mRegister;
    };

    /**
     * EXA1	Skip the following instruction if the key corresponding to the hex value currently stored in register VX
     * is not pressed
     */
    class SkipIfKeyNotPressedInstruction : public Instruction {
    public:
        SkipIfKeyNotPressedInstruction(uint8_t reg)
            : mRegister(reg)
        {
        }

    private:
        uint8_t mRegister;
    };

    /**
     * FX07	Store the current value of the delay timer in register VX
     */
    class StoreDelayTimerInstruction : public Instruction {
    public:
        StoreDelayTimerInstruction(uint8_t reg) : mRegister(reg)
        {
        }

        void execute(cpu_t& cpu) const override {
            cpu.V[mRegister] = cpu.delayTimer;
        }

        std::string toString() const override {
            return "MOVED V" + std::to_string(mRegister);
        }

    private:
        uint8_t mRegister;
    };

    /**
     * FX0A	Wait for a keypress and store the result in register VX
     */
    class WaitForKeypressInstruction : public Instruction {
    public:
        WaitForKeypressInstruction(uint8_t reg)
            : mRegister(reg)
        {
        }

    private:
        uint8_t mRegister;
    };

    /**
     * FX15	Set the delay timer to the value of register VX
     */
    class LoadDelayTimerInstruction : public Instruction {
    public:
        LoadDelayTimerInstruction(uint8_t reg) : mRegister(reg)
        {
        }

        void execute(cpu_t& cpu) const override {
            cpu.delayTimer = cpu.V[mRegister];
        }

        std::string toString() const override {
            return "LOADD V" + std::to_string(mRegister);
        }

    private:
        uint8_t mRegister;
    };

    /**
     * FX18	Set the sound timer to the value of register VX
     */
    class SetSoundTimerInstruction : public Instruction {
    public:
        SetSoundTimerInstruction(uint8_t reg)
            : mRegister(reg)
        {
        }

    private:
        uint8_t mRegister;
    };

    /**
     * FX1E	Add the value stored in register VX to register I
     */
    class AddToIInstruction : public Instruction {
    public:
        AddToIInstruction(uint8_t reg)
            :mRegister(reg)
        {
        }

    private:
        uint8_t mRegister;
    };

    /**
     * FX29	Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
     */
    class StoreInIInstruction : public Instruction {
    public:
        StoreInIInstruction(uint8_t reg)
            : mRegister(reg)
        {
        }

    private:
        uint8_t mRegister;
    };

    /**
     * FX33	Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I+1, and I+2
     */
    class StoreDecimalInstruction : public Instruction {
    public:
        StoreDecimalInstruction(uint8_t reg)
            : mRegister(reg)
        {
        }

    private:
        uint8_t mRegister;
    };

    /**
     * FX55	Store the values of registers V0 to VX inclusive in memory starting at address I
     * I is set to I + X + 1 after operation
     */
    class StoreRegistersInstruction : public Instruction {
    public:
        StoreRegistersInstruction(uint8_t up_to_register)
            : mUpToRegister(up_to_register)
        {
        }

        void execute(cpu_t &cpu) const override {
            auto byte_count = mUpToRegister + static_cast<uint16_t>(1);
            std::copy_n(cpu.V.begin(), byte_count, cpu.memory.begin() + cpu.I);
            cpu.I = cpu.I + byte_count;
        }

        std::string toString() const override {
            return "STOR V" + std::to_string(mUpToRegister);
        }

    private:
        uint8_t mUpToRegister;
    };

    /**
     * FX65	Fill registers V0 to VX inclusive with the values stored in memory starting at address I
     * I is set to I + X + 1 after operation
     */
    class RestoreRegistersInstruction : public Instruction {
    public:
        RestoreRegistersInstruction(uint8_t up_to_register)
            : mUpToRegister(up_to_register)
        {
        }

        void execute(cpu_t &cpu) const override {
            auto byte_count = mUpToRegister + static_cast<uint16_t>(1);
            std::copy_n(cpu.memory.begin() + cpu.I, byte_count, cpu.V.begin());
            cpu.I = cpu.I + byte_count + static_cast<uint16_t>(1);
        }

        std::string toString() const override {
            return "READ V" + std::to_string(mUpToRegister);
        }

    private:
        uint8_t mUpToRegister;
    };
}
