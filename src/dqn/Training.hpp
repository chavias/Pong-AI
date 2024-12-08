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

class Training
{
public:
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

    LearningParams learningParams;
    EpsilonParams epsilonParams;
    RewardParams rewardParams;
    float deltaTime;
    
    Training();

    Training(size_t hidden);

    Training(const LearningParams &learningParams, const EpsilonParams &epsilonParams, float deltaTime);

    // setup Memory
    std::unique_ptr<Memory> mem = std::make_unique<Memory>();

    // initialize Game
    std::unique_ptr<Game> game = std::make_unique<Game>(); // initialize with ai paddles

    // Initialize Agents and Target
    std::unique_ptr<Agent> agent1 = std::make_unique<Agent>(21, 7, 3, 22);
    std::unique_ptr<Agent> agent2 = std::make_unique<Agent>(21, 7, 3, 22);

    std::unique_ptr<Agent> nextTarget1 = std::make_unique<Agent>(21, 7, 3, 22);
    std::unique_ptr<Agent> nextTarget2 = std::make_unique<Agent>(21, 7, 3, 22);

    std::unique_ptr<Agent> target1 = std::make_unique<Agent>(21, 7, 3, 22);
    std::unique_ptr<Agent> target2 = std::make_unique<Agent>(21, 7, 3, 22);

    std::unique_ptr<Rand> random = std::make_unique<Rand>();

    // Populate the Memory randomly
    void populateMemoryRandom();

    // Main training loop
    void train();

    // performs gradient descent on specified agent
    void minibatchSGD(bool isAgent);

    // calculates gradient
    std::pair<Eigen::MatrixXf, Eigen::MatrixXf>
    gradient(const EpisodeParameter &ep, bool isAgent);

    //
    void test();

    void set_paddles(std::unique_ptr<Paddle>&& p1 = {}, std::unique_ptr<Paddle>&& p2 = {});

    void set_player(bool side);

    void playGame();
};