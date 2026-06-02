#include "randomizer.h"
#include <algorithm>
#include <random>
#include <chrono>

namespace core {
    Randomizer::Randomizer(RandomMode mode) : mode(mode), currentIndex(0) {}

    void Randomizer::initialize(const std::vector<std::string>& names) {
        randomIndices.clear();
        currentIndex = 0;
        
        // Create indices from 0 to names.size()-1
        for (size_t i = 0; i < names.size(); i++) {
            randomIndices.push_back(i);
        }
        
        // Shuffle the indices
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(randomIndices.begin(), randomIndices.end(), g);
    }

    size_t Randomizer::getNextIndex() {
        if (currentIndex >= randomIndices.size()) {
            return randomIndices.size(); // Indicate end
        }
        return randomIndices[currentIndex++];
    }

    size_t Randomizer::peekNextIndex() const {
        if (currentIndex >= randomIndices.size()) {
            return randomIndices.size();
        }
        return randomIndices[currentIndex];
    }

    bool Randomizer::hasNext() const {
        return currentIndex < randomIndices.size();
    }

    void Randomizer::reset() {
        currentIndex = 0;
    }

    size_t Randomizer::getRemainingCount() const {
        return randomIndices.size() - currentIndex;
    }
}