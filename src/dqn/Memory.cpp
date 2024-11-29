#include "Memory.hpp"


// sample from Memory
EpisodeParameter Memory::sample()
{
    if (size == 0) {
        throw std::runtime_error("Memory is empty, cannot sample.");
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, size - 1); // Use only valid indices
    return ReplayMemory[dis(gen)];
};

// Add a new entry to the memory
void Memory::append(EpisodeParameter ep)
{
    ReplayMemory[highest_index] = ep;
    highest_index = (highest_index + 1) % ReplayMemory.size(); // Wrap around
    if (size < ReplayMemory.size()) {
        ++size; // Increase size up to the capacity
    }
};