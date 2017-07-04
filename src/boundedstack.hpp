#pragma once

#include <stack>

namespace chip8 {
//    template <typename T, std::size_t Capacity>
//    class appendable_array : public std::array<T, Capacity> {
//        using base = std::array<T, Capacity>;
//        using base::base;
//        using base::operator=;
//
//    public:
//        void push_back(T value)
//        {
//            this->at(mIdx--) = value;
//        }
//
//        T pop_back()
//        {
//            return this->at(mIdx++);
//        }
//
//        std::size_t size() const {
//            return Capacity - mIdx + 1;
//        }
//
//    private:
//        std::size_t mIdx{Capacity - 1};
//    };
//    template <typename T, std::size_t Capacity>
//    class appendable_array {
//    public:
//        using value_type = T;
//
//        T pop_back() {
//        }
//
//        void push_back(const T& value) {
//        }
//
//        std::size_t size() const {
//            return mIdx;
//        }
//
//        std::size_t max_size() const {
//            return Capacity;
//        }
//
//    private:
//        std::size_t mIdx{0};
//        std::array<T, Capacity> mData;
//    };
//
//    template<typename T, std::size_t Capacity>
//    class boundedstack : public std::stack<T, appendable_array<T, Capacity>> {
//        using base = std::stack<T, appendable_array<T, Capacity>>;
//        using base::base;
//        using base::operator=;
//
//    public:
//        std::size_t max_size() const
//        {
//            return base::c.max_size();
//        }
//    };
    template <typename T, std::size_t Capacity>
    class boundedstack {
    public:
        void push(const T& value) {
            mData[mIdx++] = value;
        }

        void pop() {
            mIdx--;
        }

        const T& top() const {
            return mData[mIdx - 1];
        }

        std::size_t size() const {
            return mIdx;
        }

        std::size_t max_size() const {
            return Capacity;
        }

        void clear() {
            mIdx = 0;
        }

    private:
        std::array<T, Capacity> mData;
        std::size_t mIdx{0};
    };
}
