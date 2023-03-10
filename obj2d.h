#pragma once
using namespace GameLib;

enum OBJ_TYPE
{
    NONE,
    PLAYER,
    ENEMY,
    ITEM,
    SHOT,
    EFFECT,
};

enum OBJ_ELEMENT
{
    NORMAL,
    FIRE,
    WATER,
    WIND,
    RARE,
};


class Transform
{
private:
    VECTOR2 pos_;
    VECTOR2 scale_;     
    VECTOR2 center_;    
    VECTOR2 texPos_;    
    VECTOR2 pivot_;     
    float   angle_;     
    VECTOR2 offset_;    
    VECTOR2 orgPos_;    
    VECTOR2 velocity_;  
    VECTOR2 orgVelocity_;

public:
    Transform()
        : angle_(0)
    {}

    VECTOR2 pos()       const { return pos_; }
    VECTOR2 scale()     const { return scale_; }
    VECTOR2 center()    const { return center_; }
    VECTOR2 texPos()    const { return texPos_; }
    VECTOR2 pivot()     const { return pivot_; }
    float   angle()     const { return angle_; }
    VECTOR2 offset()    const { return offset_; }
    VECTOR2 orgPos()    const { return orgPos_; }
    VECTOR2 velocity()  const { return velocity_; }
    float   velocityX() const { return velocity_.x; }
    VECTOR2 orgVelocity() const { return orgVelocity_; }

public:
    void addPos(VECTOR2 pos) { pos_ += pos; }
    void addPosX(float posX) { pos_.x += posX; }
    void addPosY(float posY) { pos_.y += posY; }
    void addScale(VECTOR2 scale) { scale_ += scale; }
    void addTexPos(VECTOR2 texPos) { texPos_ += texPos; }
    void addTexPosY(float texPosY) { texPos_.y += texPosY; }
    void addAngle(float angle) { angle_ += angle; }
    void addVelocity(VECTOR2 velocity) { velocity_ += velocity; }
    void addVelocityX(float velocityX) { velocity_.x += velocityX; }
    void addVelocityY(float velocityY) { velocity_.y += velocityY; }

public:
    void setPos(VECTOR2 pos)           { pos_ = pos; }
    void setPosY(float posY)           { pos_.y = posY; }
    void setScale(VECTOR2 scale)       { scale_ = scale; }
    void setCenter(VECTOR2 center)     { center_ = center; }
    void setTexPos(VECTOR2 texPos)     { texPos_ = texPos; }
    void setTexPosX(float texPosX)     { texPos_.x = texPosX; }
    void setTexPosY(float texPosY)     { texPos_.y = texPosY; }
    void setAngle(float angle)         { angle_ = angle; }
    void setOffset(VECTOR2 offset)     { offset_ = offset; }
    void setOrgPos(VECTOR2 orgPos)     { orgPos_ = orgPos; }
    void setVelocity(VECTOR2 velocity) { velocity_ = velocity; }
    void setOrgVelocity(VECTOR2 orgVelocity) { orgVelocity_ = orgVelocity; }
};

class Renderer
{
private:
    int     sprNo_;
    int     animeX_;
    int     animeY_;
    int     animeTimer_;
    char    pad_[3];      // 余白
    VECTOR4 color_;

public:
    Renderer()
        :sprNo_(0)
        , animeX_(0)
        , animeY_(0)
        , animeTimer_(0)
        , pad_()
        , color_(0, 0, 0, 0)
    {}

    int sprNo()   const { return sprNo_; }
    int animeX()  const { return animeX_; }
    int animeY()  const { return animeY_; }
    int animeTimer() const { return animeTimer_; }
    VECTOR4 color()  const { return color_; }

public:
    void addAnimeTimer(int animeTimer) { animeTimer_ += animeTimer; }
    void addAnimeX(int animeX) { animeX_ += animeX; }
    void addColor(VECTOR4 color) { color_ += color; }
    void addColorW(float colorW) { color_.w += colorW; }
public:
    void setSprNo(int sprNo) { sprNo_ = sprNo; }
    void setAnimeX(int animeX) { animeX_ = animeX; }
    void setAnimeY(int animeY) { animeY_ = animeY; }
    void setAnimeTimer(int animeTimer) { animeTimer_ = animeTimer; }
    void setColor(VECTOR4 color) { color_ = color; }
};

class Collider
{
private:
    VECTOR2 size_;
    VECTOR2 hitSize_;

public:
    VECTOR2 size()  const { return size_; }
    VECTOR2 hitSize() const { return hitSize_; }

public:
    void setSize(VECTOR2 size) { size_ = size; }
    void setHitSize(VECTOR2 hitSize) { hitSize_ = hitSize; }
};

