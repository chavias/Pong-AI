#include "Game.hpp"
#include <array>
#include <utility>

/*
 * The state is represented as an array with 6 elements
 * (ball_x, ball_y, ball_velocity_x, ball_velocity_y, paddle_position_middle)
 * 
 * The action of the agent are represented with [0,1,2]
 */


namespace pong 
{

struct EpisodeParameter
{
    std::array<double, 2> Ball_coordinates;
    std::array<double, 2> Ball_velocity;
    double paddle_coordinate; // y coordinate of the center of the padddle
    int reward1 = 0;
    int reward2 = 0;
    bool gameEnd;

};


class Engine 
{
public:
    Engine(EpisodeParameter&& episode)
        : m_episode(std::move(episode)) {}

    void StepGame(int action1, int action2);


private:
    EpisodeParameter m_episode;

    void updateBall();

    void updatePaddle(int action1, int action2);


};

}