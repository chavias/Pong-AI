#include "Memory.hpp"
#include <iostream>
#include <tuple>
 

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
    return {ep , state, ended};
};

// void Memory::append(const EpisodeParameter& ep)
// {
//     ReplayMemory.emplace_back(ep);
// }


// Add a new entry to the memory
// void Memory::append(const EpisodeParameter& ep)
// {
//     highest_index = (highest_index + 1) % ReplayMemory.capacity(); // Wrap around
//     ReplayMemory[highest_index] = ep;
// };

void Memory::append(const EpisodeParameter& ep)
{
    highest_index = (highest_index + 1) % ReplayMemory.capacity(); // Wrap around
    ReplayMemory[highest_index] = ep;

    // Update random distribution range
    dis = std::uniform_int_distribution<>(0, std::min(static_cast<int>(ReplayMemory.capacity() - 2), highest_index));
}

EpisodeParameter& Memory::getCurrent()
{
    return ReplayMemory[highest_index];
};


EpisodeParameter& operator[](size_t index)
{
    return ReplayMemory[index % capacity];
}

const EpisodeParameter& operator[](size_t index) const
{
    return ReplayMemory[index % capacity];
}



EpisodeParameter& Memory::getNext()
{
    highest_index = (highest_index + 1) % ReplayMemory.capacity();
    return ReplayMemory[highest_index];
}
