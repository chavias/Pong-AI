#include "Game.hpp"
#include "constants.hpp"
#include <iostream>

Game::Game(std::unique_ptr<Paddle> &&p1, std::unique_ptr<Paddle> &&p2)
    : paddle1(std::move(p1)),
      paddle2(std::move(p2)),
      ball(std::make_unique<Ball>()),
      scoreManager(std::make_unique<ScoreManager>())
{
    if (!paddle1)
    {
        paddle1 = std::make_unique<CpuPaddle>(PADDLE1_X, PADDLE_Y);
    }
    if (!paddle2)
    {
        paddle2 = std::make_unique<PlayerPaddle>(PADDLE2_X, PADDLE_Y);
    }
}

Game::~Game() {}

void Game::Run()
{
    // Should maybe moved to the constructor
    InitWindow(RENDER_WIDTH, RENDER_HEIGHT, "Pong");

    RenderTexture2D simTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    SetTargetFPS(50);
    while (!WindowShouldClose())
    {
        float deltaTime = 0.1f; // GetFrameTime();
        // Update(deltaTime);
        Action action1 = WAIT;
        Action action2 = WAIT;
        EpisodeParameter state = Step(deltaTime, action1, action2);
        if (state.gameEnd)
            Reset();
        
        // Render the simulation scene to the low-res render texture.
        BeginTextureMode(simTexture);
            RenderSimulation();
        EndTextureMode();
    
        // Now draw the simulation texture scaled up to the window size.
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(simTexture.texture,
                           // Source rectangle (flip vertically with negative height)
                           (Rectangle){ 0, 0, (float)simTexture.texture.width, -(float)simTexture.texture.height },
                           // Destination rectangle: scale up to fill the window
                           (Rectangle){ 0, 0, (float)RENDER_WIDTH, (float)RENDER_HEIGHT },
                           (Vector2){ 0, 0 },
                           0.0f,
                           WHITE);
        EndDrawing();    
    }

    CloseWindow();
}

void Game::RenderSimulation() const
{
    ClearBackground(DARKGREEN);

    // Draw background elements using simulation coordinates.
    // (Here we map the drawing to SCREEN_WIDTH and SCREEN_HEIGHT instead of the full screen.)
    DrawRectangle(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, DARKGREEN);
    // Scale the circle radius relative to the simulation width.
    DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 150 * ((float)SCREEN_WIDTH / RENDER_WIDTH), LIME);
    DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);

    // Draw game objects (ball, paddles, score) assuming their positions are in simulation space.
    ball->Draw();
    paddle1->Draw();
    paddle2->Draw();
    scoreManager->DrawScore();
}



/// @brief Provides State and Rewards for the Agent to learn
/// @param deltaTime, action1, action2,
/// @return EpisodeParameter structure represents current reward and game state
EpisodeParameter Game::Step(float deltaTime, Action action1, Action action2)
{
    bool gameEnd = false;
    int reward1 = 0;
    int reward2 = 0;
    // Update paddles and ball
    paddle1->Update(deltaTime, ball->y, action1);
    paddle2->Update(deltaTime, ball->y, action2);
    ball->Update(deltaTime);

    // Collision checks
    if (CheckCollisionCircleRec(Vector2{ball->x, ball->y}, ball->radius,
                                Rectangle{paddle1->x, paddle1->y, paddle1->width, paddle1->height}))
    {
        ball->speed_x *= -1;
        reward1 = 1; // increase reward
        ball->x += PADDLE_WIDTH / 2;
        // std::cout << "+1 left" << std::endl;
        ball->speed_y += GetRandomValue(-0.4, 0.4);
    }
    if (CheckCollisionCircleRec(Vector2{ball->x, ball->y}, ball->radius,
                                Rectangle{paddle2->x, paddle2->y, paddle2->width, paddle2->height}))
    {
        ball->speed_x *= -1;
        ball->x -= PADDLE_WIDTH / 2;
        reward2 = 1; // increase reward
        // std::cout << "+1 right" << std::endl;
        ball->speed_y += GetRandomValue(-0.4, 0.4);
    }

    Eigen::Matrix<float, 6, 1> pongVariables(ball->x, ball->y, ball->speed_x, ball->speed_y, paddle1->y, paddle2->y);

    // count score and reset
    if (ball->x - ball->radius <= 0)
    {
        scoreManager->RightScored();
        reward1 = -1; // decrease reward
        // std::cout << "-1 left" << std::endl;
        // ball->speed_x *= -1;
        gameEnd = true;
        // Reset();
    }
    else if (ball->x + ball->radius >= SCREEN_WIDTH)
    {
        scoreManager->LeftScored();
        reward2 = -1; // decrease reward
        // ball->speed_x *= -1;
        // std::cout << "-1 right" << std::endl;
        gameEnd = true;
        // Reset();
    }

    return {pongVariables, action1, action2, reward1, reward2, gameEnd};
}

void Game::Render() const
{
    BeginDrawing();
    ClearBackground(DARKGREEN);

    DrawRectangle(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, DARKGREEN);
    DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 150, LIME);
    DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);

    ball->Draw();
    paddle1->Draw();
    paddle2->Draw();
    scoreManager->DrawScore();

    EndDrawing();
}

EpisodeParameter Game::Reset()
{
    ball->Reset();
    // ball->Reflect();
    paddle1->Reset();
    paddle2->Reset();
    Eigen::Matrix<float, 6, 1> pongVariables(ball->x, ball->y, ball->speed_x, ball->speed_y, paddle1->y, paddle2->y);
    EpisodeParameter ep(pongVariables, WAIT, WAIT, 0, 0, false);
    return ep;
}