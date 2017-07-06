#pragma clang diagnostic push
#pragma ide diagnostic ignored "ClangTidyInspection"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE "instructions"

#include <algorithm>
#include <iostream>

#include <boost/test/unit_test.hpp>

#include "cpu.hpp"
#include "instructions.hpp"

BOOST_AUTO_TEST_CASE(test_store_in_register) {
    chip8::StoreInVxInstruction storeInVxInstruction{5, 123};
    BOOST_CHECK_EQUAL(storeInVxInstruction.toString(), "MOV V5, 0x7b");

    chip8::cpu_t cpu;
    cpu.reset();
    storeInVxInstruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[0], 0);
    BOOST_CHECK_EQUAL(cpu.V[1], 0);
    BOOST_CHECK_EQUAL(cpu.V[2], 0);
    BOOST_CHECK_EQUAL(cpu.V[3], 0);
    BOOST_CHECK_EQUAL(cpu.V[4], 0);
    BOOST_CHECK_EQUAL(cpu.V[5], 123);
    BOOST_CHECK_EQUAL(cpu.V[6], 0);
    BOOST_CHECK_EQUAL(cpu.V[7], 0);
    BOOST_CHECK_EQUAL(cpu.V[8], 0);
    BOOST_CHECK_EQUAL(cpu.V[9], 0);
    BOOST_CHECK_EQUAL(cpu.V[10], 0);
    BOOST_CHECK_EQUAL(cpu.V[11], 0);
    BOOST_CHECK_EQUAL(cpu.V[12], 0);
    BOOST_CHECK_EQUAL(cpu.V[13], 0);
    BOOST_CHECK_EQUAL(cpu.V[14], 0);
    BOOST_CHECK_EQUAL(cpu.V[15], 0);

    BOOST_CHECK_EQUAL(cpu.pc, 0x200);
    BOOST_CHECK_EQUAL(cpu.I, 0x0);
    BOOST_CHECK_EQUAL(cpu.delayTimer, 0);
    BOOST_CHECK_EQUAL(cpu.soundTimer, 0);
}

BOOST_AUTO_TEST_CASE(load_delay_timer_instruction) {
    chip8::LoadDelayTimerInstruction instruction{2};
    BOOST_CHECK_EQUAL(instruction.toString(), "LOADD V2");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.V[2] = 0x20;
    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.delayTimer, 0x20);
}

BOOST_AUTO_TEST_CASE(store_delay_timer_instruction) {
    chip8::StoreDelayTimerInstruction instruction{3};
    BOOST_CHECK_EQUAL(instruction.toString(), "MOVED V3");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.delayTimer = 0x4f;
    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[3], 0x4f);
}

BOOST_AUTO_TEST_CASE(subtract_instruction) {
    chip8::SubtractInstruction instruction{3, 4};
    BOOST_CHECK_EQUAL(instruction.toString(), "SUB V3, V4");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.V[3] = 2;
    cpu.V[4] = 10;
    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[3], 8);
    BOOST_CHECK_EQUAL(cpu.V[4], 10);
}

BOOST_AUTO_TEST_CASE(increment_instruction) {
    chip8::IncrementInstruction instruction{5, 3};
    BOOST_CHECK_EQUAL(instruction.toString(), "INC V5, 0x3");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.V[5] = 1;
    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[5], 4);
}

BOOST_AUTO_TEST_CASE(clear_screen_instruction) {
    chip8::ClearScreenInstruction instruction;
    BOOST_CHECK_EQUAL(instruction.toString(), "CLS");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.fb[12] = true;
    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.fb[12], false);
}

BOOST_AUTO_TEST_CASE(bitwise_and_instruction) {
    chip8::BitwiseAndInstruction instruction{5, 2};
    BOOST_CHECK_EQUAL(instruction.toString(), "AND V5, V2");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.V[5] = 0x7B;
    cpu.V[2] = 0x0F;
    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[5], 0xB);
    BOOST_CHECK_EQUAL(cpu.V[2], 0x0F);
}

BOOST_AUTO_TEST_CASE(bitwise_or_instruction) {
    chip8::BitwiseOrInstruction instruction{1, 9};
    BOOST_CHECK_EQUAL(instruction.toString(), "OR V1, V9");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.V[1] = 0xF0;
    cpu.V[9] = 0x0F;
    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[1], 0xFF);
    BOOST_CHECK_EQUAL(cpu.V[9], 0x0F);
}

