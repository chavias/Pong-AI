#include "Training.hpp"

#define DEBUG

#ifdef DEBUG
    #define DEBUG(X) X
    #define LOG(X) std::cout << X << std::endl
#endif

#ifndef DEBUG
    #define DEBUG(X)
    #define LOG(X)
#endif

Training::Training()
    : learningParams{0.4e-1, 10001, 5000, 600000, 0.95, 0.1e-5, 50000, 128},
      epsilonParams{1, 1e-5, 0.03},
      rewardParams{0, 0},
      deltaTime(5.0f),
      mem(std::make_unique<Memory>()),
      game(std::make_unique<Game>(std::make_unique<AIPaddle>(PADDLE1_X, PADDLE_Y),
                                  std::make_unique<AIPaddle>(PADDLE2_X, PADDLE_Y))),
      agent1(std::make_unique<Agent>(21, 7, 3, 22)),
      agent2(std::make_unique<Agent>(21, 7, 3, 22)),
      nextTarget1(std::make_unique<Agent>(21, 7, 3, 22)),
      nextTarget2(std::make_unique<Agent>(21, 7, 3, 22)),
      target1(std::make_unique<Agent>(21, 7, 3, 22)),
      target2(std::make_unique<Agent>(21, 7, 3, 22)),
      random(std::make_unique<Rand>()) {}

Training::Training(size_t hidden)
    : learningParams{6e-1, 1001, 5000, 64000, 0.95, 1e-5, 500, 64},
      epsilonParams{1, 1e-4, 0.05},
      rewardParams{0, 0},
      deltaTime(1.0f),
      mem(std::make_unique<Memory>()),
      game(std::make_unique<Game>(std::make_unique<AIPaddle>(PADDLE1_X, PADDLE_Y),
                                  std::make_unique<AIPaddle>(PADDLE2_X, PADDLE_Y))),
      agent1(std::make_unique<Agent>(hidden, 7, 3, hidden + 1)),
      agent2(std::make_unique<Agent>(hidden, 7, 3, hidden + 1)),
      nextTarget1(std::make_unique<Agent>(hidden, 7, 3, hidden + 1)),
      nextTarget2(std::make_unique<Agent>(hidden, 7, 3, hidden + 1)),
      target1(std::make_unique<Agent>(hidden, 7, 3, hidden + 1)),
      target2(std::make_unique<Agent>(hidden, 7, 3, hidden + 1)),
      random(std::make_unique<Rand>()) {}

Training::Training(const LearningParams &learningParams, const EpsilonParams &epsilonParams, float deltaTime)
    : learningParams(learningParams),
      epsilonParams(epsilonParams),
      rewardParams{-200, -200},
      deltaTime(deltaTime),
      mem(std::make_unique<Memory>()),
      game(std::make_unique<Game>(std::make_unique<AIPaddle>(PADDLE1_X, PADDLE_Y),
                                  std::make_unique<AIPaddle>(PADDLE2_X, PADDLE_Y))),
      agent1(std::make_unique<Agent>(21, 7, 3, 22)),
      agent2(std::make_unique<Agent>(21, 7, 3, 22)),
      nextTarget1(std::make_unique<Agent>(21, 7, 3, 22)),
      nextTarget2(std::make_unique<Agent>(21, 7, 3, 22)),
      target1(std::make_unique<Agent>(21, 7, 3, 22)),
      target2(std::make_unique<Agent>(21, 7, 3, 22)),
      random(std::make_unique<Rand>()) {}

