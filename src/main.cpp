#include "Game.hpp"
#include "Training.hpp"

void setLearningParameter(LearningParams& learning)
{
    learning.learningRate = 1e-1;
    learning.updateTarget = 1e5;//1e5;
    learning.startLearning = 1e5;
    learning.numEpisodes = 1e6;
    learning.discount = 0.95; // 95
    learning.regularization = 0e-5;
    learning.maxRunningTime = 5e3;
    learning.miniBatchSize = 64;
}

void setEpsilonParameter(EpsilonParams& epParameter)
{
    epParameter.epsilon = 1;
    epParameter.epsilonDel = 1e-6; //1e-6
    epParameter.epsilonMin = 0.05;
}



int main()
{
    /**
     *  Choose parameter
     */
    LearningParams learning;
    setLearningParameter(learning);
    
    EpsilonParams eps;
    setEpsilonParameter(eps);

    float deltaTime = 0.1f;

    int number_hidden = 21;

    // Load the agent later
    std::string filenameLeft = "/home/mach/Projects/Pong_AI/models/agent_left_large.dat";
    std::string filenameRight = "/home/mach/Projects/Pong_AI/models/agent_right_large.dat";


    std::unique_ptr<Training> training = std::make_unique<Training>(learning, eps, deltaTime, number_hidden);

    // Load the agents before the training
    // training->loadAgents(filenameLeft, filenameRight);


    training->train();
    
    // // Save the agent after training
    training->saveAgents(filenameLeft, filenameRight);

    training->playGame();

    // Human player on the left
    training->set_player(0);

    training->playGame();

    return 0;
}