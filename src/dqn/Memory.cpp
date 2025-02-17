#include "Memory.hpp"
#include <iostream>
#include <tuple>
#include <algorithm>
 

Memory::Memory()
    : highest_index(-1), gen(rd()), dis(0, 0)
{
    ReplayMemory.reserve(100000);
}

// sample from Memory return EpisodeParameter and next state
const std::tuple< EpisodeParameter, Eigen::Matrix<float, 6, 1>, bool> Memory::sample()
{
    size_t index = dis(gen);
    EpisodeParameter ep = ReplayMemory[index];
    Eigen::Matrix<float, 6, 1> state = ReplayMemory[(index + 1) % ReplayMemory.capacity()].pongVariables;
    bool ended = ReplayMemory[(index + 1) % ReplayMemory.capacity()].gameEnd;
    return {ep, state, ended};
};

void Memory::append(const EpisodeParameter& ep)
{
    highest_index = (highest_index + 1) % ReplayMemory.capacity(); // Wrap around
    ReplayMemory[highest_index] = ep;

    // dis = std::uniform_int_distribution<size_t>(0, std::min(static_cast<int>(ReplayMemory.size() - 2), highest_index));
    dis.param(std::uniform_int_distribution<size_t>::param_type(0, ReplayMemory.size() == highest_index ? highest_index : ReplayMemory.size()));
}   

EpisodeParameter& Memory::getCurrent()
{
    return ReplayMemory[highest_index];
};


EpisodeParameter& Memory::operator[](size_t index)
{
    return ReplayMemory[index % ReplayMemory.capacity()];
}

const EpisodeParameter& Memory::operator[](size_t index) const
{
    return ReplayMemory[index % ReplayMemory.capacity()];
}



EpisodeParameter& Memory::getNext()
{
    highest_index = (highest_index + 1) % ReplayMemory.capacity();
    // dis = std::uniform_int_distribution<size_t>(0, std::min(static_cast<int>(ReplayMemory.size() - 2), highest_index));
    dis.param(std::uniform_int_distribution<size_t>::param_type(0, ReplayMemory.size() == highest_index ? highest_index : ReplayMemory.size()));

    return ReplayMemory[highest_index];
}
