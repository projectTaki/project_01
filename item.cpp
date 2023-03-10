#include "all.h"
using namespace std;

OBJ2D* Item::items_[itemMax];

void Item::init()
{
    state_ = 0;
    timer_ = 0;

    for (auto& item : items_)
    {
        item = new OBJ2D(
            new Transform,
            new Renderer,
            new Collider,
            new ActorComponent
        );
    }
}

void Item::deinit()
{
    for (auto& item : items_) safe_delete(item);
}

void Item::update()
{
    using namespace input;
    int i;

    switch (state_)
    {
    case 0:
        timer_ = 0;     

        ++state_;
        [[fallthrough]];
    case 1:
        // 生成
        for (auto& item : items_)
        {
            if (item->exist()) continue;

            const int createTimer = (Enemy::bossLRNum() == 2) 
                                  ? timer_ % 25 
                                  : timer_ % 55;

            if (createTimer != 0) break;

                // パラメータ設定
                setItem(item);

                const int rabbitsSpr[] = {
                    RABBIT_NORMAL,
                    RABBIT_FIRE,
                    RABBIT_WATER,
                    RABBIT_WIND,
                };
                const int rabbitsElm[] = {
                    NORMAL,
                    FIRE,
                    WATER,
                    WIND,
                };
                const int random = rand() % 4;
                item->renderer()->setSprNo(rabbitsSpr[random]);
                item->setElement(rabbitsElm[random]);

                item->transform()->setOrgVelocity(VECTOR2(3.5f, 0));
                item->transform()->setVelocity(item->transform()->orgVelocity());


                // Rare
                const int randRare = rand() % 50;
                if (randRare == 7)
                {
                    item->renderer()->setSprNo(RABBIT_RARE);
                    item->setElement(RARE);
                    item->transform()->setOrgVelocity(VECTOR2(25.0f, 0));
                    item->transform()->setVelocity(item->transform()->orgVelocity());
                }

                if (item->element() != NORMAL &&
                    item->element() != RARE)
                {
                    switch (rand() % 4)
                    {
                    case 0:
                        break;
                    case 1:
                        item->transform()->setOrgVelocity(VECTOR2(7.0f, 0));
                        item->transform()->setVelocity(item->transform()->orgVelocity());
                        break;
                    case 2:
                        item->mover_ = rabbitMove1;
                        break;
                    case 3:
                        item->mover_ = rabbitMove2;
                        break;
                    }
                }


                // XがareaCheck()の範囲を超えないように注意
                VECTOR2 pos{ -100, float(rand() % (itemMaxPosY - itemMinPosY)) + itemMinPosY };

                // Flip
                if (rand() % 2 == 0)
                {
                    item->setIsFlipX(false);
                    pos.x = -80;
                    item->transform()->setScale(VECTOR2(1, 1));
                }
                else
                {
                    item->setIsFlipX(true);
                    pos.x = 1920 + 80;
                    item->transform()->setScale(VECTOR2(-1, 1));
                }

                
                OBJ2D::searchSet(item, ITEM, pos, false);
                break;
        }

        // 更新
        for (i = 0; i < itemMax; ++i)
        {
            if (!items_[i]->exist()) continue;

            // item
            if (items_[i]->type() == ITEM)
            {
                // isHold
                if (items_[i]->isHold())
                {
                    items_[i]->transform()->setPos(Cursor::cursor_->transform()->pos() + VECTOR2(0, 90));
                }
                else
                {
                    // レーン外のitem_のposYを修正
                    if (items_[i]->transform()->pos().y < itemMinPosY)
                    {
                        items_[i]->transform()->addPosY(40);
                        if(items_[i]->transform()->pos().y > itemMinPosY) items_[i]->transform()->setPosY(itemMinPosY);
                    }
                    else if (items_[i]->transform()->pos().y > itemMaxPosY)
                    {
                        items_[i]->transform()->addPosY(-40);
                        if (items_[i]->transform()->pos().y < itemMaxPosY) items_[i]->transform()->setPosY(itemMaxPosY);
                    }
                    else
                    {
                        if (Enemy::bossLRNum() == 2 && items_[i]->renderer()->sprNo() != RABBIT_RARE)
                        {
                            items_[i]->transform()->setVelocity(VECTOR2(8.0f, 0));
                        }
                        else items_[i]->transform()->setVelocity(items_[i]->transform()->orgVelocity());

                        if (items_[i]->mover_) items_[i]->mover_(items_[i]);

                        if (!items_[i]->isFlipX())
                        {
                            items_[i]->transform()->addPos(
                                VECTOR2(items_[i]->transform()->velocity().x, 0.0f)
                            );
                        }
                        else items_[i]->transform()->addPos(VECTOR2(-items_[i]->transform()->velocity().x, 0.0f));
                    }
                }
            }
            // shot
            else if (items_[i]->type() == SHOT)
            {

                if (Enemy::bossLRNum() == 2)
                {
                    float dx = items_[i]->transform()->pos().x - 960;
                    float dy = items_[i]->transform()->pos().y - 210;
                    float dist = sqrtf(dx * dx + dy * dy);
                    items_[i]->transform()->setVelocity(VECTOR2(dx / dist * 8.0f, dy / dist * 8.0f));

                    items_[i]->transform()->addPos(-items_[i]->transform()->velocity());
                }
                else items_[i]->transform()->addPosY(-10);


                items_[i]->transform()->addAngle(ToRadian(10));


                if (items_[i]->transform()->scale().x > 0)
                {
                    if (items_[i]->transform()->pos().y > 550)
                         items_[i]->transform()->addScale(VECTOR2(0.03f, 0.03f));
                    else items_[i]->transform()->addScale(VECTOR2(-0.05f, -0.05f));
                }
                else items_[i]->transform()->setScale(VECTOR2(0, 0));
            }

            if (!(STATE(0) & PAD_LCLICK)) items_[i]->setIsHold(false);

            // エリア外チェック
            OBJ2D::areaCheck(items_[i], 250);
        }

        ++timer_;
        break;
    }
}


