#pragma once

class Item
{
public:
    static const int itemMax = 30;
    static const int itemMinPosY = 480;
    static const int itemMaxPosY = 690;

    static OBJ2D* items_[itemMax];

private:
    int state_ = 0;
    int timer_ = 0;

public:
    Item()
        :state_(0)
        , timer_(0)
    {}

    void init();
    void deinit();
    void update();
    void render();

    static void setItem(OBJ2D* obj);
    static void setItem(OBJ2D* arr[]);

};

void rabbitMove1(OBJ2D* obj);
void rabbitMove2(OBJ2D* obj);
