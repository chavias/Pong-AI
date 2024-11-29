#pragma once
#include "Memory.hpp"
#include "Agent.hpp"
#include <memory>

class Training
{
public:
    double learningRate = 6e-4;
    int updateTarget = 10001;
    int startLearning = 5000;
    int numEpisods = 46000;
    double discount = 0.95; // Discount in Bellman Equation
    double regularization = 0e-5;

    // epsilon greedy
    double epsilon = 1;
    double epsilonDel = 1e-4;
    double epsilonMin = 0.05;

    // setup Memory
    std::unique_ptr<Memory> mem;

    // initialize Game 
    std::unique_ptr<Game> game;

    // Initialize Agents and Target
    std::unique_ptr<Agent> agent1;
    std::unique_ptr<Agent> agent1;

    std::unique_ptr<Agent> Target;
    std::unique_ptr<Agent> Target;

    // Populate the Memory randomly
    void populateMemoryRandom();

    // Main training loop
    void train();

    // performs gradient descent on specified agent
    void minibatchGradientSGD();

    // 
    void test()

};
