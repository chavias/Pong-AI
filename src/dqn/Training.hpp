#pragma once
#include "Memory.hpp"
#include "Agent.hpp"
#include "../pong/Game.hpp"
#include "DQN.hpp"
#include "Random.hpp"
#include "../pong/constants.hpp"
#include <omp.h>
#include <random>
#include <algorithm>
#include <memory>
#include <iomanip>


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


class Training
{
public:

    LearningParams learningParams;
    EpsilonParams epsilonParams;
    RewardParams rewardParams;
    float deltaTime;
    
    Training();

    Training(size_t hidden);

    Training(const LearningParams &learningParams, const EpsilonParams &epsilonParams, float deltaTime);

    Training(const LearningParams &learningParams, const EpsilonParams &epsilonParams, float deltaTime, size_t hidden);


    void setAgent1(const std::string& filename);

    void setAgent2(const std::string& filename);

    void loadAgents(const std::string& filename1, const std::string& filename2);

    void saveAgent1(const std::string& filename);

    void saveAgent2(const std::string& filename);

    void saveAgents(const std::string& agent1Filename, const std::string& agent2Filename);

    // setup Memory
    std::unique_ptr<Memory> mem;

    // initialize Game
    std::unique_ptr<Game> game;

    // Initialize Agents and Target
    std::unique_ptr<Agent> agent1;
    std::unique_ptr<Agent> agent2;

    std::unique_ptr<Agent> nextTarget1;
    std::unique_ptr<Agent> nextTarget2;

    std::unique_ptr<Agent> target1;
    std::unique_ptr<Agent> target2;

    std::unique_ptr<Rand> random;

    // Populate the Memory randomly
    void populateMemoryRandom();

    // Main training loop
    void train();

    // performs gradient descent on specified agent
    void minibatchSGD(bool isAgent);

    // calculates gradient
    std::pair<Eigen::MatrixXf, Eigen::MatrixXf>
    gradient(const EpisodeParameter &ep,
                       const Eigen::Matrix<float, 6, 1>& nextState,
                       bool ended,
                       bool isAgent);


    // set a human player
    void set_player(bool side);

    // play a game with the current agents
    void playGame();
};