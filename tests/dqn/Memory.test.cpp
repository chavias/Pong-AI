#include <gtest/gtest.h>
#include "../../src/dqn/Memory.hpp"
#include "../../src/pong/EpisodeParameter.hpp"  // Assuming this is the file where EpisodeParameter is defined
#include <iostream>
#include <memory>
#include <Eigen/Dense>

// Test for the Memory class
class MemoryTest : public ::testing::Test {
protected:
    // Define the memory capacity for the tests
    static constexpr size_t memory_capacity = 5;
    
    // Instance of Memory class
    std::unique_ptr<Memory> memory  = std::make_unique<Memory>();
    
    void SetUp() override
    {
        memory = std::make_unique<Memory>();  // Initialize the unique_ptr
    }

    // Add mock data to memory for the tests
    void fillMemory() {
        for (size_t i = 0; i < memory_capacity; ++i) {
            EpisodeParameter ep;
            ep.pongVariables.setZero();  // Initialize pongVariables
            ep.action1 = static_cast<Action>(i % 3);  // Cycle through actions
            ep.reward1 = static_cast<int>(i);  // Use the index as reward
            ep.reward2 = static_cast<int>(i + 1);  // Use a different value for reward2
            ep.gameEnd = (i == memory_capacity - 1);  // Mark the last entry as game end
            memory->append(ep);
        }
    }

        // Add mock data to memory for the tests
    void fillAllMemory() {
        for (size_t i = 0; i < 100000; ++i) {
            EpisodeParameter ep;
            ep.pongVariables.setZero();  // Initialize pongVariables
            ep.action1 = static_cast<Action>(i % 3);  // Cycle through actions
            ep.reward1 = static_cast<int>(i);  // Use the index as reward
            ep.reward2 = static_cast<int>(-i);  // Use a different value for reward2
            ep.gameEnd = (i == memory_capacity - 1);  // Mark the last entry as game end
            memory->append(ep);
        }
    }
};

// Test for sampling from non-empty memory
TEST_F(MemoryTest, GetHighestIndex) {
    fillMemory();  // Add data to memory
    // Expect the sample to return an EpisodeParameter with the correct data
    int index = memory->highest_index;
    std::cout << index << std::endl;
    EXPECT_EQ(index, 4);
}

// Test for appending data to the memory
TEST_F(MemoryTest, AppendDataToMemory) {
    EpisodeParameter ep;
    ep.pongVariables.setZero();
    ep.reward1 = 10;
    ep.reward2 = 20;
    ep.action1 = UP;
    ep.action2 = DOWN;

    memory->append(ep);  // Append a new episode
    EXPECT_EQ(memory->getCurrent().reward1, 10);  // Verify it's at the current index
    EXPECT_EQ(memory->getCurrent().reward2, 20);
    EXPECT_EQ(memory->getCurrent().action1, UP);
    EXPECT_EQ(memory->getCurrent().action2, DOWN);
}

// Test for wraparound behavior when appending data
TEST_F(MemoryTest, AppendDataWithWraparound) {
    // Fill memory to capacity
    fillAllMemory();

    // Append new data, which should replace the oldest entry (index 0)
    EpisodeParameter ep;
    ep.pongVariables.setZero();
    ep.reward1 = 99;
    ep.reward2 = 100;
    ep.action1 = UP;
    ep.action2 = DOWN;
    memory->append(ep);

    // Check the current data (index should be 1 after append)
    EXPECT_EQ(memory->getCurrent().reward1, 99);  // The next element should be 1
    
    // Sample the first element that was overwritten (index 0)
    EXPECT_EQ(memory->highest_index, 0);  // Expect the first element after wraparound
}

// Test for correct current index behavior
TEST_F(MemoryTest, getNextMemoryEntry) {
    fillMemory();
    // Verify that the current index points to the last added episode
    EpisodeParameter& current = memory->getNext();
    // Fill current slot
    Eigen::Matrix<float, 6, 1> pongVariables;
    pongVariables << 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f;
    current.action1 = WAIT;
    current.action2 = UP;
    current.reward1 = 10;
    current.reward2 = 20;
    current.gameEnd = true;
    // Verify current slot
    EXPECT_EQ(memory->highest_index, 5);

    EXPECT_EQ(memory->getCurrent().reward1, 10);
    EXPECT_EQ(memory->getCurrent().gameEnd, true);
}
