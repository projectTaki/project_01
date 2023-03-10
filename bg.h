#pragma once

enum QuakeNum {
    NONE_QUAKE,
    SHOT_QUAKE,
    BOSS_QUAKE,
    BOSS_DIED_QUAKE
};

class BG
{
public:
    static const int WINDOW_W = 1920;
    static const int WINDOW_H = 1080; 
    static const int WINDOW_W_HALF = 960;
    static const int WINDOW_H_HALF = 540; 


private:
    int   state_;
    int   timer_;

    static int wallSprNo_;
    static VECTOR2 wallPos_;

    static int bgQuakeState_;
    static int bgQuakeAct_;
    static int bgQuakeNum_;
    static VECTOR2 bgQuakePos_;
    static bool isBgQuake_;

    static int wallQuakeState_;
    static int wallQuakeAct_;
    static int wallQuakeNum_;
    static VECTOR2 wallQuakePos_;
    static bool isWallQuake_;

public:
    BG()
        :state_(0)
        , timer_(0)
    {}

    void init();
    void deinit();
    void update();
    void render();

    static void bgQuake();
    static void wallQuake();

public:
    static int bgQuakeNum()     { return bgQuakeNum_; }
    static VECTOR2 bgQuakePos() { return bgQuakePos_; }
    static bool isBgQuake()     { return isBgQuake_; }

    static int wallQuakeNum()     { return wallQuakeNum_; }
    static VECTOR2 wallQuakePos() { return wallQuakePos_; }
    static bool isWallQuake()     { return isWallQuake_; }

public:
    static void setBgQuakeState(int bgQuakeState) { bgQuakeState_ = bgQuakeState; }
    static void setBgQuakeNum(int bgQuakeNum) { bgQuakeNum_ = bgQuakeNum; }
    static void setIsBgQuake(bool isBgQuake) { isBgQuake_ = isBgQuake; }    
    
    static void setWallQuakeState(int wallQuakeState) { wallQuakeState_ = wallQuakeState; }
    static void setWallQuakeNum(int wallQuakeNum)     { wallQuakeNum_   = wallQuakeNum; }
    static void setIsWallQuake(bool isWallQuake)      { isWallQuake_    = isWallQuake; }
};
