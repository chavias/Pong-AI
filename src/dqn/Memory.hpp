#pragma once
#include "../pong/EpisodeParameter.hpp"
#include <array>
#include <random>
#include <stdexcept>

class Memory
{
public:
    void append(EpisodeParameter);
    EpisodeParameter sample(); // get random sample from the memory array

private:
    std::array<EpisodeParameter, 100000> ReplayMemory;
    int highest_index = 0;
    int size = 0;
};
