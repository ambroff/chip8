#define BOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE "instructions"

#include <boost/test/unit_test.hpp>

#include "cpu.hpp"
#include "instructions.hpp"

BOOST_AUTO_TEST_CASE(test_store_in_register) {
    chip8::cpu_t cpu;
    chip8::StoreInVxInstruction storeInVxInstruction{5, 123};
    storeInVxInstruction.execute(cpu);

    BOOST_ASSERT(cpu.V[5] == 123);
}
