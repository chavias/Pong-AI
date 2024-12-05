#include "Training.hpp"
#include "constants.hpp"
#include "Random.hpp"
// #include <omp.h>

#define LOG(X) std::cout << X << std::endl

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
            ep = game->Step(deltaTime,random->randomAction(),
                                                       random->randomAction());
            mem->append(ep);
            t++;
        }
    }
    // std::cout << "mem filled" << std::endl;
};

void Training::train()
{
    LOG("Entered train");
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

    EpisodeParameter randomEpisode;
    // Collect gradients over the minibatch
    for (int i = 0; i < miniBatchSize; ++i) {
        // Sample a random instance from memory
        randomEpisode = mem->sample();

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
    Action action = isAgent ? ep.action1 : ep.action2;


    // ----------------- Calculate y from Bellman Equation ---------------- //

    float y = 0.0f;
    if (ep.gameEnd) {
        y = reward;
    } else {
        Eigen::Matrix<float, 3, 1> out = DQN(targetRef, ep.pongVariables);
        float qTarget = out.maxCoeff(); // Maximum Q-value from target agent
        y = reward + discount * qTarget;
    }
    LOG("--------------------------");
    LOG("bellman equation y = " << y);
    LOG("--------------------------");

    // ------------------- Agent's Estimate of Q ------------------------- //

    Eigen::Matrix<float, 3, 1> agentOut = DQN(agentRef, ep.pongVariables);
    LOG("--------------------------");
    LOG("Q agent = " << agentOut);
    LOG("--------------------------");

    // ------------------- Backpropagation ------------------------------- //

    // Output layer error and delta
    Eigen::Matrix<float, 3, 1> e2 = Eigen::Vector3f::Zero();
    e2(action) = y - agentOut(action);
    Eigen::VectorXf delta2 = e2; // transpose ?? 
    LOG("--------------------------");
    LOG("delta2  = " << e2);
    LOG("--------------------------");


    //  Agent.W2(:,1:end-1)'
    Eigen::MatrixXf e11 = agentRef->W2.leftCols(agentRef->W2.cols()-1).transpose();
    LOG("e11.rows() = " << e11.rows());
    LOG("e11.cols() = " << e11.cols());

    Eigen::VectorXf e12 = e11 * delta2; //?

    LOG("e12.rows() = " << e12.rows());
    LOG("e12.cols() = " << e12.cols());

    // // Hidden layer error and delta
    // Eigen::VectorXf e1 = agentRef->W2.leftCols(agentRef->W2.cols() - 1).transpose() * delta2;
    // Eigen::VectorXf delta1 = derActivationF(agentOut.head(agentOut.size() - 1)).cwiseProduct(e1);
    // assert(agentRef->W2.cols() == delta2.size());
    // LOG(agentRef->W2.cols());
    // LOG(agentOut.size());
    // assert(agentRef->W2.cols() - 1 == agentOut.size() - 1);

    // Hidden layer error and delta
    // Eigen::VectorXf e1 = agentRef->W2.leftCols(agentRef->W2.cols() - 1).transpose() * delta2;
    Eigen::VectorXf e1 = agentRef->W2.leftCols(agentRef->W2.cols() - 1).transpose() * delta2;

    // Ensure the size of the activation function input matches the hidden layer size
    Eigen::VectorXf hiddenLayerOutput = agentOut.head(agentRef->W2.rows() - 1);
    LOG("hiddenLayerOutput.rows() = " << hiddenLayerOutput.rows());
    LOG("hiddenLayerOutput.rows() = " << hiddenLayerOutput.rows());

    Eigen::VectorXf delta1 = derActivationF(hiddenLayerOutput).cwiseProduct(e1);




    LOG("Calculated first hidden layer");
    // ------------------- Input Normalization --------------------------- //

    Eigen::Matrix<float, 6, 1> normalizedInput = ep.pongVariables.cwiseQuotient(
        Eigen::Matrix<float, 6, 1>((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, (float)BALL_SPEED,
                                   (float)BALL_SPEED, (float)SCREEN_HEIGHT, (float)SCREEN_HEIGHT));

    // Add bias to input
    Eigen::VectorXf inputWithBias(normalizedInput.size() + 1);
    inputWithBias.head(normalizedInput.size()) = normalizedInput;
    inputWithBias(normalizedInput.size()) = 1.0f; // Add bias term

    // ------------------- Compute Gradients ----------------------------- //

    // Ensure matrix dimensions are compatible
    // if (delta1.size() != agentRef->W1.rows() || inputWithBias.size() != agentRef->W1.cols()) {
    //     throw std::runtime_error("Dimension mismatch for dW1 calculation");
    // }

    // if (delta2.size() != agentRef->W2.rows() || agentOut.size() + 1 != agentRef->W2.cols()) {
    //     throw std::runtime_error("Dimension mismatch for dW2 calculation");
    // }

    Eigen::MatrixXf dW1 = delta1 * inputWithBias.transpose();
    Eigen::MatrixXf dW2 = delta2 * agentOut.transpose();

    LOG("Gradient computation complete");

    // Return gradients as a pair
    return std::make_pair(dW1.transpose(), dW2.transpose());
}


// std::pair<Eigen::MatrixXf, Eigen::MatrixXf>
// Training::gradient(const EpisodeParameter& ep, bool isAgent)
// {
//     LOG("Inside gradient");

//     // References to the correct agent and target
//     auto& agentRef = isAgent ? agent1 : agent2;
//     auto& targetRef = isAgent ? target1 : agent2;
//     float reward = isAgent ? ep.reward1 : ep.reward2;

//     // Initialize y based on the Bellman equation
//     float y = 0.0f;
//     if (ep.gameEnd) {
//         y = reward;
//     } else {
//         // Compute target Q-value using the target agent
//         Eigen::Matrix<float, 3, 1> out = DQN(targetRef, ep.pongVariables);
//         float qTarget = out.maxCoeff(); // Maximum Q-value from target agent
//         y = reward + discount * qTarget;
//     }

//     LOG("Called DQN for target agent");

//     // Agent's estimate of Q (and intermediate steps)
//     Eigen::Matrix<float, 3, 1> agentOut = DQN(agentRef, ep.pongVariables);

//     // Ensure actionIndex is valid
//     int actionIndex = ...; // Replace with correct logic to derive action index from ep or reward
//     if (actionIndex < 0 || actionIndex >= agentOut.size()) {
//         throw std::out_of_range("Invalid action index");
//     }

//     // Backpropagation
//     Eigen::Matrix<float, 3, 1> e2 = Eigen::Vector3f::Zero(); // Error in output layer
//     e2(actionIndex) = y - agentOut(actionIndex);
//     Eigen::VectorXf delta2 = e2; // Output layer delta

//     // Calculate delta for the hidden layer
//     Eigen::VectorXf e1 = agentRef->W2.leftCols(agentRef->W2.cols() - 1).transpose() * delta2;
//     Eigen::VectorXf delta1 = derActivationF(agentOut.head(agentOut.size() - 1)).cwiseProduct(e1);

//     // Normalize input like in DQN
//     Eigen::Matrix<float, 6, 1> normalizedInput = ep.pongVariables.cwiseQuotient(
//         Eigen::Matrix<float, 6, 1>((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, (float)BALL_SPEED,
//                                    (float)BALL_SPEED, (float)SCREEN_HEIGHT, (float)SCREEN_HEIGHT));

//     // Add bias to input
//     Eigen::VectorXf inputWithBias(normalizedInput.size() + 1);
//     inputWithBias.head(normalizedInput.size()) = normalizedInput;
//     inputWithBias(normalizedInput.size()) = 1.0f; // Add bias term

//     // Ensure matrix dimensions are compatible
//     if (delta1.size() != agentRef->W1.rows() || inputWithBias.size() != agentRef->W1.cols()) {
//         throw std::runtime_error("Dimension mismatch for dW1 calculation");
//     }

//     if (delta2.size() != agentRef->W2.rows() || agentOut.size() + 1 != agentRef->W2.cols()) {
//         throw std::runtime_error("Dimension mismatch for dW2 calculation");
//     }

//     // Compute gradients
//     Eigen::MatrixXf dW1 = delta1 * inputWithBias.transpose();
//     Eigen::MatrixXf dW2 = delta2 * agentOut.transpose();

//     LOG("Gradient computation complete");

//     // Return gradients as a pair
//     return std::make_pair(dW1, dW2);
// }



// std::pair<Eigen::MatrixXf, Eigen::MatrixXf>
// Training::gradient(const EpisodeParameter& ep, bool isAgent)
// {
//     LOG("Inside gradient");
//     // References to the correct agent and target
//     auto& agentRef = isAgent ? agent1 : agent2;
//     auto& targetRef = isAgent ? target1 : target2;
//     float reward = isAgent ? ep.reward1 : ep.reward2;


//     // Initialize y based on the Bellman equation
//     float y = 0.0f;
//     if (ep.gameEnd) {
//         y = reward;
//     } else {
//         // Compute target Q-value using the target agent
//         Eigen::Matrix<float, 3, 1> out = DQN(targetRef, ep.pongVariables);
//         float qTarget = out.maxCoeff(); // Maximum Q-value from target agent
//         y = reward + discount * qTarget;
//     }
//     LOG("Called DQN");
//     // Agent's estimate of Q (and intermediate steps)
//     Eigen::Matrix<float, 3, 1> agentOut = DQN(agentRef, ep.pongVariables);

//     // Backpropagation
//     Eigen::Matrix<float, 3, 1> e2 = Eigen::Vector3f::Zero(); // Error in output layer
//     int actionIndex = static_cast<int>(reward); // Assuming reward encodes action
//     e2(actionIndex) = y - agentOut(actionIndex);

//     Eigen::VectorXf delta2 = e2; // Output layer delta

//     // Calculate delta for the hidden layer
//     Eigen::VectorXf e1 = agentRef->W2.leftCols(agentRef->W2.cols() - 1).transpose() * delta2;
//     Eigen::VectorXf delta1 = derActivationF(agentOut.head(agentOut.size() - 1)).cwiseProduct(e1);
//     // (derActivationF(hiddenLayerOutput) * W2.transpose()) * delta2

//     // Normalize input like in DQN
//     Eigen::Matrix<float, 6, 1> normalizedInput = ep.pongVariables.cwiseQuotient(
//         Eigen::Matrix<float, 6, 1>((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, (float)BALL_SPEED,
//                                    (float)BALL_SPEED, (float)SCREEN_HEIGHT, (float)SCREEN_HEIGHT));

//     // // Add bias to input
//     // Eigen::VectorXf inputWithBias(normalizedInput.size() + 1);
//     // inputWithBias.head(normalizedInput.size()) = normalizedInput;
//     // inputWithBias(normalizedInput.size()) = 1.0f; // Add bias term

//     // Compute gradients
//     Eigen::MatrixXf dW1 = delta1 * normalizedInput.transpose();
//     Eigen::MatrixXf dW2 = delta2 * agentOut.transpose();

//     // Return gradients as a pair
//     return std::make_pair(dW1, dW2);
// }