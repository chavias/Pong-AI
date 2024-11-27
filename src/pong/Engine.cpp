#include "constants.hpp"
#include "Engine.hpp"

namespace pong 
{

void Engine::StepGame(int action1, int action2)
{
    updateBall();


}

void Engine::updateBall()
{
    m_episode.Ball_coordinates[0] += m_episode.Ball_velocity[0];
    m_episode.Ball_coordinates[1] += m_episode.Ball_velocity[1];
    if (m_episode.Ball_coordinates[1] - BALL_RADIUS <= 0
        || m_episode.Ball_coordinates[1] + BALL_RADIUS >= SCREEN_HEIGHT) 
    {
        m_episode.Ball_velocity[1] *= -1;
    }
}

void Engine::updatePaddle(int action1, int action2)
{
    
}
}
