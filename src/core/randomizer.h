#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <vector>
#include <random>

namespace core {
    enum class RandomMode {
        ALL_RANDOM,
        ONE_BY_ONE
    };

    class Randomizer {
    private:
        RandomMode mode;
        std::vector<size_t> randomIndices;
        size_t currentIndex;
        
    public:
        Randomizer(RandomMode mode);
        void initialize(const std::vector<std::string>& names);
        size_t getNextIndex();
        size_t peekNextIndex() const;
        bool hasNext() const;
        void reset();
        size_t getRemainingCount() const;
    };
}

#endif // RANDOMIZER_H