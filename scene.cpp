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
//  ���s
//--------------------------------------------------------------
Scene* Scene::execute()
{
    using namespace GameLib;

    // �V�[���J�ڂ���܂ł̎���
    int changeSceneTimer = 0/*60*/;

    pCursor_ = unique_ptr<Cursor>(new Cursor);
    pCursor_->init();

    // ����������
    init();

    // �T�E���h�ǂݍ���
    Audio::init();

    isGameLoop = GameLib::gameLoop;

    // �Q�[�����[�v
    while (isGameLoop(false))    // false��true�ɂ���ƃ^�C�g���o�[�Ƀt���[�����[�g�\��
    {
        // ���͏���
        input::update();

        // �X�V����
        pCursor_->update();
        ImGuiManager::getInstance().newFrame();
        update();
        music::update();

        // �X�e���V�����[�h�F�ʏ�
        DepthStencil::instance().set(DepthStencil::MODE::NONE);

        // �`�揈��
        draw();
        //pCursor_->render();
        irisShot(Scene::irisInOut(), &isIrisEnd_ );

        // �X�e���V�����[�h�F�ʏ�
        DepthStencil::instance().set(DepthStencil::MODE::NONE);

        ImGuiManager::getInstance().render();

        // �f�o�b�O������̕`��
        debug::display(1, 1, 1, 1, 1);
        //debug::setString("GP2_13 SAMPLE");

        // ��ʃt���b�v
        GameLib::present(1, 0);

        // �I���`�F�b�N
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
            if (changeSceneTimer <= 0) break; // timer�b��ɃV�[���J��

        }
    }


    // �I������
    deinit();
    pCursor_->deinit();

    music::stop();

    // ���y�̃N���A
    music::clear();

    return nextScene_;	// ���̃V�[����Ԃ�
}

//******************************************************************************
//
//      SceneManager�N���X
//
//******************************************************************************

//--------------------------------------------------------------
//  ���s
//--------------------------------------------------------------
void SceneManager::execute(Scene* scene)
{
    using namespace GameLib;

    srand(static_cast<unsigned int>(time(nullptr)));

    bool isFullScreen = true; // (Release���[�h�̂݁j

    // �Q�[�����C�u�����̏���������
    GameLib::init(L"���r�b�g�L���m��", BG::WINDOW_W, BG::WINDOW_H, isFullScreen);

    GameLib::setBlendMode(GameLib::Blender::BS_ALPHA);

    ImGuiManager::getInstance().init();

#ifndef _DEBUG
    ShowCursor(!isFullScreen);	// �t���X�N���[�����̓J�[�\��������
#endif

    // ���C�����[�v
    while (scene)
    {
        scene = scene->execute();
    }

    // �e�N�X�`���̉��
    texture::releaseAll();

    ImGuiManager::getInstance().shutdown();
    
    // �Q�[�����C�u�����̏I������
    GameLib::uninit();
}

//******************************************************************************

// false��Ԃ����߂�����bool�^����bool�̊֐�
bool returnFalse(const bool none = false) 
{
    return false;
}

// bool inOut (true�FirisIn, false�FirisOut)
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

// ���݈ʒu, �ŏI�ʒu, �ړ����x, �������x
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

// ���݈ʒu, �ړ����x
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