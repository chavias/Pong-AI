#include "Training.hpp"
#include "constants.hpp"

/// @brief Plays pong with random actions for both agents and saves everything in memory
void Training::populateMemoryRandom()
{
    for (int i = 0; i < startLearning; i++)
    {
    }
};

void Training::train()
{
    std::random_device rd;                                       // Seed source
    std::mt19937 gen(rd());                                      // Mersenne Twister generator
    std::uniform_real_distribution<float> distEpsilon(0.0, 1.0); // for epsilon
    std::uniform_int_distribution<int> distAction(1, 3);

    int idx = 0;
    for (int episode = 0; episode < numEpisodes; episode++)
    {
        epsilon = std::max(epsilonMin, epsilon - epsilonDel);

        EpisodeParameter ep = mem->ReplayMemory[idx];
        ep.gameEnd = false;

        int totalReward1 = 0;
        int totalReward2 = 0;

        bool looser; // determines which agent to train
        int t = 1;
        while (!ep.gameEnd && t < maxRunningTime)
        {
            // Agent 1
            // with probability epsilon choose random action
            if (distEpsilon(gen) < epsilon)
                ep.action1 = (Action)distAction(gen);
            else
            {
                Eigen::Matrix<float, 3, 1> out = DQN(agent1, ep.pongVariables);
                // Find the maximum value and its index
                Action outAction;
                float max_value = out.maxCoeff(&outAction);
                ep.action1 = outAction;
            }
            // Agent 2
            // with probability epsilon choose random action
            if (distEpsilon(gen) < epsilon)
                ep.action2 = (Action)distAction(gen);
            else
            {
                Eigen::Matrix<float, 3, 1> out = DQN(agent1, ep.pongVariables);
                // Find the maximum value and its index
                Action outAction;
                float max_value = out.maxCoeff(&outAction);
                ep.action1 = outAction;
            }

            // Step game
            EpisodeParameter nextEpisode = game->Step(deltaTime, ep.action1, ep.action2);
            // Save new episode in parameters
            mem->append(nextEpisode);

            // Update total Reward
            totalReward1 += nextEpisode.reward1;
            totalReward2 += nextEpisode.reward2;

            // raise indices
            idx = idx % mem->size + 1;
            t++;

            // The looser of this round is trained next round
            if (nextEpisode.gameEnd)
            {
                looser = (nextEpisode.reward1 < nextEpisode.reward2) ? true : false;
            }
        }

        // Record max reward and save weights for targets update
        if (totalReward1 >= maxReward1)
        {
            maxReward1 = totalReward1;
            //nextTarget1 = std::make_unique<Agent>(*agent1);
            *nextTarget1 = *agent1;
        }

        if (totalReward2 >= maxReward2)
        {
            maxReward2 = totalReward2;
            // nextTarget2 = std::make_unique<Agent>(*agent2);
            *nextTarget2 = *agent2;
        }

        // Update Targets
        if (episode % updateTarget == 0)
        {
            // target1 = std::make_unique<Agent>(*nextTarget1);
            // target2 = std::make_unique<Agent>(*nextTarget2);
            *target1 = *nextTarget1;
            *target2 = *nextTarget2;
        }

        // train the looser Agent only
        minibatchSGD(looser);
    }
}


void Training::minibatchSGD(bool isAgent)
{
    // References to the correct agent and target
    auto& agent = isAgent ? agent1 : agent2;
    auto& target = isAgent ? target1 : target2;

    // Initialize gradients
    Eigen::MatrixXf dW1 = Eigen::MatrixXf::Zero(agent->W1.rows(), agent->W1.cols());
    Eigen::MatrixXf dW2 = Eigen::MatrixXf::Zero(agent->W2.rows(), agent->W2.cols());

    // Collect gradients over the minibatch
    for (int i = 0; i < miniBatchSize; ++i) {
        // Sample a random instance from memory
        EpisodeParameter randomEpisode = mem->sample();

        // Compute gradients for the sampled instance
        auto [dW1temp, dW2temp] = gradient(randomEpisode, isAgent);

        // Accumulate gradients
        dW1 += dW1temp;
        dW2 += dW2temp;
    }

    // Update the weights of the agent
    agent->W1 = (learningRate / miniBatchSize) * dW1 + (1.0f - regularization) * agent->W1;
    agent->W2 = (learningRate / miniBatchSize) * dW2 + (1.0f - regularization) * agent->W2;
}


