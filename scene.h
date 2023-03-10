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
    int state_ = 0;                // ���
    int timer_ = 0;                // �^�C�}�[
    Scene* nextScene_ = nullptr;   // ���̃V�[��
    

public:
    Scene* execute();   // ���s����

    virtual void init()
    { // ����������
        state_ = 0;
        timer_ = 0;
        nextScene_ = nullptr;
    };

    virtual void deinit() {};   // �I������
    virtual void update() = 0;  // �X�V����
    virtual void draw()   = 0;  // �`�揈��

    void changeScene(Scene *scene) { nextScene_ = scene; }   // �V�[���ύX����
    Scene *getScene() const { return nextScene_; }           // nextScene�̃Q�b�^�[

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
//      �V�[���Ǘ�
//
//******************************************************************************

class SceneManager
{
public:
    void execute(Scene*);  // ���s����
};

//******************************************************************************

bool returnFalse(const bool none);
// ���݈ʒu, �ŏI�ʒu, �ړ����x, �������x
bool moveIn(VECTOR2*, const VECTOR2, VECTOR2*, const VECTOR2);
bool moveInOutRight(VECTOR2* nowPos, VECTOR2* velo);

extern bool (*isGameLoop)(bool);