void Item::render()
{
    unique_ptr<OBJ2D> FrontItem = 
        unique_ptr<OBJ2D>(new OBJ2D(
            new Transform,
            new Renderer,
            new Collider,
            nullptr
        ));    
    struct ShotCopy {
        int sprNo[itemMax]{};
        VECTOR2 pos[itemMax]{};
        VECTOR2 scale[itemMax]{};
        VECTOR2 texPos[itemMax]{};
        VECTOR2 size[itemMax]{};
        VECTOR2 center[itemMax]{};
        float angle[itemMax]{};
        VECTOR4 color[itemMax]{};
    } shotCopy;

    bool isItemCopy = false;
    bool isShotCopy[itemMax]{};

    for (int i = 0; i < itemMax; ++i)
    {
        if (!items_[i]->exist()) continue;
        // 描画範囲
        if (items_[i]->transform()->pos().x < -200 ||
            items_[i]->transform()->pos().y < -200 ||
            items_[i]->transform()->pos().x > BG::WINDOW_W + 200 ||
            items_[i]->transform()->pos().y > BG::WINDOW_H + 200) continue;

 
        texture::begin(items_[i]->renderer()->sprNo());
        texture::draw(
            items_[i]->renderer()->sprNo(),
            items_[i]->transform()->pos() + BG::bgQuakePos(),
            items_[i]->transform()->scale(),
            items_[i]->transform()->texPos(),
            items_[i]->collider()->size(),
            items_[i]->transform()->center(),
            items_[i]->transform()->angle(),
            items_[i]->renderer()->color()
        );
        texture::end(items_[i]->renderer()->sprNo());
        
        
        if (items_[i]->isHold())
        {
            FrontItem->renderer()->setSprNo(items_[i]->renderer()->sprNo());
            FrontItem->transform()->setPos(items_[i]->transform()->pos());
            FrontItem->transform()->setScale(items_[i]->transform()->scale());
            FrontItem->transform()->setTexPos(items_[i]->transform()->texPos());
            FrontItem->collider()->setSize(items_[i]->collider()->size());
            FrontItem->transform()->setCenter(items_[i]->transform()->center());
            FrontItem->transform()->setAngle(items_[i]->transform()->angle());
            FrontItem->renderer()->setColor(items_[i]->renderer()->color());
            isItemCopy = true;
        }
        if (items_[i]->type() == SHOT)
        {
            shotCopy.sprNo[i]  = items_[i]->renderer()->sprNo();
            shotCopy.pos[i]    = items_[i]->transform()->pos();
            shotCopy.scale[i]  = items_[i]->transform()->scale();
            shotCopy.texPos[i] = items_[i]->transform()->texPos();
            shotCopy.size[i]   = items_[i]->collider()->size();
            shotCopy.center[i] = items_[i]->transform()->center();
            shotCopy.angle[i]  = items_[i]->transform()->angle();
            shotCopy.color[i]  = items_[i]->renderer()->color();
            isShotCopy[i] = true;
        }


       // HitRect
       if (Scene::isDispHitRect_)
       {
           primitive::rect(
               items_[i]->transform()->pos() + items_[i]->transform()->offset(),
               items_[i]->collider()->hitSize(),
               {0,0}, 0, { 0, 0, 1, 0.5f }
           );
       }

       // Animation Update
       items_[i]->transform()->setTexPos(VECTOR2(
           float(items_[i]->renderer()->animeX()) * items_[i]->collider()->size().x,
           float(items_[i]->renderer()->animeY()) * items_[i]->collider()->size().y)
       );

       if (items_[i]->type() == ITEM)
       {
           if (items_[i]->isHold())
           {
               items_[i]->renderer()->setAnimeY(1);
               items_[i]->renderer()->setAnimeX(items_[i]->renderer()->animeTimer() / 7 % 4);
           }
           else
           {
               items_[i]->renderer()->setAnimeY(0);

               // スピードが速くなるにつれてアニメーションも早くさせる
               int velocity = static_cast<int>(items_[i]->transform()->velocity().x) < 10
                            ? static_cast<int>(items_[i]->transform()->velocity().x)
                            : 9;
               items_[i]->renderer()->setAnimeX(items_[i]->renderer()->animeTimer() / (10 - velocity) % 5);
           }
           items_[i]->renderer()->addAnimeTimer(1);
       }


       //// debug::setString()
       // 
       // isHoldチェック
       //if (items_[i]->isHold()) debug::setString("hold");
       // existチェック
       //if (items_[i]->exist()) debug::setString("true");
       // Velocityチェック
       //debug::setString("velocityX:%f", items_[i]->transform()->velocity().x);
    }

    if (isItemCopy)
    {
        texture::begin(FrontItem->renderer()->sprNo());
        texture::draw(
            FrontItem->renderer()->sprNo(),
            FrontItem->transform()->pos() + BG::bgQuakePos(),
            FrontItem->transform()->scale(),
            FrontItem->transform()->texPos(),
            FrontItem->collider()->size(),
            FrontItem->transform()->center(),
            FrontItem->transform()->angle(),
            FrontItem->renderer()->color()
        );
        texture::end(FrontItem->renderer()->sprNo());
    }
    for (int i = 0; i < itemMax; ++i)
    {
        if (!isShotCopy[i]) continue;

            texture::begin(shotCopy.sprNo[i]);
            texture::draw(
                shotCopy.sprNo[i],
                shotCopy.pos[i] + BG::bgQuakePos(),
                shotCopy.scale[i],
                shotCopy.texPos[i],
                shotCopy.size[i],
                shotCopy.center[i],
                shotCopy.angle[i],
                shotCopy.color[i]
            );
            texture::end(shotCopy.sprNo[i]);
    }

}

