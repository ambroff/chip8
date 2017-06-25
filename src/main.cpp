/**
 * Chip8 Interpreter. Made following documentation here:
 *
 *  - http://mattmik.com/files/chip8/mastering/chip8.html
 *  - http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */
#include <array>
#include <iostream>

namespace {
    class Register final {
    public:
        uint8_t read() const {
            return mValue;
        }

        void write(uint8_t value) {
            mValue = value;
        }

    private:
        uint8_t mValue{0};
    };

    enum class OpCode : uint16_t {
    };

    struct CPU final {
        std::array<uint8_t, 128> mainMemory;
        std::array<Register, 16> vxRegisters;
        uint16_t pcRegister;
    };

    class Instruction {
    public:
        virtual void execute(CPU& cpu) const = 0;
    };
}

int main() {
    // 64 x 32 pixel display
    const int GRAPHICS_SCALE_FACTOR{3};

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
