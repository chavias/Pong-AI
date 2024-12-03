#include "Memory.hpp"

// sample from Memory
const EpisodeParameter& Memory::sample()
{
    return ReplayMemory[dis(gen)];
};

// Add a new entry to the memory
void Memory::append(const EpisodeParameter& ep)
{
    highest_index = (highest_index + 1) % ReplayMemory.size(); // Wrap around
    ReplayMemory[highest_index] = ep;
};

EpisodeParameter& Memory::getNext()
{
    highest_index = (highest_index + 1) % ReplayMemory.size();
    return ReplayMemory[highest_index];
};

EpisodeParameter& Memory::getCurrent()
{
    return ReplayMemory[highest_index];
};

// EpisodeParameter& Memory::getNext()
// {
//     highest_index = (highest_index + 1) % ReplayMemory.size();
//     return ReplayMemory[highest_index];
// }

// EpisodeParameter& Memory::getCurrent()
// {
//     return ReplayMemory[highest_index];
// }
