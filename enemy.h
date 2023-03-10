#pragma once

#define ENEMY_S_HP     (4)
#define ENEMY_M_HP     (8)
#define BOSS_LLRR_HP   (6)
#define BOSS_LR_HP     (6)
#define ELM_BOSS_LLRR_HP   (17)
#define ELM_BOSS_LR_HP     (13)
#define BOSS_CENTER_HP (100)

#define ENEMY_S_ATK_TIMER   (9 * 60)
#define ENEMY_M_ATK_TIMER   (7 * 60)
#define BOSS_LLRR_ATK_TIMER (6 * 60)

extern bool isPutEnm[4];

const int enmAtkTimer = 600;

const VECTOR2 enmPos[] = {
    {280,  210},
    {720,  210},
    {1170, 210},
    {1610, 210},
};

// int round, int timer, int type, int sprNo, VECTOR2 pos
struct EnemySet {
    int round;
    int timer;
    int type;
    int sprNo;
    VECTOR2 pos;
};

class Enemy
{
public:
    static const int enemyMax = 20;

    static OBJ2D* enemys_[enemyMax];

    static EnemySet* enemySet_;

private:
    int state_;
    int timer_;


    static VECTOR2 moveBoss_;
    static int bossLRNum_;
    static bool killAllEnm_;

    static int enmRound_;
    static int existEnmNum_;
    static bool endEnm_[enemyMax];

public:
    Enemy()
        :state_(0)
        , timer_(0)
    {}

    void init();
    void deinit();
    void update();
    void render();

    void enmCreate();
    void resetEnemy(OBJ2D* obj);
    void killAllEnm();

public:
    void enmS(OBJ2D* obj);
    void enmM(OBJ2D* obj);
    void bossLR(OBJ2D* obj);
    void bossLLRR(OBJ2D* obj);
    void bossCenter(OBJ2D* obj);

public:
    static int bossLRNum() { return bossLRNum_; }

};

extern EnemySet* enemySets[];