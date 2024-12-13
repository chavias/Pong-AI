#include "Memory.hpp"
#include <iostream>

Memory::Memory()
    : highest_index(-1), gen(rd()), dis(0, 0)
{
    ReplayMemory.reserve(1000000);
}


// sample from Memory
const EpisodeParameter& Memory::sample()
{
    return ReplayMemory[dis(gen)];
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
    dis = std::uniform_int_distribution<>(0, std::min(static_cast<int>(ReplayMemory.capacity() - 1), highest_index));
}

EpisodeParameter& Memory::getCurrent()
{
    return ReplayMemory[highest_index];
};

EpisodeParameter& Memory::getNext()
{
    highest_index = (highest_index + 1) % ReplayMemory.capacity();
    return ReplayMemory[highest_index];
}