BOOST_AUTO_TEST_CASE(bitwise_xor_instruction) {
    chip8::BitwiseXorInstruction instruction{1, 9};
    BOOST_CHECK_EQUAL(instruction.toString(), "XOR V1, V9");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.V[1] = 0xF0;
    cpu.V[9] = 0x0F;
    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[1], 0xFF);
    BOOST_CHECK_EQUAL(cpu.V[9], 0x0F);
}

BOOST_AUTO_TEST_CASE(system_call_instruction) {
    chip8::SystemCallInstruction systemCallInstruction{123};
    BOOST_CHECK_EQUAL(systemCallInstruction.toString(), "SYS 0x7b");

    chip8::cpu_t unused_cpu;
    unused_cpu.reset();

    chip8::cpu_t cpu;
    cpu.reset();
    systemCallInstruction.execute(cpu);


    BOOST_CHECK_EQUAL_COLLECTIONS(cpu.V.begin(), cpu.V.end(), unused_cpu.V.begin(), unused_cpu.V.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(
        cpu.memory.begin(), cpu.memory.end(),
        unused_cpu.memory.begin(), unused_cpu.memory.end());
}

BOOST_AUTO_TEST_CASE(call_instruction) {
    chip8::CallInstruction instruction{0x2123};
    BOOST_CHECK_EQUAL(instruction.toString(), "CALL 0x2123");

    chip8::cpu_t cpu;
    cpu.reset();
    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.pc, 0x2123);
}

BOOST_AUTO_TEST_CASE(jump_instruction) {
    chip8::JumpInstruction instruction{123};
    BOOST_CHECK_EQUAL(instruction.toString(), "JMP 0x7b");

    chip8::cpu_t cpu;
    cpu.reset();
    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.pc, 0x7b);
}

BOOST_AUTO_TEST_CASE(skip_if_equals_instruction) {
    chip8::SkipIfVxEqualsInstruction instruction{2, 5};
    BOOST_CHECK_EQUAL(instruction.toString(), "SKE V2, 0x5");

    chip8::cpu_t cpu;
    cpu.reset();
    uint16_t starting_point{cpu.pc};

    instruction.execute(cpu);
    BOOST_CHECK_EQUAL(cpu.pc, starting_point);

    cpu.V[2] = 5;

    instruction.execute(cpu);
    BOOST_CHECK_EQUAL(cpu.pc, starting_point + 2);
}

BOOST_AUTO_TEST_CASE(skip_if_not_equals_instruction) {
    chip8::SkipIfVxNotEqualInstruction instruction{3, 123};
    BOOST_CHECK_EQUAL(instruction.toString(), "SKNE V3, 0x7b");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.V[3] = 123;
    uint16_t starting_point{cpu.pc};

    instruction.execute(cpu);
    BOOST_CHECK_EQUAL(cpu.pc, starting_point);

    cpu.V[3] = 99;
    instruction.execute(cpu);
    BOOST_CHECK_EQUAL(cpu.pc, starting_point + 2);
}

BOOST_AUTO_TEST_CASE(skip_if_reg_equals_instruction) {
    chip8::SkipIfVxEqualsVyInstruction instruction{3, 9};
    BOOST_CHECK_EQUAL(instruction.toString(), "SKRE V3, V9");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.V[9] = 2;

    uint16_t starting_point{cpu.pc};

    instruction.execute(cpu);
    BOOST_CHECK_EQUAL(cpu.pc, starting_point);

    cpu.V[9] = 0;
    instruction.execute(cpu);
    BOOST_CHECK_EQUAL(cpu.pc, starting_point + 2);
}

BOOST_AUTO_TEST_CASE(move_instruction) {
    chip8::MoveInstruction moveInstruction{2, 9};
    BOOST_CHECK_EQUAL(moveInstruction.toString(), "MOV V2, V9");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.V[9] = 23;

    moveInstruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[2], cpu.V[9]);
}

BOOST_AUTO_TEST_CASE(skip_if_register_not_equals_instruction) {
    chip8::SkipIfVxNotEqualsVyInstruction instruction{2, 3};
    BOOST_CHECK_EQUAL(instruction.toString(), "SKRNE V2, V3");

    chip8::cpu_t cpu;
    cpu.reset();
    uint16_t starting_point{cpu.pc};

    instruction.execute(cpu);
    BOOST_CHECK_EQUAL(cpu.pc, starting_point);

    cpu.V[2] = 5;
    instruction.execute(cpu);
    BOOST_CHECK_EQUAL(cpu.pc, starting_point + 2);
}

BOOST_AUTO_TEST_CASE(store_i_register_instruction) {
    chip8::LoadIndexInstruction instruction{123};
    BOOST_CHECK_EQUAL(instruction.toString(), "LOADI 0x7b");

    chip8::cpu_t cpu;
    cpu.reset();

    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.I, 123);
}

