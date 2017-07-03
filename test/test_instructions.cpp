#pragma clang diagnostic push
#pragma ide diagnostic ignored "ClangTidyInspection"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE "instructions"

#include <boost/test/unit_test.hpp>
#include <iostream>

#include "cpu.hpp"
#include "instructions.hpp"

BOOST_AUTO_TEST_CASE(test_store_in_register) {
    chip8::StoreInVxInstruction storeInVxInstruction{5, 123};
    BOOST_CHECK_EQUAL(storeInVxInstruction.toString(), "MOV V5, 123");

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
    BOOST_CHECK_EQUAL(instruction.toString(), "INC V5, 3");

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

#pragma clang diagnostic pop
