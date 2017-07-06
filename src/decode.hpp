#pragma once

#include <memory>

#include "instructions.hpp"

namespace chip8 {

inline std::unique_ptr<Instruction> decode_opcode(uint16_t opcode)
{
    switch (opcode & 0xF000) {
        case 0x0000:
            // NB: SYS instruction (0x0NNN) is not implemented, since apparently nobody implements this.
            // Also security lol.
            if (opcode == 0x00E0) {
                return std::make_unique<ClearScreenInstruction>();
            }

            if (opcode == 0x00EE) {
                return std::make_unique<ReturnInstruction>();
            }

            break;

        case 0x1000:
            return std::make_unique<JumpInstruction>(opcode & 0x0FFF);

        case 0x2000:
            return std::make_unique<CallInstruction>(opcode & 0x0FFF);

        case 0x3000: {
            auto reg = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto val = static_cast<uint8_t>(opcode & 0x00FF);
            return std::make_unique<SkipIfVxEqualsInstruction>(reg, val);
        }

        case 0x4000: {
            auto reg = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto val = static_cast<uint8_t>(opcode & 0x00FF);
            return std::make_unique<SkipIfVxNotEqualInstruction>(reg, val);
        }

        case 0x5000: {
            // 0x5XY0
            auto reg_x = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto reg_y = static_cast<uint8_t>((opcode & 0x00F0) >> 4);
            return std::make_unique<SkipIfVxEqualsVyInstruction>(reg_x, reg_y);
        }

        case 0x6000: {
            auto reg = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto val = static_cast<uint8_t>(opcode & 0x00FF);
            return std::make_unique<StoreInVxInstruction>(reg, val);
        }

        case 0x7000: {
            auto reg = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto val = static_cast<uint8_t>(opcode & 0x00FF);
            return std::make_unique<IncrementInstruction>(reg, val);
        }

        case 0x8000: {
            auto reg_x = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto reg_y = static_cast<uint8_t>((opcode & 0x0F00) >> 4);

            switch (opcode & 0x000F) {
                case 0:
                    return std::make_unique<MoveInstruction>(reg_x, reg_y);

                case 1:
                    return std::make_unique<BitwiseOrInstruction>(reg_x, reg_y);

                case 2:
                    return std::make_unique<BitwiseAndInstruction>(reg_x, reg_y);

                case 3:
                    return std::make_unique<BitwiseXorInstruction>(reg_x, reg_y);

                case 4:
                    return std::make_unique<AddInstruction>(reg_x, reg_y);

                case 5:
                    return std::make_unique<DecrementInstruction>(reg_x, reg_y);

                case 6:
                    return std::make_unique<ShiftRightInstruction>(reg_x, reg_y);

                case 7:
                    return std::make_unique<SubtractInstruction>(reg_x, reg_y);

                case 0xE:
                    return std::make_unique<ShiftLeftInstruction>(reg_x, reg_y);

                default:
                    break;
            }

            break;
        }

        case 0x9000: {
            // 0x9XY0
            auto reg_x = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto reg_y = static_cast<uint8_t>((opcode & 0x00F0) >> 4);
            return std::make_unique<SkipIfVxNotEqualsVyInstruction>(reg_x, reg_y);
        }

        case 0xA000: {
            // 0xANNN
            auto address = opcode & 0x0FFF;
            return std::make_unique<LoadIndexInstruction>(address);
        }

        case 0xB000: {
            // 0xBNNN
            auto address = opcode & 0x0FFF;
            return std::make_unique<JumpIndexInstruction>(address);
        }

        case 0xC000: {
            // 0xCXNN
            auto reg = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto val = static_cast<uint8_t>(opcode & 0x00FF);
            return std::make_unique<StoreRandomWithMaskInstruction>(reg, val);
        }

        case 0xD000: {
            // 0xDXYN
            auto reg_x = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto reg_y = static_cast<uint8_t>((opcode & 0x00F0) >> 4);
            auto bytes = static_cast<uint8_t>(opcode & 0x000F);
            return std::make_unique<DrawSpriteInstruction>(reg_x, reg_y, bytes);
        }

        case 0xE000: {
            auto reg = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            switch (opcode & 0x00FF) {
                case 0x009E:
                    return std::make_unique<SkipIfKeyPressedInstruction>(reg);

                case 0x00A1:
                    return std::make_unique<SkipIfKeyNotPressedInstruction>(reg);

                default:
                    break;
            }

            break;
        }

        case 0xF000: {
            auto reg = static_cast<uint8_t>((opcode & 0x0F00) >> 8);

            switch (opcode & 0x00FF) {
                // 0xFX07
                case 0x0007:
                    return std::make_unique<StoreDelayTimerInstruction>(reg);

                // 0xFX0A
                case 0x000A:
                    return std::make_unique<WaitForKeypressInstruction>(reg);

                // 0xFX15
                case 0x0015:
                    return std::make_unique<LoadDelayTimerInstruction>(reg);

                // 0xFX18
                case 0x0018:
                    return std::make_unique<SetSoundTimerInstruction>(reg);

                // 0xFX1E
                case 0x001E:
                    return std::make_unique<AddToIInstruction>(reg);

                // 0xFX29
                case 0x0029:
                    return std::make_unique<StoreInIInstruction>(reg);

                // 0xFX33
                case 0x0033:
                    return std::make_unique<StoreDecimalInstruction>(reg);

                // 0xFX55
                case 0x0055:
                    return std::make_unique<StoreRegistersInstruction>(reg);

                // 0xFX65
                case 0x0065:
                    return std::make_unique<RestoreRegistersInstruction>(reg);

                default:
                    break;
            }

            break;
        }

        default:
            break;

    }

    // Unhandled instruction
    return {};
}

}