BOOST_AUTO_TEST_CASE(jump_I_instruction) {
    chip8::JumpIndexInstruction instruction{1239};
    BOOST_CHECK_EQUAL(instruction.toString(), "JUMPI 0x4d7");

    chip8::cpu_t cpu;
    cpu.reset();

    cpu.V[0] = 1;

    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.pc, 1240);
}

BOOST_AUTO_TEST_CASE(stor_instruction) {
    chip8::StoreRegistersInstruction instruction{5};
    BOOST_CHECK_EQUAL(instruction.toString(), "STOR V5");

    chip8::cpu_t cpu;
    cpu.reset();

    cpu.V[0] = 99;
    cpu.V[1] = 98;
    cpu.V[2] = 97;
    cpu.V[3] = 96;
    cpu.V[4] = 95;
    cpu.V[5] = 94;

    cpu.I = 0x900;

    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.memory[0x900], 99);
    BOOST_CHECK_EQUAL(cpu.memory[0x901], 98);
    BOOST_CHECK_EQUAL(cpu.memory[0x902], 97);
    BOOST_CHECK_EQUAL(cpu.memory[0x903], 96);
    BOOST_CHECK_EQUAL(cpu.memory[0x904], 95);
    BOOST_CHECK_EQUAL(cpu.memory[0x905], 94);
    BOOST_CHECK_EQUAL(cpu.memory[0x906], 0);

    BOOST_CHECK_EQUAL(cpu.I, 0x906);
}

BOOST_AUTO_TEST_CASE(read_instruction) {
    chip8::RestoreRegistersInstruction instruction{6};
    BOOST_CHECK_EQUAL(instruction.toString(), "READ V6");

    chip8::cpu_t cpu;
    cpu.reset();

    cpu.memory[0x900] = 99;
    cpu.memory[0x901] = 98;
    cpu.memory[0x902] = 97;
    cpu.memory[0x903] = 96;
    cpu.memory[0x904] = 95;
    cpu.memory[0x905] = 94;

    cpu.I = 0x900;

    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[0], 99);
    BOOST_CHECK_EQUAL(cpu.V[1], 98);
    BOOST_CHECK_EQUAL(cpu.V[2], 97);
    BOOST_CHECK_EQUAL(cpu.V[3], 96);
    BOOST_CHECK_EQUAL(cpu.V[4], 95);
    BOOST_CHECK_EQUAL(cpu.V[5], 94);
    BOOST_CHECK_EQUAL(cpu.V[6], 0);
}

BOOST_AUTO_TEST_CASE(bcd_instruction) {
    chip8::StoreDecimalInstruction instruction{3};
    BOOST_CHECK_EQUAL(instruction.toString(), "BCD V3");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.I = 0x900;
    cpu.V[3] = 139;

    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.memory[0x900], 1);
    BOOST_CHECK_EQUAL(cpu.memory[0x901], 3);
    BOOST_CHECK_EQUAL(cpu.memory[0x902], 9);
}

BOOST_AUTO_TEST_CASE(shift_left_instruction) {
    chip8::ShiftLeftInstruction instruction{3, 5};
    BOOST_CHECK_EQUAL(instruction.toString(), "SHL V3, V5");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.V[3] = 64;
    cpu.V[5] = 1;

    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[3], 128);
    BOOST_CHECK_EQUAL(cpu.V[5], 1);
    BOOST_CHECK_EQUAL(cpu.V[15], 0);

    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[3], 0);
    BOOST_CHECK_EQUAL(cpu.V[5], 1);
    BOOST_CHECK_EQUAL(cpu.V[15], 1);
}

BOOST_AUTO_TEST_CASE(shift_right_instruction) {
    chip8::ShiftRightInstruction instruction{3, 5};
    BOOST_CHECK_EQUAL(instruction.toString(), "SHR V3, V5");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.V[3] = 2;
    cpu.V[5] = 1;

    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[3], 1);
    BOOST_CHECK_EQUAL(cpu.V[5], 1);
    BOOST_CHECK_EQUAL(cpu.V[15], 0);

    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[3], 0);
    BOOST_CHECK_EQUAL(cpu.V[5], 1);
    BOOST_CHECK_EQUAL(cpu.V[15], 1);
}

template<typename T>
class MockRandomNumberGenerator : public chip8::RandomNumberGenerator<T>
{
public:
    MockRandomNumberGenerator(T value)
        : mValue(value)
    {
    }

    T getNext() override
    {
        return mValue;
    }

private:
    T mValue;
};

