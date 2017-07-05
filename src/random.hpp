#pragma once

#include <boost/random.hpp>

namespace chip8 {
    template <typename T>
    class RandomNumberGenerator
    {
    public:
        virtual ~RandomNumberGenerator() = default;
        virtual T getNext() = 0;
    };

    template <typename T>
    class RealRandomNumberGenerator : public RandomNumberGenerator<T>
    {
    public:
        T getNext() override
        {
            return mDistribution(mImpl);
        }

    private:
        boost::random::mt19937 mImpl;
        boost::random::uniform_int_distribution<> mDistribution{0, 255};
    };
}
