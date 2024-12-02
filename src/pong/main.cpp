#include "Game.hpp"

int main() {

    // std::unique_ptr<Game> game;
    Game* game = new Game;

    // game.set_paddles(std::make_unique<PlayerPaddle>(PADDLE1_X, PADDLE_Y),
    //                  std::make_unique<PlayerPaddle>(PADDLE2_X, PADDLE_Y));
    
    
    
    game->Run();
    return 0;
}
