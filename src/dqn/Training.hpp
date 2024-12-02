#pragma once
#include "Memory.hpp"
#include "Agent.hpp"
#include "Game.hpp"
#include "DQN.hpp"
#include "MiniBatchSGD.hpp"
#include <algorithm>
#include <memory>
#include "EpisodeParameter.hpp"

class Training
{
public:
    // Learning parameter
    double learningRate = 6e-4;
    int updateTarget = 10001;
    int startLearning = 5000;
    int numEpisodes = 46000;
    double discount = 0.95; // Discount in Bellman Equation
    double regularization = 0e-5;
    int maxRunningTime = 500;
    int miniBatchSize = 64;

    // Game parameter 
    float deltaTime;

    // epsilon greedy parameter
    double epsilon = 1;
    double epsilonDel = 1e-4;
    double epsilonMin = 0.05;

    // Record rewards
    int maxReward1 = 0;
    int maxReward2 = 0;

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

    // Populate the Memory randomly
    void populateMemoryRandom();

    // Main training loop
    void train();

    // performs gradient descent on specified agent
    void minibatchGradientSGD();

    // 
    void test();

};
