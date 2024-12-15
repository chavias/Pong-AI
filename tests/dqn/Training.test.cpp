#include <gtest/gtest.h>
#include "../../src/dqn/Training.hpp"
#include "../../src/pong/EpisodeParameter.hpp"  // Assuming this is the file where EpisodeParameter is defined
#include <iostream>
#include <memory>
#include <Eigen/Dense>

#define LOG(X) std::cout << X << std::endl

class TrainingTest : public ::testing::Test {
protected:
    std::unique_ptr<Training> training;

    void SetUp() override {
        training = std::make_unique<Training>(21);
    }

    // void TearDown() override {
    //     training.reset();
    // }
};


// Test for populateMemoryRandom
TEST_F(TrainingTest, PopulateMemoryRandomAddsEpisodes) {
    // Arrange
    int initialMemorySize = training->mem->highest_index;

    //     Act
    training->populateMemoryRandom();
    //     Assert
    auto [ep1, state1, ended1] = training->mem->sample();

    // LOG(ep1.pongVariables);

    auto [ep2, state2, ended2] = training->mem->sample();

    // LOG(ep2.pongVariables);

    auto [ep3, state3, ended3] = training->mem->sample();

    // LOG(ep3.pongVariables);
}

// Test for gradient
TEST_F(TrainingTest, GradientCalculatesCorrectValues) {
    // Arrange
    training->populateMemoryRandom();
    auto [ep, nextState, ended] = training->mem->sample();

    // Act
    auto [dW1, dW2] = training->gradient(ep,nextState, ended, true); // Compute for agent
    LOG("========================================");
    LOG(dW1);
    LOG("========================================");
    LOG(dW2);
    LOG("========================================");
    std::cout << "Gradient norms: dW1=" << dW1.norm() << ", dW2=" << dW2.norm() << std::endl;
    LOG("========================================");

    // Assert
    ASSERT_EQ(dW1.rows(), training->agent1->W1.rows());
    ASSERT_EQ(dW1.cols(), training->agent1->W1.cols());
    ASSERT_EQ(dW2.rows(), training->agent1->W2.rows());
    ASSERT_EQ(dW2.cols(), training->agent1->W2.cols());
}

// Test for minibatchSGD
TEST_F(TrainingTest, MinibatchSGDUpdatesAgentWeights) {

    // Arrange
    Eigen::MatrixXf initialW1 = training->agent1->W1;
    Eigen::MatrixXf initialW2 = training->agent1->W2;

    // Act
    training->populateMemoryRandom();
    training->minibatchSGD(true); // Train agent1

    // Assert
    ASSERT_NE(initialW1, training->agent1->W1);
    ASSERT_NE(initialW2, training->agent1->W2);
}

// Test for train
TEST_F(TrainingTest, TrainReducesEpsilon) {

    // Arrange
    double initialEpsilon = training->epsilonParams.epsilon;
    
    // omp_set_num_threads(4);  // Set to 4 threads

    // Act
    training->train();
    training->playGame();

    std::cout << training->rewardParams.maxReward1 << ' ' << training->rewardParams.maxReward2 << std::endl;



    // // Assert
    ASSERT_LT(training->epsilonParams.epsilon, initialEpsilon)
         << "Epsilon should decrease during training.";
    ASSERT_GE(training->epsilonParams.epsilon, training->epsilonParams.epsilonMin)
         << "Epsilon should not fall below epsilonMin.";
}

// Test for test
// TEST_F(TrainingTest, TestExecutesWithoutErrors) {
//     // Act & Assert
//     ASSERT_NO_THROW(training->test())
//         << "Test method should execute without throwing exceptions.";
// }

// TEST_F(TrainingTest, PopulateMemoryRandom)
// {
//     training->populateMemoryRandom();
//     EpisodeParameter& ep = training->mem->getCurrent();
//     std::cout << ep.reward1 << ep.reward2 << ep.gameEnd << std::endl;
// }

// TEST_F(TrainingTest, PopulateMemoryRandomAppendsCorrectly) {
//     // Arrange
//     EXPECT_CALL(mockRandom, randomAction())
//         .Times(testing::AtLeast(1))
//         .WillRepeatedly(testing::Return(Action::STAY)); // Assume `STAY` is an action.

//     EXPECT_CALL(mockGame, Step(testing::_, testing::_, testing::_))
//         .Times(testing::AtLeast(1))
//         .WillRepeatedly(testing::Return(EpisodeParameter{})); // Return default EpisodeParameter.

//     EXPECT_CALL(mockMemory, append(testing::_))
//         .Times(testing::AtLeast(1)); // Verify that memory append is called.

//     // Act
//     training->populateMemoryRandom();

//     // Assert
//     // The expectations set in EXPECT_CALL will automatically verify behavior.
// }
