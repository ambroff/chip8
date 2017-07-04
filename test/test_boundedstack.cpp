#pragma clang diagnostic push
#pragma ide diagnostic ignored "ClangTidyInspection"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE "boundedstack"

#include <boost/test/unit_test.hpp>

#include "boundedstack.hpp"

BOOST_AUTO_TEST_CASE(boundedstack) {
    chip8::boundedstack<uint16_t, 3> boundedstack;

    BOOST_CHECK_EQUAL(boundedstack.max_size(), 3);

    boundedstack.push(5);
    boundedstack.push(4);

    BOOST_CHECK_EQUAL(boundedstack.top(), 4);
    boundedstack.pop();

    BOOST_CHECK_EQUAL(boundedstack.top(), 5);
    boundedstack.pop();

    BOOST_CHECK_EQUAL(boundedstack.size(), 0);
}

#pragma clang diagnostic pop
