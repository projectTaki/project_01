#pragma once

class Scene
{
private:
    std::unique_ptr<Cursor> pCursor_;
    static bool irisInOut_;

    static int  irisSprNo_;
    static VECTOR4 irisColor_;
public:
    static bool isDispHitRect_;
    static bool isIrisEnd_;


protected:
    int state_ = 0;                // 状態
    int timer_ = 0;                // タイマー
    Scene* nextScene_ = nullptr;   // 次のシーン
    

public:
    Scene* execute();   // 実行処理

    virtual void init()
    { // 初期化処理
        state_ = 0;
        timer_ = 0;
        nextScene_ = nullptr;
    };

    virtual void deinit() {};   // 終了処理
    virtual void update() = 0;  // 更新処理
    virtual void draw()   = 0;  // 描画処理

    void changeScene(Scene *scene) { nextScene_ = scene; }   // シーン変更処理
    Scene *getScene() const { return nextScene_; }           // nextSceneのゲッター

    int getState() const { return state_; }
    void setState(int s) { state_ = s; }
    void setTimer(int t) { timer_ = t; }

public:
    static bool irisInOut() { return irisInOut_; }
    static void setIrisInOut(bool irisInOut) { irisInOut_ = irisInOut; }

    void irisShot(bool inOut, bool* irisEnd);
};

//******************************************************************************
//
//      シーン管理
//
//******************************************************************************

class SceneManager
{
public:
    void execute(Scene*);  // 実行処理
};

//******************************************************************************

bool returnFalse(const bool none);
// 現在位置, 最終位置, 移動速度, 加減速度
bool moveIn(VECTOR2*, const VECTOR2, VECTOR2*, const VECTOR2);
bool moveInOutRight(VECTOR2* nowPos, VECTOR2* velo);

extern bool (*isGameLoop)(bool);
