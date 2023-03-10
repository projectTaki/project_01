#pragma once

class Game : public Scene
{
public:
    static const int gameHpMax_ = 5;

private:
    BG*     pBg_;
    Player* pPlayer_;
    Enemy*  pEnemy_;
    Item*   pItem_;
    Effect* pEffect_;

    static int gameHp_;
    static int copyGameHp_;

    static Game instance_;

    static bool isPlayedSE_;

    struct Wave {
        int     sprNo = 0;
        VECTOR2 pos   = {};
        VECTOR2 size  = {};
        VECTOR2 velocity = {};
    };

public:
    static Wave wave_;

public:
    static Game* instance() { return &instance_; }

    void init() override;
    void deinit() override;
    void update() override;
    void draw() override;

    void reset();

    static int gameHp() { return gameHp_; }
    static int gameHpMax() { return gameHpMax_; }
    static void setGameHp(int gameHp) { gameHp_ = gameHp; }
    static void addGameHp(int gameHp) { gameHp_ += gameHp; }

    static bool isPlayedSE() { return isPlayedSE_; }
    static void setIsPlayedSE(bool isPlayedSE) { isPlayedSE_ = isPlayedSE; }

private:
    Game()
        : pBg_(nullptr)
        , pPlayer_(nullptr)
        , pEnemy_(nullptr)
        , pItem_(nullptr)
        , pEffect_(nullptr)
    {}
    Game(const Game&) = delete; // = delete コピーコンストラクタが存在しないことを明示
};