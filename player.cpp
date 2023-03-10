#include "all.h"

//VECTOR4 colors

//VECTOR4 colors[12] = {
//	{   1,    0,    0,    1},
//	{   1, 0.5f,    0,    1},
//	{   1,    1,    0,    1},
//	{0.5f,    1,    0,    1},
//	{	0, 	  1,	0,    1},
//	{   0,    1,    0.5f, 1},
//	{	0,    1,    1,    1},
//	{	0, 	  0.5f, 1,    1},
//	{   0,    0,    1,	  1},
//	{0.5f,    0,    1,    1},
//	{	1, 	  0,    1,    1},
//	{   1,    0, 0.5f,    1}
//};

OBJ2D* Player::players_[plMax];

void Player::init()
{
	state_ = 0;
	timer_ = 0;

	for (auto& player : players_)
	{
		player = new OBJ2D(
			new Transform,
			new Renderer,
			new Collider,
			new ActorComponent
		);
	}
}

void Player::deinit()
{
	for (auto& player : players_) safe_delete(player);
}

void Player::update()
{
	int i;

	switch (state_)
	{
	case 0:
		timer_ = 0;

		setPlayer();

		for (i = 0; i < plMax; ++i)
		{
			VECTOR2 pos = { (458 * float(i + 1)) - 185, 925 };
			OBJ2D::searchSet(players_[i], PLAYER, pos, false);

			cannonRabbit[i].isDisp = true;
			cannonEfc[i].isDisp = false;
		}

		++state_;
		//fallthrough//
		[[fallthrough]];
	case 1:

		++state_;
		//fallthrough//
		[[fallthrough]];
	case 2:

		if (Game::gameHp() <= 0)
		{
			BG::setBgQuakeNum(BOSS_QUAKE);
			BG::setBgQuakeState(0);
			BG::setIsBgQuake(true);
			++state_;
			break;
		}

		for (i = 0; i < plMax; ++i)
		{
			if (!players_[i]->exist()) continue;

			switch (players_[i]->renderer()->sprNo())
			{
			case CANNON:
				cannonRabbit[i].animeX = cannonRabbit->animeTimer / 6 % 5;
				break;
			case CANNON_SHOT_NORMAL:
			case CANNON_SHOT_FIRE:
			case CANNON_SHOT_WATER:
			case CANNON_SHOT_WIND:
			case CANNON_SHOT_RARE:
				switch (players_[i]->actorComponent()->act())
				{
				case 0:
					cannonRabbit[i].animeTimer = 0;
					cannonRabbit[i].animeX = 0;
					cannonRabbit[i].animeY = 1;

					players_[i]->actorComponent()->setAct(1);
					break;
				case 1: // うさぎ着火
					
					if (cannonRabbit[i].animeX >= 2)
					{
						cannonRabbit[i].animeX = 2;

						players_[i]->renderer()->setAnimeTimer(0);
						players_[i]->renderer()->setAnimeX(0);

						cannonRabbit[i].animeTimer = 0;
						cannonRabbit[i].animeX = 0;
						cannonRabbit[i].animeY = 0;

						players_[i]->actorComponent()->setAct(2);
						break;
					}

					cannonRabbit[i].animeX = cannonRabbit[i].animeTimer / 8;
					break;
				case 2: // 大砲発射

					for (auto& item : Item::items_)
					{
						if (players_[i]->renderer()->animeX() >= 3)
						{
							players_[i]->renderer()->setAnimeX(3);

							if (item->exist()) continue;

							Item::setItem(item);
							int rabbitsSpr[] = {
								RABBIT_SHOT_NORMAL,
								RABBIT_SHOT_FIRE,
								RABBIT_SHOT_WATER,
								RABBIT_SHOT_WIND,
								RABBIT_SHOT_RARE,
							};
							item->renderer()->setSprNo(rabbitsSpr[players_[i]->element()]);
							item->setElement(players_[i]->element());

							OBJ2D::searchSet(
								item, SHOT,
								{ players_[i]->transform()->pos().x + players_[i]->transform()->offset().x + 80,
								  players_[i]->transform()->pos().y + players_[i]->transform()->offset().y },
								false
							);
							players_[i]->setIsInRabbit(false);
							music::play(SE_CANNON, false);

							players_[i]->renderer()->setAnimeTimer(0);
							players_[i]->renderer()->setAnimeX(0);

							players_[i]->actorComponent()->setAct(0);
							players_[i]->renderer()->setSprNo(CANNON);

							cannonEfc[i].isDisp = true;

							if (BG::bgQuakeNum() == NONE_QUAKE)
							{
								BG::setBgQuakeNum(SHOT_QUAKE);
								if (!BG::isBgQuake())
								{
									BG::setBgQuakeState(0);
									BG::setIsBgQuake(true);
								}
							}
							break;
						}
					}

					players_[i]->renderer()->setAnimeX(players_[i]->renderer()->animeTimer() / 8);
					cannonRabbit[i].animeX = cannonRabbit[i].animeTimer / 6 % 5;
					break;
				}
			}

			cannonEfc[i].animeX = cannonEfc[i].animeTimer / 8;
			if (cannonEfc[i].animeX >= 6)
			{
				cannonEfc[i].animeX = 6;

				cannonEfc[i].isDisp = false;
				cannonEfc[i].animeTimer = 0;
				cannonEfc[i].animeX = 0;
			}

			players_[i]->transform()->setTexPos(VECTOR2(players_[i]->renderer()->animeX() * players_[i]->collider()->size().x, 0));
			cannonRabbit[i].texPos = { static_cast<float>(cannonRabbit[i].animeX) * 400, 
									   static_cast<float>(cannonRabbit[i].animeY) * 200 };
			cannonEfc[i].texPos = { static_cast<float>(cannonEfc[i].animeX) * 270, 
									static_cast<float>(cannonEfc[i].animeY) * 250 };

			players_[i]->renderer()->addAnimeTimer(1);
			++cannonRabbit[i].animeTimer;
			if(cannonEfc[i].isDisp) ++cannonEfc[i].animeTimer;
		}

		++timer_;
		break;
	case 3:
		// collapse wall
		for (i = 0; i < plMax; ++i)
		{
			players_[i]->transform()->addVelocityY(0.25f);
			players_[i]->transform()->addPos(players_[i]->transform()->velocity());


			// エフェクトだけ最後までアニメーションを動かしきる
			if (cannonEfc[i].animeX >= 6)
			{
				cannonEfc[i].isDisp = false;
			}
			else
			{
				if (cannonEfc[i].isDisp) ++cannonEfc[i].animeTimer;
				cannonEfc[i].animeX = cannonEfc[i].animeTimer / 8;
				cannonEfc[i].texPos = { static_cast<float>(cannonEfc[i].animeX) * 270,
							static_cast<float>(cannonEfc[i].animeY) * 250 };
			}
		}

		++timer_;
		break;
	}
}