BOOST_AUTO_TEST_CASE(random_instruction) {
    auto rng = std::make_shared<MockRandomNumberGenerator<uint8_t>>(9);
    chip8::StoreRandomWithMaskInstruction instruction{2, 255, rng};
    BOOST_CHECK_EQUAL(instruction.toString(), "RND V2, 255");

    chip8::cpu_t cpu;
    cpu.reset();
    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[2], 9);
}

BOOST_AUTO_TEST_CASE(set_sound_timer_instruction) {
    chip8::SetSoundTimerInstruction instruction{7};
    BOOST_CHECK_EQUAL(instruction.toString(), "LOADS V7");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.V[7] = 0x80;
    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.soundTimer, 0x80);
}

BOOST_AUTO_TEST_CASE(wait_for_key_press_instruction) {
    chip8::WaitForKeypressInstruction instruction{1};
    BOOST_CHECK_EQUAL(instruction.toString(), "KEYD");

    // TODO: Implement KEYD
}

BOOST_AUTO_TEST_CASE(draw_sprite_instruction) {
    chip8::DrawSpriteInstruction instruction{1, 2, 5};
    BOOST_CHECK_EQUAL(instruction.toString(), "DRW V1, V2, 5");

    std::array<uint8_t, 5> sprite = {
        0b00011000,
        0b00011000,
        0b11111111,
        0b00011000,
        0b00011000,
    };

    chip8::cpu_t cpu;
    cpu.reset();

    // Write the test sprite into main memory at address 0x40
    std::copy(sprite.begin(), sprite.end(), cpu.memory.begin() + 0x40);

    // Draw the sprite at (10, 10) on the display
    cpu.V[1] = 10;
    cpu.V[2] = 10;
    cpu.I = 0x40;

    instruction.execute(cpu);

    // There should have been no collissions since the frame buffer was empty before.
    BOOST_CHECK_EQUAL(cpu.V[15], 0);

    // Ensure that the frame buffer has this contents (converted from a string for convenience representation in this
    // file).
    std::string expected_frame_buffer_str =
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000110000000000000000000000000000000000000000000000000"
        "0000000000000110000000000000000000000000000000000000000000000000"
        "0000000000111111110000000000000000000000000000000000000000000000"
        "0000000000000110000000000000000000000000000000000000000000000000"
        "0000000000000110000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000";

    std::array<uint8_t, 64 * 32> expected_frame_buffer;
    std::transform(
        expected_frame_buffer_str.begin(), expected_frame_buffer_str.end(),
        expected_frame_buffer.begin(),
        [](char c) { return c == '1'; });

    BOOST_CHECK_EQUAL_COLLECTIONS(
        cpu.fb.begin(), cpu.fb.end(),
        expected_frame_buffer.begin(), expected_frame_buffer.end());

    // Now draw the same sprite again at (9, 9)
    cpu.V[1] = 9;
    cpu.V[2] = 9;
    instruction.execute(cpu);

    // There should have been a collision
    BOOST_CHECK_EQUAL(cpu.V[15], 1);

    // Ensure that the frame buffer now looks like this
    std::string expected_frame_buffer_str_2 =
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000001100000000000000000000000000000000000000000000000000"
        "0000000000001010000000000000000000000000000000000000000000000000"
        "0000000001111001100000000000000000000000000000000000000000000000"
        "0000000000110011110000000000000000000000000000000000000000000000"
        "0000000000001010000000000000000000000000000000000000000000000000"
        "0000000000000110000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000";

    std::transform(
        expected_frame_buffer_str_2.begin(), expected_frame_buffer_str_2.end(),
        expected_frame_buffer.begin(),
        [](char c) { return c == '1'; });
    BOOST_CHECK_EQUAL_COLLECTIONS(
        cpu.fb.begin(), cpu.fb.end(),
        expected_frame_buffer.begin(), expected_frame_buffer.end());
}

BOOST_AUTO_TEST_CASE(add_instruction) {
    chip8::AddInstruction instruction{1, 2};
    BOOST_CHECK_EQUAL(instruction.toString(), "ADD V1, V2");

    chip8::cpu_t cpu;
    cpu.reset();
    cpu.V[1] = 252;
    cpu.V[2] = 2;

    instruction.execute(cpu);
    BOOST_CHECK_EQUAL(cpu.V[1], 254);
    BOOST_CHECK_EQUAL(cpu.V[15], 0);

    instruction.execute(cpu);

    BOOST_CHECK_EQUAL(cpu.V[1], 0);
    BOOST_CHECK_EQUAL(cpu.V[15], 1);

}

#pragma clang diagnostic pop
