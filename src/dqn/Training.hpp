#pragma once
#include "Memory.hpp"
#include "Agent.hpp"
#include "Game.hpp"
#include "DQN.hpp"
#include "Random.hpp"
// #include "EpisodeParameter.hpp"
#include <random>
#include <algorithm>
#include <memory>   

class Training
{
public:
    // Learning parameter
    double learningRate = 6e-4;
    int updateTarget = 10001;
    int startLearning = 500; // 5000
    int numEpisodes = 46000;
    double discount = 0.95; // Discount in Bellman Equation
    double regularization = 0e-5;
    int maxRunningTime = 50; // 500
    int miniBatchSize = 64;

    // Game parameter
    float deltaTime = 1;

    // epsilon greedy parameter
    double epsilon = 1;
    double epsilonDel = 1e-4;
    double epsilonMin = 0.05;

    // Record rewards
    int maxReward1 = 0;
    int maxReward2 = 0;

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
};