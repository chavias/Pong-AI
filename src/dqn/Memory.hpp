#pragma once
#include "../pong/EpisodeParameter.hpp"
// #include <array>
#include <vector>
#include <random>
#include <stdexcept>

class Memory
{
public:
    Memory();

    void append(const EpisodeParameter& ep);
    const EpisodeParameter& sample();
    EpisodeParameter& getNext();
    EpisodeParameter& getCurrent();

    int highest_index;
private:
    std::vector<EpisodeParameter> ReplayMemory;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;
};