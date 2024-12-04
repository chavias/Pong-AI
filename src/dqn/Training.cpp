#include "Training.hpp"
#include "constants.hpp"
#include "Random.hpp"
#include <omp.h>


/// @brief Populates memory with one game of pong
void Training::populateMemoryRandom()
{
    // #pragma omp parallel for
    for (int i = 0; i < startLearning; i++)
    {
        int t = 1;
        EpisodeParameter ep;
        ep.gameEnd = false;

        
        while (!ep.gameEnd && t < maxRunningTime)
        {
            // EpisodeParameter ep = game->Step(deltaTime,random->randomAction(),
            //                                            random->randomAction());

            ep = game->Step(deltaTime, WAIT, WAIT);
            mem->append(ep);
            t++;
        }
    }
    // std::cout << "mem filled" << std::endl;
};

void Training::train()
{
    for (int episode = 0; episode < numEpisodes; episode++)
    {
        epsilon = std::max(epsilonMin, epsilon - epsilonDel);

        // get struct to populate
        EpisodeParameter ep = mem->getNext();
        ep.gameEnd = false;

        int totalReward1 = 0;
        int totalReward2 = 0;

        bool looser; // determines which agent to train
        int t = 1;
        while (!ep.gameEnd && t < maxRunningTime)
        {
            // Agent 1
            // with probability epsilon choose random action
            if (random->randomEpsilon() < epsilon)
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
            if (random->randomEpsilon() < epsilon)
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
            // Save new episode in parameters
            mem->append(nextEpisode);

            // Update total Reward
            totalReward1 += nextEpisode.reward1;
            totalReward2 += nextEpisode.reward2;

            // The looser of this round is trained next round
            if (nextEpisode.gameEnd)
            {
                looser = (nextEpisode.reward1 < nextEpisode.reward2) ? true : false;
            }

            // raise indices
            t++;
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
    // (derActivationF(hiddenLayerOutput) * W2.transpose()) * delta2

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