std::pair<Eigen::MatrixXf, Eigen::MatrixXf>
Training::gradient(const EpisodeParameter& ep, bool isAgent)
{
    // References to the correct agent and target
    auto& agentRef = isAgent ? agent1 : agent2;
    auto& targetRef = isAgent ? target1 : target2;
    float reward = isAgent ? ep.reward1 : ep.reward2;

    // Initialize y based on the Bellman equation
    float y = 0.0f;
    if (ep.gameEnd) {
        y = reward;
    } else {
        // Compute target Q-value using the target agent
        Eigen::Matrix<float, 3, 1> out = DQN(targetRef, ep.pongVariables);
        float qTarget = out.maxCoeff(); // Maximum Q-value from target agent
        y = reward + discount * qTarget;
    }

    // Agent's estimate of Q (and intermediate steps)
    Eigen::Matrix<float, 3, 1> agentOut = DQN(agentRef, ep.pongVariables);

    // Backpropagation
    Eigen::Matrix<float, 3, 1> e2 = Eigen::Vector3f::Zero(); // Error in output layer
    int actionIndex = static_cast<int>(reward); // Assuming reward encodes action
    e2(actionIndex) = y - agentOut(actionIndex);

    Eigen::VectorXf delta2 = e2; // Output layer delta

    // Calculate delta for the hidden layer
    Eigen::VectorXf e1 = agentRef->W2.leftCols(agentRef->W2.cols() - 1).transpose() * delta2;
    Eigen::VectorXf delta1 = derActivationF(agentOut.head(agentOut.size() - 1)).cwiseProduct(e1);

    // Normalize input like in DQN
    Eigen::Matrix<float, 6, 1> normalizedInput = ep.pongVariables.cwiseQuotient(
        Eigen::Matrix<float, 6, 1>((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, (float)BALL_SPEED,
                                   (float)BALL_SPEED, (float)SCREEN_HEIGHT, (float)SCREEN_HEIGHT));

    // // Add bias to input
    // Eigen::VectorXf inputWithBias(normalizedInput.size() + 1);
    // inputWithBias.head(normalizedInput.size()) = normalizedInput;
    // inputWithBias(normalizedInput.size()) = 1.0f; // Add bias term

    // Compute gradients
    Eigen::MatrixXf dW1 = delta1 * normalizedInput.transpose();
    Eigen::MatrixXf dW2 = delta2 * agentOut.transpose();

    // Return gradients as a pair
    return std::make_pair(dW1, dW2);
}





std::tuple<PongVariables, bool, float, float> PongNextStep(
    const PongVariables &pongVars, int action1, int action2);

// RandomAgent function
void RandomAgent(int &idx, int MaxMemory, int MaxRuntime, int &t, float &R1, float &R2)
{
    // Ensure the Memory vector is large enough
    if (Memory.size() < MaxMemory)
    {
        Memory.resize(MaxMemory);
    }

    // Initialize Pong
    Memory[idx].pongVariables = InitializePong();
    Memory[idx].game_end = false;

    t = 1; // Reset runtime counter
    while (t < MaxRuntime && !Memory[idx].game_end)
    {
        // Choose random actions
        Memory[idx].action1 = rand() % 3 + 1; // Random number in [1, 3]
        Memory[idx].action2 = rand() % 3 + 1;

        // Execute next step
        auto [newPongVars, gameEnd, reward1, reward2] = PongNextStep(
            Memory[idx].pongVariables, Memory[idx].action1, Memory[idx].action2);

        // Save new state into the next memory slot
        int nextIdx = (idx + 1) % MaxMemory;
        Memory[nextIdx].pongVariables = newPongVars;
        Memory[nextIdx].game_end = gameEnd;
        Memory[idx].reward1 = reward1;
        Memory[idx].reward2 = reward2;

        // Update indices and runtime
        t++;
        idx = nextIdx;
    }

    // Set rewards for the last step
    R1 = Memory[(idx - 1 + MaxMemory) % MaxMemory].reward1;
    R2 = Memory[(idx - 1 + MaxMemory) % MaxMemory].reward2;

    // Update idx for the next call
    idx = (idx + 1) % MaxMemory;
}
// for (int i = 0; i < startLearning; i++)
//         {

//             bool gameEnd = false;

//             while (!gameEnd)
//             {
//                 auto Q1 = DQN(agent1, PongVariable);
//                 auto Q1 = DQN(agent2, PongVariable);

//                 EpisodeParameter episode = game->Step(deltaTime, action1, action2);

//                 TotalReward1 += reward1;
//                 TotalReward2 += reward2;
//             }
//         }