class OBJ2D;
class ActorComponent
{
private:
    int             act_;         // 行動遷移用
    int             hpMax_;
    int             hp_;
    int             atk_;
    int             dmgTimer_;
    int             atkTimer_;
    int             atkTimerMax_;
    int             score_;
    OBJ2D*          parent_;

public:
    int act() const {return act_;}
    int hp() const {return hp_;}
    int hpMax() const {return hpMax_;}
    int atk() const {return atk_;}
    int dmgTimer() const {return dmgTimer_;}
    int atkTimer() const { return atkTimer_; }
    int atkTimerMax() const { return atkTimerMax_; }
    int score() const {return score_;}
    OBJ2D* parent() const { return parent_; }

public:
    void setAct(int act) { act_ = act; }
    void setHp(int hp) { hp_ = hp; }
    void setHpMax(int hpMax) { hpMax_ = hpMax; }
    void setAtk(int atk) { atk_ = atk; }
    void setDmgTimer(int dmgTimer) { dmgTimer_ = dmgTimer; }
    void setAtkTimer(int atkTimer) { atkTimer_ = atkTimer; }
    void setAtkTimerMax(int atkTimerMax) { atkTimerMax_ = atkTimerMax; }
    void setScore(int score) { score_ = score; }
    void setParent(OBJ2D* parent) { parent_ = parent; }

public:
    void addHp(int hp) { hp_ += hp; }
    void subHp(int hp) { hp_ -= hp; }
    void subDmgTimer(int dmgTimer) { dmgTimer_ -= dmgTimer; }
    void subAtkTimer(int atkTimer) { atkTimer_ -= atkTimer; }
};


class OBJ2D
{
public:
    void (*mover_)(OBJ2D* obj);
private:
    Transform* transform_;
    Renderer*  renderer_;
    Collider*  collider_;
    ActorComponent* actorComponent_;

    int  state_;       // 状態
    int  timer_;       // タイマー
    int  type_;        // タイプ
    int  element_;
    bool exist_;       // 存在フラグ
    bool hostilityFg_; // 敵か味方か
    bool isHold_;      // つかんでいるかどうか
    bool isTouchCmd_;  // コマンドに触れているか
    bool isInRabbit_;  // うさぎが大砲に入っているか
    bool isFlipX_;

public:
    OBJ2D(Transform* transform, Renderer* renderer,
        Collider* collider, ActorComponent* actorComponent)
        :mover_(nullptr)
        , transform_(transform)
        , renderer_(renderer)
        , collider_(collider)
        , actorComponent_(actorComponent)
        , state_(0)
        , timer_(0)
        , type_(0)
        , element_(0)
        , exist_(false)
        , hostilityFg_(false)
        , isHold_(false)
        , isTouchCmd_(false)
        , isInRabbit_(false)
        , isFlipX_(false)
    {}

    Transform* transform() const { return transform_; }
    Renderer* renderer() const { return renderer_; }
    Collider* collider() const { return collider_; }
    ActorComponent* actorComponent() const { return actorComponent_; }

    int state()  const { return state_; }
    int timer()  const { return timer_; }
    int type()   const { return type_; }
    int element() const { return element_; }
    bool exist() const { return exist_; }
    bool hostilityFg() const { return hostilityFg_; }
    bool isHold() const { return isHold_; }
    bool isTouchCmd() const { return isTouchCmd_; }
    bool isInRabbit() const { return isInRabbit_; }
    bool isFlipX() const { return isFlipX_; }

public:
    void setState(int state) { state_ = state; }
    void setTimer(int timer) { timer_ = timer; }
    void setElement(int element) { element_ = element; }
    void setExist(bool exist) { exist_ = exist; }
    void setIsHold(bool isHold) { isHold_ = isHold; }
    void setIsTouchCmd(bool isTouchCmd) { isTouchCmd_ = isTouchCmd; }
    void setIsInRabbit(bool isInRabbit) { isInRabbit_ = isInRabbit; }
    void setIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; }

public:
    void addTimer(int timer) { timer_ += timer; }

public:
    static OBJ2D* searchSet(OBJ2D* obj, int type, VECTOR2 pos, const bool hostilityFg);
    static OBJ2D* searchSet(OBJ2D* arr[], int arrayNum, int type, VECTOR2 pos, const bool hostilityFg);
    static OBJ2D* searchSet(OBJ2D* arr[], int arrayNum, int type, int sprNo, VECTOR2 pos, VECTOR2 scale, const bool hostilityFg);
    static bool   areaCheck(OBJ2D*, float);
};


