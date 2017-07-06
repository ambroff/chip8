#pragma once

namespace chip8 {
    const uint16_t FONT_DATA_OFFSET{0};

    enum class CharacterSprite : uint16_t {
        NUMERAL_0 = FONT_DATA_OFFSET,
        NUMERAL_1 = FONT_DATA_OFFSET +  5,
        NUMERAL_2 = FONT_DATA_OFFSET + 10,
        NUMERAL_3 = FONT_DATA_OFFSET + 15,
        NUMERAL_4 = FONT_DATA_OFFSET + 20,
        NUMERAL_5 = FONT_DATA_OFFSET + 25,
        NUMERAL_6 = FONT_DATA_OFFSET + 30,
        NUMERAL_7 = FONT_DATA_OFFSET + 35,
        NUMERAL_8 = FONT_DATA_OFFSET + 40,
        NUMERAL_9 = FONT_DATA_OFFSET + 45,
        NUMERAL_A = FONT_DATA_OFFSET + 50,
        NUMERAL_B = FONT_DATA_OFFSET + 55,
        NUMERAL_C = FONT_DATA_OFFSET + 60,
        NUMERAL_D = FONT_DATA_OFFSET + 65,
        NUMERAL_E = FONT_DATA_OFFSET + 70,
        NUMERAL_F = FONT_DATA_OFFSET + 75
    };

    const std::array<uint8_t, 5 * 16> FONT_DATA{
        0xF0, // ****
        0x90, // *  *
        0x90, // *  *
        0x90, // *  *
        0xF0, // ****

        0x20, //   *
        0x60, //  **
        0x20, //   *
        0x20, //   *
        0x70, //  ***

        0xF0, // ****
        0x10, //    *
        0xF0, // ****
        0x80, // *
        0xF0, // ****

        0xF0, // ****
        0x10, //    *
        0xF0, // ****
        0x10, //    *
        0xF0, // ****

        0x90, // *  *
        0x90, // *  *
        0xF0, // ****
        0x10, //    *
        0x10, //    *

        0xF0, // ****
        0x80, // *
        0xF0, // ****
        0x10, //    *
        0xF0, // ****

        0xF0, // ****
        0x80, // *
        0xF0, // ****
        0x90, // *  *
        0xF0, // ****

        0xF0, // ****
        0x10, //    *
        0x20, //   *
        0x40, //  *
        0x40, //  *

        0xF0, // ****
        0x90, // *  *
        0xF0, // ****
        0x90, // *  *
        0xF0, // ****

        0xF0, // ****
        0x90, // *  *
        0xF0, // ****
        0x10, //    *
        0xF0, // ****

        0xF0, // ****
        0x90, // *  *
        0xF0, // ****
        0x90, // *  *
        0x90, // *  *

        0xE0, // ***
        0x90, // *  *
        0xE0, // ***
        0x90, // *  *
        0xE0, // ***

        0xF0, // ****
        0x80, // *
        0x80, // *
        0x80, // *
        0xF0, // ****

        0xE0, // ***
        0x90, // *  *
        0x90, // *  *
        0x90, // *  *
        0xE0, // ***

        0xF0, // ****
        0x80, // *
        0xF0, // ****
        0x80, // *
        0xF0, // ****

        0xF0, // ****
        0x80, // *
        0xF0, // ****
        0x80, // *
        0x80  // *
    };

}
