#include "Game.hpp"

int main() {

    Game game;

    game.set_paddles(std::make_unique<PlayerPaddle>(PADDLE1_X, PADDLE_Y),
                     std::make_unique<PlayerPaddle>(PADDLE2_X, PADDLE_Y));
    
    
    
    game.Run();
    return 0;
}