void Item::setItem(OBJ2D* obj)
{
    obj->renderer()->setSprNo(0);
    obj->transform()->setPos(VECTOR2(0, 0));
    obj->transform()->setScale(VECTOR2(1, 1));
    obj->transform()->setTexPos(VECTOR2(0, 0));
    obj->collider()->setSize(VECTOR2(200, 200));
    obj->transform()->setCenter(VECTOR2(100, 100));
    obj->transform()->setAngle(ToRadian(0));
    obj->renderer()->setColor(VECTOR4(1, 1, 1, 1));
    obj->transform()->setVelocity(VECTOR2(0, 0));
    obj->transform()->setOrgVelocity(VECTOR2(0, 0));
    obj->setIsFlipX(false);

    obj->collider()->setHitSize(VECTOR2(150, 150));
    obj->transform()->setOffset(-obj->collider()->hitSize() / 2);

    obj->renderer()->setAnimeTimer(0);
    obj->renderer()->setAnimeX(0);
    obj->renderer()->setAnimeY(0);

    obj->setElement(0);
    obj->setState(0);
    obj->actorComponent()->setAct(0);

    obj->mover_ = nullptr;
}

void Item::setItem(OBJ2D* arr[])
{
    for (int i = 0; i < itemMax; ++i)
    {
        arr[i]->renderer()->setSprNo(0);
        arr[i]->transform()->setPos(VECTOR2(0, 0));
        arr[i]->transform()->setScale(VECTOR2(1, 1));
        arr[i]->transform()->setTexPos(VECTOR2(0, 0));
        arr[i]->collider()->setSize(VECTOR2(200, 200));
        arr[i]->transform()->setCenter(VECTOR2(100, 100));
        arr[i]->transform()->setAngle(ToRadian(0));
        arr[i]->renderer()->setColor(VECTOR4(1, 1, 1, 1));
        arr[i]->transform()->setVelocity(VECTOR2(0, 0));
        arr[i]->setIsFlipX(false);

        arr[i]->collider()->setHitSize(VECTOR2(150, 150));
        arr[i]->transform()->setOffset(-arr[i]->collider()->hitSize() / 2);

        arr[i]->renderer()->setAnimeTimer(0);
        arr[i]->renderer()->setAnimeX(0);
        arr[i]->renderer()->setAnimeY(0);

        arr[i]->setElement(0);
        arr[i]->setState(0);
        arr[i]->actorComponent()->setAct(0);

        arr[i]->mover_ = nullptr;
    }
}