Training::Training(const LearningParams &learningParams, const EpsilonParams &epsilonParams, float deltaTime, size_t hidden)
    : learningParams(learningParams),
      epsilonParams(epsilonParams),
      rewardParams{-200, -200},
      deltaTime(deltaTime),
      mem(std::make_unique<Memory>()),
      game(std::make_unique<Game>(std::make_unique<AIPaddle>(PADDLE1_X, PADDLE_Y),
                                  std::make_unique<AIPaddle>(PADDLE2_X, PADDLE_Y))),
      agent1(std::make_unique<Agent>(hidden, 7, 3, hidden + 1)),
      agent2(std::make_unique<Agent>(hidden, 7, 3, hidden + 1)),
      nextTarget1(std::make_unique<Agent>(hidden, 7, 3, hidden + 1)),
      nextTarget2(std::make_unique<Agent>(hidden, 7, 3, hidden + 1)),
      target1(std::make_unique<Agent>(hidden, 7, 3, hidden + 1)),
      target2(std::make_unique<Agent>(hidden, 7, 3, hidden + 1)),
      random(std::make_unique<Rand>()) {}


void Training::setAgent1(const std::string& filename) {
    // Load agent from file
    try {
        // Create a new Agent and load its state from the file
        agent1 = std::make_unique<Agent>(0, 0, 0, 0); // Initialize with placeholder dimensions
        agent1->loadFromFile(filename);

        // Create target agents for agent1
        nextTarget1 = std::make_unique<Agent>(*agent1); // Copy weights from loaded agent
        target1 = std::make_unique<Agent>(*agent1);     // Copy weights from loaded agent

        std::cout << "[+] Agent1 and its targets successfully loaded and set up from: " << filename << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[-] Error loading agent1 from file: " << e.what() << std::endl;
    }
}


void Training::setAgent2(const std::string& filename) {
    // Load agent from file
    try {
        // Create a new Agent and load its state from the file
        agent2 = std::make_unique<Agent>(0, 0, 0, 0); // Initialize with placeholder dimensions
        agent2->loadFromFile(filename);

        // Create target agents for agent1
        nextTarget2 = std::make_unique<Agent>(*agent2); // Copy weights from loaded agent
        target2 = std::make_unique<Agent>(*agent2);     // Copy weights from loaded agent

        std::cout << "[+] Agent2 and its targets successfully loaded and set up from: " << filename << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[-] Error loading agent2 from file: " << e.what() << std::endl;
    }
}    

