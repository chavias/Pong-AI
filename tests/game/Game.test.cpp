#include <gtest/gtest.h>
#include "Game.hpp"
#include <iostream>

#define LOG(X) std::cout << X << std::endl

// Test fixture for Game
class GameTest : public ::testing::Test {
protected:

    std::unique_ptr<Game> game;

    void SetUp() override {
        game = std::make_unique<Game>();
    }
};

// Test for Step function with a basic scenario
TEST_F(GameTest, StepUpdatesCorrectly) {
    // Call Step and validate its return value
    float deltaTime = 0.016f;  // Assuming a frame rate of 60 FPS
    Action action1 = WAIT;
    Action action2 = WAIT;

    // Simulate Step function call

    // for (size_t i = 0; i < 100; i++)
    // {
        EpisodeParameter result = game->Step(deltaTime, action1, action2);
        // LOG(result.pongVariables);
        // LOG("-------------------------");
    // }
    // Validate the returned values
    ASSERT_EQ(result.reward1, 0);
    ASSERT_EQ(result.reward2, 0);
    ASSERT_FALSE(result.gameEnd);



}



// // Test for collision and rewards when ball hits the paddle
// TEST_F(GameTest, StepCollisionAndReward) {
//     // Simulate the ball hitting paddle1
//     EXPECT_CALL(*paddle1, Update(testing::_, testing::_, testing::_)).Times(1);
//     EXPECT_CALL(*paddle2, Update(testing::_, testing::_, testing::_)).Times(1);
//     EXPECT_CALL(*ball, Update(testing::_)).Times(1);

//     Action action1 = WAIT;
//     Action action2 = WAIT;
//     ball->x = paddle1->x + paddle1->width;  // Ball is at paddle1's right edge
//     ball->y = paddle1->y + paddle1->height / 2;

//     // Call Step function and check the rewards
//     EpisodeParameter result = game->Step(0.016f, action1, action2);

//     // The ball should have collided with paddle1, and reward1 should be updated
//     ASSERT_EQ(result.reward1, 1);  // reward1 should increase
//     ASSERT_EQ(result.reward2, 0);  // reward2 should remain unchanged
// }

// // Test for game reset and game end
// TEST_F(GameTest, StepGameEndAndReset) {
//     // Simulate the ball going out of bounds on the left side
//     EXPECT_CALL(*scoreManager, LeftScored()).Times(1);
//     EXPECT_CALL(*scoreManager, RightScored()).Times(0);
//     EXPECT_CALL(*paddle1, Reset()).Times(1);
//     EXPECT_CALL(*paddle2, Reset()).Times(1);
//     EXPECT_CALL(*ball, Reset()).Times(1);

//     Action action1 = WAIT;
//     Action action2 = WAIT;
//     ball->x = -ball->radius;  // Ball is off-screen on the left side

//     // Call Step function and check the game end condition
//     EpisodeParameter result = game->Step(0.016f, action1, action2);

//     // The game should end with a reset
//     ASSERT_TRUE(result.gameEnd);
//     ASSERT_EQ(result.reward1, 1);  // Reward for losing the point
//     ASSERT_EQ(result.reward2, 0);  // Reward for winning the point
// }

