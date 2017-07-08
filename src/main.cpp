/**
 * Chip8 Interpreter. Made following documentation here:
 *
 *  - http://mattmik.com/files/chip8/mastering/chip8.html
 *  - http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */
#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <iterator>
#include <iostream>
#include <fstream>
#include <thread>

#include <boost/filesystem.hpp>

#include <SFML/Graphics.hpp>

#include "cpu.hpp"
#include "decode.hpp"
#include "font.hpp"

namespace chip8 {
    class Machine final {
    public:
        Machine(sf::RenderWindow& window, int scalingFactor)
            : mWindow(window),
              mScalingFactor(scalingFactor)
        {
        }

        void reset() {
            mCpu.reset();

            // Load the font
            std::copy(FONT_DATA.begin(), FONT_DATA.end(), mCpu.memory.begin() + FONT_DATA_OFFSET);
        }

        void loadProgram(std::ifstream &inputStream) {
            reset();
            std::copy(
                std::istream_iterator<uint8_t>(inputStream),
                std::istream_iterator<uint8_t>(),
                mCpu.memory.begin() + 0x200);
        }

        /**
         * Execute one instruction.
         */
        void step() {
            // Read the next opcode from main memory
            uint16_t opcode{static_cast<uint16_t>(mCpu.memory[mCpu.pc] << 8 | mCpu.memory[mCpu.pc + 1])};

            // Decode the instruction
            std::unique_ptr<Instruction> instruction = decode_opcode(opcode);
            if (!instruction) {
                std::cerr << "Unable to decode instruction: " << std::hex << opcode << std::endl;
                exit(1); // TODO: Use an exception here instead?
            }

            // Actually execute the instruction
            instruction->execute(mCpu);

            // Each instruction is two bytes long, so we need to advance by two bytes.
            mCpu.pc += 2;
        }

        int run() {
            using namespace std::chrono_literals;

            while (mWindow.isOpen()) {
                sf::Event event;
                while (mWindow.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        mWindow.close();
                    }
                }

                step();

                // TODO: only render if shouldDraw flag is set?
                render(mCpu.fb, mWindow);

                mWindow.display();

                std::this_thread::sleep_for(1s);
            }

            return 0;
        }

        void render(std::array<bool, 64 * 32>& fb, sf::RenderTarget& renderTarget) {
            mWindow.clear(sf::Color::Black);

            auto viewportSize = renderTarget.getDefaultView().getSize();

            auto pixelWidth = viewportSize.x / mScalingFactor;
            auto pixelHeight = viewportSize.y / mScalingFactor;
            auto pixelDimensions = sf::Vector2f{pixelWidth, pixelHeight};

            sf::RectangleShape rectangleShape{pixelDimensions};
            rectangleShape.setFillColor(sf::Color::White);

            for (int i = 0; i < fb.size(); i++) {
                //rectangleShape.setPosition()
                renderTarget.draw(rectangleShape);
            }
        }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
        void dumpCore(std::ostream& outputStream) {
            mCpu.dumpState(outputStream);
        }
#pragma clang diagnostic pop

    private:
        cpu_t mCpu;
        sf::RenderWindow& mWindow;
        int mScalingFactor;
    };
}

int main() {
    const std::string PROGRAM{"data/games/BREAKOUT"};

    auto program = boost::filesystem::canonical(PROGRAM);

    // 64 x 32 pixel display
    const int GRAPHICS_SCALE_FACTOR{2};

    sf::RenderWindow window{sf::VideoMode{1024, 768}, "Chip8: " + program.string()};

    std::ifstream inputStream;
    inputStream.open(program.string(), std::ios::binary);
    if (!inputStream.is_open()) {
        std::cerr << "Unable to open file: " << std::strerror(errno) << std::endl;
        return 1;
    }

    chip8::Machine machine{window, GRAPHICS_SCALE_FACTOR};
    machine.loadProgram(inputStream);
    return machine.run();
}
