#include "all.h"
#include "imgui_manager.h"
using namespace std;

bool Scene::isDispHitRect_ = false;

bool Scene::irisInOut_     = false;
bool Scene::isIrisEnd_     = false; 
int  Scene::irisSprNo_     = RABBIT_NORMAL;
VECTOR4 Scene::irisColor_  = { 0.25f, 0.2f, 0.3f, 1 };

bool (*isGameLoop)(bool) = nullptr;

//--------------------------------------------------------------
//  実行
//--------------------------------------------------------------
Scene* Scene::execute()
{
    using namespace GameLib;

    // シーン遷移するまでの時間
    int changeSceneTimer = 0/*60*/;

    pCursor_ = unique_ptr<Cursor>(new Cursor);
    pCursor_->init();

    // 初期化処理
    init();

    // サウンド読み込み
    Audio::init();

    isGameLoop = GameLib::gameLoop;

    // ゲームループ
    while (isGameLoop(false))    // falseをtrueにするとタイトルバーにフレームレート表示
    {
        // 入力処理
        input::update();

        // 更新処理
        pCursor_->update();
        ImGuiManager::getInstance().newFrame();
        update();
        music::update();

        // ステンシルモード：通常
        DepthStencil::instance().set(DepthStencil::MODE::NONE);

        // 描画処理
        draw();
        //pCursor_->render();
        irisShot(Scene::irisInOut(), &isIrisEnd_ );

        // ステンシルモード：通常
        DepthStencil::instance().set(DepthStencil::MODE::NONE);

        ImGuiManager::getInstance().render();

        // デバッグ文字列の描画
        debug::display(1, 1, 1, 1, 1);
        //debug::setString("GP2_13 SAMPLE");

        // 画面フリップ
        GameLib::present(1, 0);

        // 終了チェック
        if (nextScene_)
        {
            const int irisSprTbl[] = {
                RABBIT_FIRE,
                RABBIT_WATER,
                RABBIT_WIND,
                RABBIT_RARE,
                RABBIT_NORMAL,
            };
            const VECTOR4 irisColorTbl[] = {
                { 0.25f, 0.1f,  0.15f, 1 },
                { 0.1f,  0.1f,  0.25f, 1 },
                { 0.1f,  0.25f, 0.25f, 1 },
                { 0.25f, 0.25f, 0.1f,  1 },
                { 0.25f, 0.2f,  0.3f,  1 },
            };
            static int index = 0;
            irisSprNo_ = irisSprTbl[index];
            irisColor_ = irisColorTbl[index];

            ++index;
            if (index > 4) index = 0;

            --changeSceneTimer;
            if (changeSceneTimer <= 0) break; // timer秒後にシーン遷移

        }
    }


    // 終了処理
    deinit();
    pCursor_->deinit();

    music::stop();

    // 音楽のクリア
    music::clear();

    return nextScene_;	// 次のシーンを返す
}

//******************************************************************************
//
//      SceneManagerクラス
//
//******************************************************************************

//--------------------------------------------------------------
//  実行
//--------------------------------------------------------------
void SceneManager::execute(Scene* scene)
{
    using namespace GameLib;

    srand(static_cast<unsigned int>(time(nullptr)));

    bool isFullScreen = true; // (Releaseモードのみ）

    // ゲームライブラリの初期化処理
    GameLib::init(L"ラビットキャノン", BG::WINDOW_W, BG::WINDOW_H, isFullScreen);

    GameLib::setBlendMode(GameLib::Blender::BS_ALPHA);

    ImGuiManager::getInstance().init();

#ifndef _DEBUG
    ShowCursor(!isFullScreen);	// フルスクリーン時はカーソルを消す
#endif

    // メインループ
    while (scene)
    {
        scene = scene->execute();
    }

    // テクスチャの解放
    texture::releaseAll();

    ImGuiManager::getInstance().shutdown();
    
    // ゲームライブラリの終了処理
    GameLib::uninit();
}

//******************************************************************************

// falseを返すためだけのbool型引数boolの関数
bool returnFalse(const bool none = false) 
{
    return false;
}

// bool inOut (true：irisIn, false：irisOut)
// bool irisEnd
void Scene::irisShot(bool inOut, bool* irisEnd)
{
    static VECTOR2 scale{ 0,0 };
    const  VECTOR2 velocity = inOut ? VECTOR2(0.45f, 0.45f)
                                    : VECTOR2(-0.25f, -0.25f);

    scale += velocity;

    if (scale.x <= 0) scale = { 0,0 };
    if (scale.x >= 25) scale = { 25,25 };

    if (scale.x == 0 || scale.x == 25) *irisEnd = true;
    else *irisEnd = false;

    static int animeX = 0;
    static int animeTimer = 0;

    if (*irisEnd == false)
    {
        ++animeTimer;
        animeX = animeTimer / 4 % 5;
    }

    DepthStencil::instance().set(DepthStencil::MODE::MASK);
    texture::begin(irisSprNo_);
    texture::draw(
        irisSprNo_, { 1920 / 2, 1080 / 2 },
        scale, { 200 * static_cast<float>(animeX), 0 }, 
        { 200,200 }, { 100,100 }, ToRadian(0)
    );
    texture::end(irisSprNo_);

    DepthStencil::instance().set(DepthStencil::MODE::EXCLUSIVE);
    primitive::rect(
        { 0,0 },
        { 1920,1080 },
        { 0,0 }, ToRadian(0),
        irisColor_
    );

    DepthStencil::instance().clear();
    DepthStencil::instance().set(DepthStencil::MODE::NONE);
}

// 現在位置, 最終位置, 移動速度, 加減速度
bool moveIn(VECTOR2* nowPos, VECTOR2 endPos, VECTOR2* velo, const VECTOR2 addVelo)
{
	*nowPos  += *velo;

	if (velo->x >= 0)
	{
		if (nowPos->x > endPos.x) nowPos->x = endPos.x;
	}
	else if (velo->x < 0)
	{
		if (nowPos->x < endPos.x) nowPos->x = endPos.x;
	}

	if (velo->y >= 0)
	{
		if (nowPos->y > endPos.y) nowPos->y = endPos.y;
	}
	else if (velo->x < 0)
	{
		if (nowPos->y < endPos.y) nowPos->y = endPos.y;
	}

	*velo += addVelo;


	if (*nowPos == endPos)
	{
		velo = {};
		return true;
	}

	return false;
}

// 現在位置, 移動速度
bool moveInOutRight(VECTOR2* nowPos, VECTOR2* velo)
{
    const float changeVeloPoint1 = BG::WINDOW_W_HALF - 110;
    const float changeVeloPoint2 = BG::WINDOW_W_HALF + 40;

    if (nowPos->x >= BG::WINDOW_W + 500) return true;

    *nowPos += *velo;

    if (nowPos->x >= changeVeloPoint1 && nowPos->x < changeVeloPoint2)
    {
        velo->x *= 0.5f;
        if (velo->x < 0.2f) velo->x = 0.2f;
    }
    if (nowPos->x >= changeVeloPoint2)
    {
        velo->x *= 1.15f;
    }

    return false;
}