void Training::saveAgents(const std::string& agent1Filename, const std::string& agent2Filename) {
    try {
        if (agent1) {
            agent1->saveToFile(agent1Filename);
            std::cout << "[+] Agent1 successfully saved to: " << agent1Filename << std::endl;
        } else {
            std::cerr << "[=] Agent1 is not initialized, cannot save!" << std::endl;
        }

        if (agent2) {
            agent2->saveToFile(agent2Filename);
            std::cout << "[+] Agent2 successfully saved to: " << agent2Filename << std::endl;
        } else {
            std::cerr << "[=] Agent2 is not initialized, cannot save!" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[-] Error saving agents: " << e.what() << std::endl;
    }
}

void Training::saveAgent1(const std::string& filename) {
    // Save agent1 to a file
    try {
        if (agent1) {
            agent1->saveToFile(filename);
            std::cout << "[+] Agent1 successfully saved to: " << filename << std::endl;
        } else {
            std::cerr << "[=] Agent1 is not initialized, cannot save!" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[-] Error saving agent1 to file: " << e.what() << std::endl;
    }
}

void Training::saveAgent2(const std::string& filename) {
    // Save agent2 to a file
    try {
        if (agent2) {
            agent2->saveToFile(filename);
            std::cout << "[+] Agent2 successfully saved to: " << filename << std::endl;
        } else {
            std::cerr << "[=] Agent2 is not initialized, cannot save!" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[-] Error saving agent2 to file: " << e.what() << std::endl;
    }
}

void Training::loadAgents(const std::string& filename1, const std::string& filename2) {
    // Load agent1
    try {
        agent1 = std::make_unique<Agent>(0, 0, 0, 0); // Initialize with placeholder dimensions
        agent1->loadFromFile(filename1);

        // Create target agents for agent1
        nextTarget1 = std::make_unique<Agent>(*agent1); // Copy weights from loaded agent
        target1 = std::make_unique<Agent>(*agent1);     // Copy weights from loaded agent

        std::cout << "[+] Agent1 and its targets successfully loaded from: " << filename1 << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[-] Error loading agent1 from file: " << e.what() << std::endl;
    }

    // Load agent2
    try {
        agent2 = std::make_unique<Agent>(0, 0, 0, 0); // Initialize with placeholder dimensions
        agent2->loadFromFile(filename2);

        // Create target agents for agent2
        nextTarget2 = std::make_unique<Agent>(*agent2); // Copy weights from loaded agent
        target2 = std::make_unique<Agent>(*agent2);     // Copy weights from loaded agent

        std::cout << "[+] Agent2 and its targets successfully loaded from: " << filename2 << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[-] Error loading agent2 from file: " << e.what() << std::endl;
    }
}




/// @brief Populates memory with one game of pong
void Training::populateMemoryRandom()
{
// #pragma omp parallel for
    for (int i = 0; i < learningParams.startLearning; i++)
    {
        int t = 1;
        EpisodeParameter ep;
        ep.gameEnd = false;

        while (!ep.gameEnd && t < learningParams.maxRunningTime)
        {
            ep = game->Step(deltaTime,
                            random->randomAction(),
                            random->randomAction());
            mem->append(ep);
            t++;
        }
    }
};


void Training::set_player(bool side)
{
    game->set_player(side);
}


void Training::train()
{
    std::cout << "===============================================================================================\n";
    std::cout << "                                   Starting the Training                                       \n";
    std::cout << "===============================================================================================\n";
    // Populate Memory
    // omp_set_num_threads(4);  // Set number of threads
    populateMemoryRandom();

    // // Should maybe be moved to the constructor
    DEBUG(InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong"));
    DEBUG(SetTargetFPS(50));

    // Declare totalReward1 and totalReward2 outside the loop
    int totalReward1 = 0;
    int totalReward2 = 0;

    EpisodeParameter ep = mem->getNext();
    ep.gameEnd = false;

    // Main training loop
    for (int episode = 0; episode < learningParams.numEpisodes; episode++)
    {
        epsilonParams.epsilon = std::max(epsilonParams.epsilonMin, epsilonParams.epsilon - epsilonParams.epsilonDel);

        bool looser; // Determines which agent to train
        int t = 1;

        // Reset rewards for each episode
        totalReward1 = 0;
        totalReward2 = 0;
        ep.gameEnd = false;

        while (!(ep.gameEnd) && t < learningParams.maxRunningTime)
        {
            // Agent 1
            if (random->randomEpsilon() < epsilonParams.epsilon)
                ep.action1 = random->randomAction();
            else
            {
                Eigen::Matrix<float, 3, 1> out = DQN(agent1, ep.pongVariables);
                Eigen::Index idx;
                float max_value = out.maxCoeff(&idx);
                ep.action1 = static_cast<Action>(idx);
            }

            // Agent 2
            if (random->randomEpsilon() < epsilonParams.epsilon)
                ep.action2 = random->randomAction();
            else
            {
                Eigen::Matrix<float, 3, 1> out = DQN(agent2, ep.pongVariables);
                Eigen::Index idx;
                float max_value = out.maxCoeff(&idx);
                ep.action2 = static_cast<Action>(idx);
            }
            
            // if (episode >= learningParams.numEpisodes - 100)
            // {
            DEBUG(game->scoreManager->ResetScore());
            DEBUG(game->Render());
            // }

            mem->append(ep);
            ep = game->Step(deltaTime, ep.action1, ep.action2);
            
            LOG(ep.pongVariables);
            // Update total rewards
            totalReward2 += ep.reward2;
            totalReward1 += ep.reward1;
            LOG("TotalReward 1 " << totalReward1);
            LOG("TotalReward 2 " << totalReward2);

            // Determine the looser of this round
            if (ep.gameEnd)
            {
                looser = (totalReward1 < totalReward2) ? true : false;
            }

            t++;
            if (t == learningParams.maxRunningTime)
                LOG("[-] Maximal runtime for game reached : " << t);
        }

        // Record max reward and save weights for target updates
        if (totalReward1 >= rewardParams.maxReward1)
        {
            rewardParams.maxReward1 = totalReward1;
            *nextTarget1 = *agent1;
        }

        if (totalReward2 >= rewardParams.maxReward2)
        {
            rewardParams.maxReward2 = totalReward2;
            *nextTarget2 = *agent2;
        }

        // Update targets periodically
        if (episode % learningParams.updateTarget == 0)
        {
            std::cout << "[+] " << std::setw(5) << episode / learningParams.updateTarget << " update of targets";
            std::cout << " in episode " << std::setw(7) << episode;
            std::cout << std::fixed << std::setprecision(2);
            std::cout << " with max rewards : " << std::setw(4) << rewardParams.maxReward1 << " | " << std::setw(4) << rewardParams.maxReward2;
            std::cout << " and epsilon " << epsilonParams.epsilon << std::endl;

            *target1 = *nextTarget1;
            *target2 = *nextTarget2;
        }

        minibatchSGD(looser);
    }

    std::cout << "===============================================================================================\n";
    std::cout << "[+] Training finished successfully" << "\n";
    std::cout << "    Max Reward 1 = " << rewardParams.maxReward1 << "\n";
    std::cout << "    Max Reward 2 = " << rewardParams.maxReward2 << "\n";
    std::cout << "================================================================================================\n" << std::endl;
}


void Training::minibatchSGD(bool isAgent)
{
    // References to the correct agent and target
    if (isAgent)
        LOG("Update left agent");
    else 
        LOG("Update right agent");

    auto &agent = isAgent ? agent1 : agent2;
    auto &target = isAgent ? target1 : target2;

    // Initialize gradients
    Eigen::MatrixXf dW1 = Eigen::MatrixXf::Zero(agent->W1.rows(), agent->W1.cols());
    Eigen::MatrixXf dW2 = Eigen::MatrixXf::Zero(agent->W2.rows(), agent->W2.cols());

    // Eigen::MatrixXf dW1temp = Eigen::MatrixXf::Zero(agent->W1.rows(), agent->W1.cols());
    // Eigen::MatrixXf dW2temp = Eigen::MatrixXf::Zero(agent->W2.rows(), agent->W2.cols());

    EpisodeParameter randomEpisode;

    for (int i = 0; i < learningParams.miniBatchSize; ++i)
    {
        // Sample a random instance from memory
        randomEpisode = mem->sample();

        // Compute gradients for the sampled instance
        auto [dW1temp, dW2temp] = gradient(randomEpisode, isAgent);

        // Accumulate gradients
        dW1 += dW1temp;
        LOG("dW1" << dW1);
        dW2 += dW2temp;
        LOG("dW2" << dW2);
    }

    // Update the weights of the agent
    agent->W1 = (learningParams.learningRate / learningParams.miniBatchSize) * dW1 + (1.0f - learningParams.regularization) * agent->W1;
    LOG("agent->W1" << agent->W1);
    agent->W2 = (learningParams.learningRate / learningParams.miniBatchSize) * dW2 + (1.0f - learningParams.regularization) * agent->W2;
}

std::pair<Eigen::MatrixXf, Eigen::MatrixXf>
Training::gradient(const EpisodeParameter &ep, bool isAgent)
{
    // References to the correct agent and target
    auto &agentRef = isAgent ? agent1 : agent2;
    auto &targetRef = isAgent ? target1 : target2;
    float reward = isAgent ? ep.reward1 : ep.reward2;
    Action action = isAgent ? ep.action1 : ep.action2;

    // ----------------- Calculate y from Bellman Equation ---------------- //

    float y = reward;
    if (!ep.gameEnd)
    {
        Eigen::Matrix<float, 3, 1> out = DQN(targetRef, ep.pongVariables);
        float qTarget = out.maxCoeff(); // Maximum Q-value from target agent
        y += learningParams.discount * qTarget;
    }

    // ------------------- Agent's Estimate of Q ------------------------- //

    DQNReturn agentOut = DQN(agentRef, ep.pongVariables, true);

    // ------------------- Backpropagation ------------------------------- //

    // Output layer error and delta
    Eigen::VectorXf e2 = Eigen::VectorXf::Zero(3);
    e2(action) = y - agentOut.v2(action); // Target - Predicted Q-value
    Eigen::VectorXf delta2 = e2;

    // Hidden layer error
    Eigen::VectorXf e1 = agentRef->W2.leftCols(agentRef->W2.cols() - 1).transpose() * delta2;

    // Hidden layer activation derivative
    Eigen::VectorXf hiddenActivation = derActivationF(agentOut.y1);
    Eigen::VectorXf delta1 = hiddenActivation.cwiseProduct(e1);

    // ------------------- Input Normalization --------------------------- //

    Eigen::Matrix<float, 6, 1> normalization_factors;
    normalization_factors << SCREEN_WIDTH, SCREEN_HEIGHT, BALL_SPEED, BALL_SPEED, SCREEN_HEIGHT, SCREEN_HEIGHT;
    Eigen::Matrix<float, 6, 1> normalizedInput = ep.pongVariables.array() / normalization_factors.array();

    // Add bias to input
    Eigen::VectorXf inputWithBias(normalizedInput.size() + 1);
    inputWithBias.head(normalizedInput.size()) = normalizedInput;
    inputWithBias(normalizedInput.size()) = 1.0f;

    // ------------------- Compute Gradients ----------------------------- //

    Eigen::MatrixXf dW1 = delta1 * inputWithBias.transpose(); // Gradients for W1
    Eigen::MatrixXf dW2 = delta2 * agentOut.y1.transpose();   // Gradients for W2

    return std::make_pair(dW1, dW2);
}

void Training::playGame()
{
    // Should maybe moved to the constructor
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(60);

    Action action1 = WAIT;
    Action action2 = WAIT;

    Eigen::Index idx1;
    Eigen::Index idx2;

    float max_value1;
    float max_value2;

    Eigen::Matrix<float, 3, 1> out;
    
    game->scoreManager->ResetScore();
    
    while (!WindowShouldClose())
    {

        EpisodeParameter state = game->Step(deltaTime, action1, action2);
        // get action 1
        out = DQN(nextTarget1, state.pongVariables);
        // std::cout << "Out1 = " << out << "\n";
        max_value1 = out.maxCoeff(&idx1);
        // std::cout << "idx1 = " << idx1 << "\n";
        action1 = static_cast<Action>(idx1);
        // std::cout << action1 << "\n";

        // get action 2
        out = DQN(nextTarget2, state.pongVariables);
        max_value2 = out.maxCoeff(&idx2);
        // std::cout << "idx2 = " << idx2 << "\n";
        action2 = static_cast<Action>(idx2);
        // std::cout << "Out2 = " << out << "\n";
        // std::cout << action2 << "\n";
        // if (state.gameEnd)
        // {
        //     LOG("===================================");
        //     LOG("Game ended");
        //     LOG("Reward 1 " << state.reward1);
        //     LOG("Reward 2 " << state.reward2);
        //     LOG("===================================");
        // }


        game->Render();
    }

    std::cout <<  "Game end"  << std::endl;
    CloseWindow();
}



/*
    Use atomics 

    #include <atomic>

// Assuming rewardParams.maxReward1 and maxReward2 are changed to std::atomic<int>
std::atomic<int> maxReward1 = 0;
std::atomic<int> maxReward2 = 0;

// Atomic max reward update logic
void updateMaxReward(std::atomic<int>& currentMax, int newReward) {
    int prevValue = currentMax.load();
    while (newReward > prevValue && !currentMax.compare_exchange_weak(prevValue, newReward)) {
        // Retry until successful
    }
}

// Inside your parallel loop
updateMaxReward(maxReward1, totalReward1);
updateMaxReward(maxReward2, totalReward2);

*/