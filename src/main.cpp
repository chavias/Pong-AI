#include "Game.hpp"
#include "Training.hpp"

    // Learning parameters
    struct LearningParams {
        double learningRate;
        int updateTarget;
        int startLearning;
        int numEpisodes;
        double discount;
        double regularization;
        int maxRunningTime;
        int miniBatchSize;
    };

    // Epsilon-greedy parameters
    struct EpsilonParams {
        double epsilon;
        double epsilonDel;
        double epsilonMin;
    };

    // Reward tracking
    struct RewardParams {
        int maxReward1;
        int maxReward2;
    };



int main() {

    // std::unique_ptr<Game> game;

    // Game* game = new Game;

    // // game.set_paddles(std::make_unique<PlayerPaddle>(PADDLE1_X, PADDLE_Y),
    // //                  std::make_unique<PlayerPaddle>(PADDLE2_X, PADDLE_Y));

    // game->Run();

    // LearningParams learning;
    // learning.learningRate = 6e-1
    // learning.updateTarget = 10001
    // learning.startLearning = 5000
    // learning.numEpisodes = 64000
    // learning.discount = 0.95
    // learning.regularization = 0e-5
    // learning.maxRuningTime = 1000000
    // learning.miniBatchSize =64};

    std::unique_ptr<Training> training = std::make_unique<Training>();

    std::cout << "delta time = " << training->deltaTime << std::endl;
    
    training->train();

    // std::string path;
    // train->save(path);

    // train->load(path);

    training->set_player(0);

    training->playGame();




    return 0;
}