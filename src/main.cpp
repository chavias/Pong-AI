#include "Game.hpp"
#include "Training.hpp"

void setLearningParameter(LearningParams& learning)
{
    learning.learningRate = 2e-1;
    learning.updateTarget = 1e5; //2e5;//1e5;
    learning.startLearning = 1e5;
    learning.numEpisodes = 10e6;
    learning.discount = 0.99; // 95
    learning.regularization = 0;//0e-5;
    learning.maxRunningTime = 5e3;
    learning.miniBatchSize = 64;
    learning.tau = 0.01f; // soft update
}


void setScreeningParameter(LearningParams& learning)
{
    learning.learningRate = 0.001;
    learning.updateTarget = 5000; //2e5;//1e5;
    learning.startLearning = 1e4;
    learning.numEpisodes = 1e6;
    learning.discount = 0.9; // 95
    learning.regularization = 0;//0e-5;
    learning.maxRunningTime = 5e3;
    learning.miniBatchSize = 64;
    learning.tau = 0.01f; // soft update
}



void setEpsilonParameter(EpsilonParams& epParameter)
{
    epParameter.decay_rate = 1-1e-6;
    epParameter.epsilon = 1;
    epParameter.epsilonDel = 1e-6;//1e-7; //1e-6
    epParameter.epsilonMin = 0.05;//0.05;
}


void Train()
{   
    LearningParams learning;
    setScreeningParameter(learning);
    
    EpsilonParams eps;
    setEpsilonParameter(eps);

    float deltaTime = 0.1f;

    int number_hidden = 21;

    // Load the agent later
    std::string filenameLeft = "/home/mach/Projects/Pong_AI/models/agent_left_reward.dat";
    std::string filenameRight = "/home/mach/Projects/Pong_AI/models/agent_right_wall_reward.dat";


    std::unique_ptr<Training> training = std::make_unique<Training>(learning, eps, deltaTime, number_hidden);

    // Load the agents before the training
    // training->loadAgents(filenameLeft, filenameRight);

    // training->game->set_right_paddle(std::make_unique<WallPaddle>(PADDLE2_X, 0));

    training->train();
    
    // // Save the agent after training
    // training->saveAgents(filenameLeft, filenameRight);

    training->playGame();

    // Human player on the left
    training->set_player(0);

    training->playGame();
}

void Play()
{
    Game* game = new Game;

    // game->set_right_paddle(std::make_unique<WallPaddle>(PADDLE1_X, PADDLE_Y));

    game->Run();
}


int main()
{
    Train();

    // Play();

    return 0;
}