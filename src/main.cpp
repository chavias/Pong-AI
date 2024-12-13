#include "Game.hpp"
#include "Training.hpp"

void setLearningParameter(LearningParams& learning)
{
    learning.learningRate = 2e-1;
    learning.updateTarget = 1e4;//1e5;
    learning.startLearning = 1e5;
    learning.numEpisodes = 1e5;// 5e6;
    learning.discount = 0.95;
    learning.regularization = 0e-5;
    learning.maxRunningTime = 5e5;
    learning.miniBatchSize = 64;
}

void setRewardParams(RewardParams& reward)
{
    reward.maxReward1 = 0;
    reward.maxReward2 = 0;
}

void setEpsilonParameter(EpsilonParams& epParameter)
{
    epParameter.epsilon = 1;
    epParameter.epsilonDel = 1e-6;
    epParameter.epsilonMin = 0.05;
}

// void StartGame()
// {
//     std::unique_ptr<Game> game;

//     Game* game = new Game;

//     // game.set_paddles(std::make_unique<PlayerPaddle>(PADDLE1_X, PADDLE_Y),
//     //                  std::make_unique<PlayerPaddle>(PADDLE2_X, PADDLE_Y));

//     game->Run();
// }


int main()
{
    /**
     *  Choose parameter
     */
    
    LearningParams learning;
    setLearningParameter(learning);
    
    EpsilonParams eps;
    setEpsilonParameter(eps);

    float deltaTime = 5.0f;

    int number_hidden = 21;
    /**
     * Construct the game
     */

    std::unique_ptr<Training> training = std::make_unique<Training>(learning, eps, deltaTime, number_hidden);

    /**
     * Start training
     */

    training->train();

    // std::string path;
    // train->save(path);

    // train->load(path);
    
    /**
     * Play the game
     */
    training->playGame();

    std::cout << "Max reward of the left agent " << training->rewardParams.maxReward1 << "\n";
    std::cout << "Max reward of the right agent " << training->rewardParams.maxReward2 << "\n";

    training->set_player(0);

    training->playGame();

    return 0;
}