// ジグザグ
void rabbitMove1(OBJ2D* obj)
{
    switch (obj->state())
    {
    case 0:
        obj->setState(1);
        [[fallthrough]];
    case 1:
        switch (obj->actorComponent()->act())
        {
        case 0:
            obj->transform()->addPosY(-1);

            if (obj->transform()->pos().y < Item::itemMinPosY)
            {
                obj->actorComponent()->setAct(1);
                break;
            }

            break;
        case 1:
            obj->transform()->addPosY(1);

            if (obj->transform()->pos().y > Item::itemMaxPosY)
            {
                obj->actorComponent()->setAct(0);
                break;
            }
            break;
        }
        break;
    }
}

// 不規則
void rabbitMove2(OBJ2D* obj)
{
    switch (obj->state())
    {
    case 0:
        obj->setState(1);
        [[fallthrough]];
    case 1:
        switch (obj->actorComponent()->act())
        {
        case 0:
            obj->transform()->addPosY(4);

            if ((rand() % 50 == 0 && obj->transform()->pos().y > Item::itemMinPosY + 50) 
                || obj->transform()->pos().y > Item::itemMaxPosY)
            {
                obj->actorComponent()->setAct(1);
                break;
            }

            break;
        case 1:
            obj->transform()->addPosY(-4);

            if ((rand() % 50 == 0 && obj->transform()->pos().y < Item::itemMaxPosY - 50)
                || obj->transform()->pos().y < Item::itemMinPosY)
            {
                obj->actorComponent()->setAct(0);
                break;
            }

            break;
        }
        break;
    }
}
