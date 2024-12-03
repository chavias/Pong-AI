#include <gtest/gtest.h>
#include "../../src/dqn/Training.hpp"
#include "../../src/pong/EpisodeParameter.hpp"  // Assuming this is the file where EpisodeParameter is defined
#include <iostream>
#include <memory>
#include <Eigen/Dense>


class TrainingTest : public ::testing::Test
{
protected:
    std::unique_ptr<Training> training;
    
    void SetUp() override
    {
        training = std::make_unique<Training>();
    }
};



// TEST_F(TrainingTest, PopulateMemoryRandom)
// {
//     training->populateMemoryRandom();
//     EpisodeParameter& ep = training->mem->getCurrent();
//     std::cout << ep.reward1 << ep.reward2 << ep.gameEnd << std::endl;
// }

