#include "Memory.hpp"
#include <iostream>
#include <tuple>
 

Memory::Memory()
    : highest_index(-1), gen(rd()), dis(0, 0)
{
    ReplayMemory.resize(100000);
}

// sample from Memory return EpisodeParameter and next state
const std::tuple< EpisodeParameter, Eigen::Matrix<float, 6, 1>, bool> Memory::sample()
{
    size_t index = dis(gen);
    EpisodeParameter ep = ReplayMemory[index];
    Eigen::Matrix<float, 6, 1> state = ReplayMemory[(index + 1) % ReplayMemory.size()].pongVariables;
    bool ended = ReplayMemory[(index + 1) % ReplayMemory.size()].gameEnd;
    return {ep , state, ended};
};

void Memory::append(const EpisodeParameter& ep)
{
    highest_index = (highest_index + 1) % ReplayMemory.size(); // Wrap around
    ReplayMemory[highest_index] = ep;

    dis = std::uniform_int_distribution<>(0, std::min(static_cast<int>(ReplayMemory.size() - 2), highest_index));
}

EpisodeParameter& Memory::getCurrent()
{
    return ReplayMemory[highest_index];
};


EpisodeParameter& Memory::operator[](size_t index)
{
    return ReplayMemory[index % ReplayMemory.size()];
}

const EpisodeParameter& Memory::operator[](size_t index) const
{
    return ReplayMemory[index % ReplayMemory.size()];
}



EpisodeParameter& Memory::getNext()
{
    highest_index = (highest_index + 1) % ReplayMemory.size();
    dis = std::uniform_int_distribution<>(0, std::min(static_cast<int>(ReplayMemory.size() - 2), highest_index));

    return ReplayMemory[highest_index];
}
