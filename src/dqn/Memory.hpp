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
    const std::tuple< EpisodeParameter, Eigen::Matrix<float, 6, 1>, bool> sample();
    EpisodeParameter& getNext();
    EpisodeParameter& getCurrent();

    EpisodeParameter& operator[](size_t index);
    const EpisodeParameter& operator[](size_t index) const;
    
    int highest_index;
private:
    std::vector<EpisodeParameter> ReplayMemory;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;
};