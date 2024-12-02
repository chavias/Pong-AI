#include "Training.hpp"

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
                bool looser = (nextEpisode.reward1 < nextEpisode.reward2) ? 0 : 1;
        }

        // Record max reward and save weights for targets update 
        if (totalReward1 >= maxReward1)
        {
            maxReward1 = totalReward1;
            nextTarget1 = std::make_unique<Agent>(*agent1);
        }

        if (totalReward2 >= maxReward2)
        {
            maxReward2 = totalReward2;
            nextTarget2 = std::make_unique<Agent>(*agent2);
        }

        // Update Targets
        if (episode % updateTarget == 0)
        {
            target1 = std::make_unique<Agent>(*nextTarget1);
            target2 = std::make_unique<Agent>(*nextTarget2);
        }

        if(looser)
        {
            agent1 = MinibatchSGD(agent1, target1, learningRate, discount, miniBatchSize, regularization)
        }
        else
        {
            agent2 = MinibatchSGD(agent1, target1, learningRate, discount, miniBatchSize, regularization)
        }


    }
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