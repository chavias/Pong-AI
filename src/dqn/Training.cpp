#include "Training.hpp"

#define LOG(X) std::cout << X << std::endl

    // // Learning parameters
    // struct LearningParams {
    //     double learningRate;
    //     int updateTarget;
    //     int startLearning;
    //     int numEpisodes;
    //     double discount;
    //     double regularization;
    //     int maxRunningTime;
    //     int miniBatchSize;
    // };

    // // Epsilon-greedy parameters
    // struct EpsilonParams {
    //     double epsilon;
    //     double epsilonDel;
    //     double epsilonMin;
    // };

    // // Reward tracking
    // struct RewardParams {
    //     int maxReward1;
    //     int maxReward2;
    // };


Training::Training()
    : learningParams{6e-1, 10001, 5000, 64000, 0.95, 1e-5, 10000, 64},
      epsilonParams{1, 1e-4, 0.05},
      rewardParams{0, 0},
      deltaTime(0.01f),
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
    : learningParams{6e-4, 1001, 5000, 64000, 0.95, 1e-5, 500, 64},
      epsilonParams{1, 1e-4, 0.05},
      rewardParams{0, 0},
      deltaTime(1.0f),
      mem(std::make_unique<Memory>()),
      game(std::make_unique<Game>()),
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
      rewardParams{0, 0},
      deltaTime(deltaTime),
      mem(std::make_unique<Memory>()),
      game(std::make_unique<Game>()),
      agent1(std::make_unique<Agent>(21, 7, 3, 22)),
      agent2(std::make_unique<Agent>(21, 7, 3, 22)),
      nextTarget1(std::make_unique<Agent>(21, 7, 3, 22)),
      nextTarget2(std::make_unique<Agent>(21, 7, 3, 22)),
      target1(std::make_unique<Agent>(21, 7, 3, 22)),
      target2(std::make_unique<Agent>(21, 7, 3, 22)),
      random(std::make_unique<Rand>()) {}

