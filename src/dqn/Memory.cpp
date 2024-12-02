#include "Memory.hpp"

// sample from Memory
const EpisodeParameter& Memory::sample()
{
    if (size == 0) {
        throw std::runtime_error("Memory is empty, cannot sample.");
    }
    return ReplayMemory[dis(gen)];
};

// Add a new entry to the memory
void Memory::append(const EpisodeParameter& ep)
{
    ReplayMemory[highest_index] = ep;
    highest_index = (highest_index + 1) % ReplayMemory.size(); // Wrap around
    if (size < ReplayMemory.size()) {
        ++size; // Increase size up to the capacity
    }
};

EpisodeParameter& Memory::getCurrent()
{
    return ReplayMemory[highest_index];
    highest_index = (highest_index + 1) % ReplayMemory.size();
};
