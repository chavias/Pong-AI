#include "Game.hpp"
#include "Training.hpp"

int main() {

    // std::unique_ptr<Game> game;

    // // game.set_paddles(std::make_unique<PlayerPaddle>(PADDLE1_X, PADDLE_Y),
    // //                  std::make_unique<PlayerPaddle>(PADDLE2_X, PADDLE_Y));

    // game->Run();


    std::unique_ptr<Training> training = std::make_unique<Training>();
    
    training->train();

    training->playGame();




    return 0;
}
