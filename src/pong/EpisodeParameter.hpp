struct EpisodeParameter
{
    float ball_x;
    float ball_y;

    float ball_vx;
    float ball_vy;

    // y coordinate of the center of the padddle
    float paddle1;
    float paddle2; 
    int reward1 = 0;
    int reward2 = 0;
    bool gameEnd = false;

};