void Player::render()
{
	for (int i = 0; i < plMax; ++i)
	{
		if (!players_[i]->exist()) continue;

		VECTOR4 plColor = { 1,1,1,1 };
		if (Game::gameHp() > 0)
		{
			if (Game::gameHp() == 1 && timer_ & 0x08) plColor = { 1.f, 0.8f, 0.8f, 1 };
			if (Game::gameHp() == 2 && timer_ & 0x20) plColor = { 1.f, 0.9f, 0.9f, 1 };

			if (BG::isWallQuake() && Game::gameHp() > 0) plColor = { 1, 0.5f, 0.5f, 1 };
		}

		texture::begin(players_[i]->renderer()->sprNo());
		texture::draw(
			players_[i]->renderer()->sprNo(),
			players_[i]->transform()->pos() + BG::bgQuakePos() + BG::wallQuakePos(),
			players_[i]->transform()->scale(),
			players_[i]->transform()->texPos(),
			players_[i]->collider()->size(),
			players_[i]->transform()->center(),
			players_[i]->transform()->angle(),
			plColor
		);
		texture::end(players_[i]->renderer()->sprNo());

		texture::begin(CANNON_RABBIT);
		texture::draw(
			CANNON_RABBIT,
			{ players_[i]->transform()->pos().x + players_[i]->transform()->offset().x - 170 + BG::bgQuakePos().x + BG::wallQuakePos().x,
			  players_[i]->transform()->pos().y + players_[i]->transform()->offset().y + 30 + BG::bgQuakePos().y + BG::wallQuakePos().y },
			{ 1,1 },
			cannonRabbit[i].texPos,
			{ 400, 200 },
			{ 0,0 },
			0,
			plColor
		);
		texture::end(CANNON_RABBIT);

		if (cannonEfc[i].isDisp)
		{
			texture::begin(CANNON_EFC);
			texture::draw(
				CANNON_EFC,
				players_[i]->transform()->pos() + players_[i]->transform()->offset() 
				+ VECTOR2(95, 50) + BG::bgQuakePos(),
				{ 1,1 },
				cannonEfc[i].texPos,
				{ 270, 250 },
				{ 135, 125 },
				0,
				{ 1,1,1,1 }
			);
			texture::end(CANNON_EFC);
		}

		if (Scene::isDispHitRect_)
		{
			primitive::rect(
				players_[i]->transform()->pos() + players_[i]->transform()->offset(),
				players_[i]->collider()->hitSize(),
				{ 0,0 }, 0, { 1,0,1,0.5f }
			);
		}

	}
}

void Player::setPlayer()
{
	for (auto& player : players_)
	{
		player->renderer()->setSprNo(CANNON);
		player->transform()->setPos(VECTOR2(0, 0));
		player->transform()->setScale(VECTOR2(1, 1));
		player->transform()->setTexPos(VECTOR2(0, 0));
		player->collider()->setSize(VECTOR2(270, 250));
		player->transform()->setCenter(VECTOR2(135, 125));
		player->transform()->setAngle(ToRadian(0));
		player->renderer()->setColor(VECTOR4(1, 1, 1, 1));
		player->transform()->setVelocity(VECTOR2(0, -6.5f));

		player->collider()->setHitSize(VECTOR2(200, 200));
		player->transform()->setOffset(-player->collider()->hitSize() / 2);

		player->setState(0);
		player->actorComponent()->setAct(0);

	}
}