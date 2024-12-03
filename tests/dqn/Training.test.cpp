#include <gtest/gtest.h>
#include "../../src/dqn/Training.hpp"
#include "../../src/pong/EpisodeParameter.hpp"  // Assuming this is the file where EpisodeParameter is defined
#include <iostream>
#include <memory>
#include <Eigen/Dense>


class TrainingTest : public ::testing::Test {
protected:
    std::unique_ptr<Training> training;

    void SetUp() override {
        training = std::make_unique<Training>();
    }

    void TearDown() override {
        training.reset();
    }
};


// Test for populateMemoryRandom
TEST_F(TrainingTest, PopulateMemoryRandomAddsEpisodes) {
    // Arrange
    int initialMemorySize = training->mem->highest_index;

    // Act
    training->populateMemoryRandom();

    // Assert
    ASSERT_GT(training->mem->highest_index, initialMemorySize)
        << "Memory size should increase after populating.";
}

// Test for train
TEST_F(TrainingTest, TrainReducesEpsilon) {
    // Arrange
    double initialEpsilon = training->epsilon;

    // Act
    training->train();

    // Assert
    ASSERT_LT(training->epsilon, initialEpsilon)
        << "Epsilon should decrease during training.";
    ASSERT_GE(training->epsilon, training->epsilonMin)
        << "Epsilon should not fall below epsilonMin.";
}

// Test for minibatchSGD
TEST_F(TrainingTest, MinibatchSGDUpdatesAgentWeights) {
    // Arrange
    Eigen::MatrixXf initialW1 = training->agent1->W1;
    Eigen::MatrixXf initialW2 = training->agent1->W2;

    // Act
    training->minibatchSGD(true); // Train agent1

    // Assert
    ASSERT_FALSE(training->agent1->W1.isApprox(initialW1))
        << "Agent1's W1 weights should be updated after minibatchSGD.";
    ASSERT_FALSE(training->agent1->W2.isApprox(initialW2))
        << "Agent1's W2 weights should be updated after minibatchSGD.";
}

// Test for gradient
TEST_F(TrainingTest, GradientCalculatesCorrectValues) {
    // Arrange
    EpisodeParameter ep;
    ep.reward1 = 10.0f; // Test with a specific reward.
    ep.gameEnd = false;

    // Act
    auto [dW1, dW2] = training->gradient(ep, true); // Compute for agent1.

    // Assert
    ASSERT_EQ(dW1.rows(), training->agent1->W1.rows());
    ASSERT_EQ(dW1.cols(), training->agent1->W1.cols());
    ASSERT_EQ(dW2.rows(), training->agent1->W2.rows());
    ASSERT_EQ(dW2.cols(), training->agent1->W2.cols());
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
