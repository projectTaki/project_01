#pragma once

const int shotSpr[] = {
    CANNON_SHOT_NORMAL,
    CANNON_SHOT_FIRE,
    CANNON_SHOT_WATER,
    CANNON_SHOT_WIND,
    CANNON_SHOT_RARE,
};

class Player
{
public:
    static const int plMax = 4;
    static OBJ2D* players_[plMax];

private:
    int state_;
    int timer_;

    struct Animation {
        int animeX = 0;
        int animeY = 0;
        int animeTimer = 0;
        VECTOR2 texPos{};
        bool isDisp = false;
    } cannonRabbit[plMax], cannonEfc[plMax];

    //VECTOR2 size{ 400, 200 };
    //VECTOR2 size{ 270, 250 };
    
public:
    Player()
        :state_(0)
        , timer_(0)
    {}

void init();
void deinit();
void update();
void render();
void setPlayer();

};