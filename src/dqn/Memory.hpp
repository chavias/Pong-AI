#pragma once
#include "../pong/EpisodeParameter.hpp"
#include <array>
#include <random>
#include <stdexcept>

class Memory
{
// it might be a good idea to fill initialize the memory with the starting episode
public:
    void append(const EpisodeParameter& ep);
    // get random sample from the memory array
    const EpisodeParameter& sample();
    // get latest sample from the memory array
    EpisodeParameter& getNext();

    EpisodeParameter& getCurrent();
    
    std::array<EpisodeParameter, 100000> ReplayMemory;
    int highest_index = -1;

private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis{0, highest_index-1}; // Use only valid indices
};
