#pragma once

class Effect
{
public:
    static const int efcMax = 100;
    static OBJ2D* effect_[efcMax];

private:
    int state_;
    int timer_;

public:
    Effect()
        :state_(0)
        , timer_(0)
    {}

    void init();
    void deinit();
    void update();
    void render();

    void setEfc(OBJ2D* obj);

};

void wallSmokeEfc(OBJ2D* obj);
void elementEfc(OBJ2D* obj);