#pragma once

#include <memory>

#include "instructions.hpp"

namespace chip8 {

inline std::unique_ptr<Instruction> decode_opcode(uint16_t opcode)
{
    if (opcode == 0x00E0) {
        return std::make_unique<ClearScreenInstruction>();
    }

    if (opcode == 0x00EE) {
        return std::make_unique<ReturnInstruction>();
    }

    if ((opcode & 0x1000) != 0) {
        return std::make_unique<JumpInstruction>(static_cast<uint16_t>(opcode & ~0xF000));
    }

    if ((opcode & 0xF000) == 0x6000) {
        auto reg = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        auto value = static_cast<uint8_t>(opcode & 0x00FF);
        return std::make_unique<StoreInVxInstruction>(reg, value);
    }

    return {};
}

}