/// @brief Populates memory with one game of pong
void Training::populateMemoryRandom()
{
#pragma omp parallel for
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

void Training::set_paddles(std::unique_ptr<Paddle>&& p1, std::unique_ptr<Paddle>&& p2)
{
    game->set_paddles(std::move(p1), std::move(p2));
};


void Training::set_player(bool side)
{
    game->set_player(side);
}


void Training::train()
{
    // populate Memory
    populateMemoryRandom();

    omp_set_num_threads(8);  // Set to 4 threads

    int totalReward1 = 0;
    int totalReward2 = 0;

#pragma omp parallel for 
    for (int episode = 0; episode < learningParams.numEpisodes; episode++)
    {
        epsilonParams.epsilon = std::max(epsilonParams.epsilonMin, epsilonParams.epsilon - epsilonParams.epsilonDel);

        // get struct to populate
        EpisodeParameter ep = mem->getNext();
        ep.gameEnd = false;

        int totalReward1 = 0;
        int totalReward2 = 0;

        bool looser; // determines which agent to train
        int t = 1;
        while (!ep.gameEnd && t < learningParams.maxRunningTime)
        {
            // Agent 1
            // with probability epsilon choose random action
            if (random->randomEpsilon() < epsilonParams.epsilon)
                ep.action1 = random->randomAction();
            else
            {
                Eigen::Matrix<float, 3, 1> out = DQN(agent1, ep.pongVariables);
                // Find the maximum value and its index
                Eigen::Index idx;
                float max_value = out.maxCoeff(&idx);
                ep.action1 = static_cast<Action>(idx);
            }
            // Agent 2
            // with probability epsilon choose random action
            if (random->randomEpsilon() < epsilonParams.epsilon)
                ep.action2 = random->randomAction();
            else
            {
                Eigen::Matrix<float, 3, 1> out = DQN(agent2, ep.pongVariables);
                // Find the maximum value and its index
                Eigen::Index idx;
                float max_value = out.maxCoeff(&idx);
                ep.action2 = static_cast<Action>(idx);
            }

            // Step game
            EpisodeParameter nextEpisode = game->Step(deltaTime, ep.action1, ep.action2);
            mem->append(nextEpisode);

            // Update total Reward
            totalReward1 += nextEpisode.reward1;
            totalReward2 += nextEpisode.reward2;

            // The looser of this round is trained next round
            if (nextEpisode.gameEnd)
            {
                looser = (nextEpisode.reward1 < nextEpisode.reward2) ? true : false;
            }

            t++;
        }
#pragma critical
{
        // Record max reward and save weights for targets update
        if (totalReward1 >= rewardParams.maxReward1)
        {
            // std::cout << totalReward1 << std::endl;
            rewardParams.maxReward1 = totalReward1;
            // nextTarget1 = std::make_unique<Agent>(*agent1);
            *nextTarget1 = *agent1;
        }

        if (totalReward2 >= rewardParams.maxReward2)
        {
            // std::cout << totalReward1 << std::endl;
            rewardParams.maxReward2 = totalReward2;
            // nextTarget2 = std::make_unique<Agent>(*agent2);
            *nextTarget2 = *agent2;
        }

        // Update Targets
        if (episode % learningParams.updateTarget == 0)
        {
            // target1 = std::make_unique<Agent>(*nextTarget1);
            // target2 = std::make_unique<Agent>(*nextTarget2);
            *target1 = *nextTarget1;
            *target2 = *nextTarget2;
        }
}
        // LOG("target1->W1.rows = " << target1->W1.rows());
        // LOG("target1->W1.cols = " << target1->W1.cols());
        // LOG("target1->W2.rows = " << target1->W2.rows());
        // LOG("target1->W2.cols = " << target1->W2.cols());
        // LOG("-----------------------------------------");
        // LOG("target2->W1.rows = " << target2->W1.rows());
        // LOG("target2->W1.cols = " << target2->W1.cols());
        // LOG("target2->W2.rows = " << target2->W2.rows());
        // LOG("target2->W2.cols = " << target2->W2.cols());
        // LOG("-----------------------------------------");
        // LOG("agent1->W1.rows = " << agent1->W1.rows());
        // LOG("agent1->W1.cols = " << agent1->W1.cols());
        // LOG("agent1->W2.rows = " << agent1->W2.rows());
        // LOG("agent1->W2.cols = " << agent1->W2.cols());
        // LOG("-----------------------------------------");
        // LOG("agent2->W1.rows = " << agent2->W1.rows());
        // LOG("agent2->W1.cols = " << agent2->W1.cols());
        // LOG("agent2->W2.rows = " << agent2->W2.rows());
        // LOG("agent2->W2.cols = " << agent2->W2.cols());

        // train the looser Agent only
        // LOG("looser = " << looser);
        minibatchSGD(looser);
        // LOG("got memory");

    }
}

void Training::minibatchSGD(bool isAgent)
{
    // References to the correct agent and target
    auto &agent = isAgent ? agent1 : agent2;
    auto &target = isAgent ? target1 : target2;

    // Initialize gradients
    Eigen::MatrixXf dW1 = Eigen::MatrixXf::Zero(agent->W1.rows(), agent->W1.cols());
    Eigen::MatrixXf dW2 = Eigen::MatrixXf::Zero(agent->W2.rows(), agent->W2.cols());

    Eigen::MatrixXf dW1temp = Eigen::MatrixXf::Zero(agent->W1.rows(), agent->W1.cols());
    Eigen::MatrixXf dW2temp = Eigen::MatrixXf::Zero(agent->W2.rows(), agent->W2.cols());

    EpisodeParameter randomEpisode;

    for (int i = 0; i < learningParams.miniBatchSize; ++i)
    {
        // Sample a random instance from memory
        randomEpisode = mem->sample();

        // Compute gradients for the sampled instance
        auto [dW1temp, dW2temp] = gradient(randomEpisode, isAgent);

        // Accumulate gradients
        dW1 += dW1temp;
        dW2 += dW2temp;
    }

    // Update the weights of the agent
    agent->W1 = (learningParams.learningRate / learningParams.miniBatchSize) * dW1 + (1.0f - learningParams.regularization) * agent->W1;

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

    float y = 0.0f;
    if (ep.gameEnd)
    {
        y = reward;
    }
    else
    {
        Eigen::Matrix<float, 3, 1> out = DQN(targetRef, ep.pongVariables);
        float qTarget = out.maxCoeff(); // Maximum Q-value from target agent
        y = reward + learningParams.discount * qTarget;
    }
    // LOG("--------------------------");
    // LOG("bellman equation y = " << y);
    // LOG("--------------------------");

    // ------------------- Agent's Estimate of Q ------------------------- //

    DQNReturn agentOut = DQN(agentRef, ep.pongVariables, true);
    // LOG("--------------------------");
    // LOG("Q agent = " << agentOut.v2);
    // LOG("--------------------------");
    // LOG("agentOut.v2.rows() = " << agentOut.v2.rows());
    // LOG("Action = " << action);
    // ------------------- Backpropagation ------------------------------- //

    // Output layer error and delta
    Eigen::Matrix<float, 3, 1> e2 = Eigen::Vector3f::Zero();
    e2(action) = y - agentOut.v2((int)action);
    Eigen::VectorXf delta2 = e2;
    // LOG("--------------------------");
    // LOG("delta2  = " << e2);
    // LOG("--------------------------");

    // Hidden layer error and delta
    Eigen::VectorXf e1 = agentRef->W2.leftCols(agentRef->W2.cols() - 1).transpose() * delta2;
    // LOG("e1.rows() = " << e1.rows());
    // LOG("e1.cols() = " << e1.cols());

    // Ensure the size of the activation function input matches the hidden layer size
    Eigen::VectorXf hiddenLayerOutput = agentOut.y1.transpose().head(agentRef->W2.cols() - 1);

    // LOG("agent y1.rows() = " << agentOut.y1.rows());
    // LOG("agent y1.cols() = " << agentOut.y1.cols());

    // LOG("hiddenLayerOutput.rows() = " << hiddenLayerOutput.rows());
    // LOG("hiddenLayerOutput.cols() = " << hiddenLayerOutput.cols());

    Eigen::VectorXf hiddenActivation = derActivationF(hiddenLayerOutput);
    // LOG("hiddenActivation.rows() = " << hiddenActivation.rows());
    // LOG("hiddenActivation.cols() = " << hiddenActivation.rows());

    Eigen::VectorXf delta1 = derActivationF(hiddenLayerOutput).cwiseProduct(e1);

    // ------------------- Input Normalization --------------------------- //

    Eigen::Matrix<float, 6, 1> normalization_factors;
    normalization_factors << SCREEN_WIDTH, SCREEN_HEIGHT, BALL_SPEED, BALL_SPEED, SCREEN_HEIGHT, SCREEN_HEIGHT;
    Eigen::Matrix<float, 6, 1> normalizedInput = ep.pongVariables.array() / normalization_factors.array();

    // // Add bias to input
    Eigen::VectorXf inputWithBias(normalizedInput.size() + 1);
    inputWithBias.head(normalizedInput.size()) = normalizedInput;
    inputWithBias(normalizedInput.size()) = 1.0f; // Add bias term

    // ------------------- Compute Gradients ----------------------------- //

    Eigen::MatrixXf dW1 = delta1 * inputWithBias.transpose();
    Eigen::MatrixXf dW2 = delta2 * agentOut.y1.transpose();

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
    

    float deltaTime = 0.01;
    while (!WindowShouldClose())
    {

        EpisodeParameter state = game->Step(deltaTime, action1, action2);
        // get action 1
        out = DQN(agent1, state.pongVariables);
        max_value1 = out.maxCoeff(&idx1);
        action1 = static_cast<Action>(idx1);

        // get action 2
        out = DQN(agent2, state.pongVariables);
        max_value2 = out.maxCoeff(&idx2);
        action2 = static_cast<Action>(idx2);
    


        // std::cout << state.gameEnd << '\n';
        game->Render();
    }

    CloseWindow();
}