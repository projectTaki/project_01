#include "all.h"

int BG::wallSprNo_   = 0;
VECTOR2 BG::wallPos_ = {};

int BG::bgQuakeNum_	  = 0;
int BG::bgQuakeState_ = 0;
int BG::bgQuakeAct_	  = 0;
VECTOR2 BG::bgQuakePos_ = {};
bool BG::isBgQuake_	  = false;

int BG::wallQuakeState_ = 0;
int BG::wallQuakeAct_	= 0;
int BG::wallQuakeNum_	= 0;
VECTOR2 BG::wallQuakePos_ = {};
bool BG::isWallQuake_	= false;

void BG::init() 
{
	state_ = 0;
	timer_ = 0;
}

void BG::deinit()
{
}

void BG::update()
{
	switch (state_) 
	{
	case 0:
		timer_ = 0;

		wallSprNo_	 = WALL_1;
		wallPos_	 = {};

		isBgQuake_   = false;
		bgQuakeNum_  = NONE_QUAKE;
		isWallQuake_ = false;

		++state_;
		/*fallthrough*/
		[[fallthrough]];
	case 1:
		     if(Game::gameHp() >= Game::gameHpMax_ * 0.75f) wallSprNo_ = WALL_1;
		else if(Game::gameHp() >= Game::gameHpMax_ * 0.5f)  wallSprNo_ = WALL_2;
		else if(Game::gameHp() >= Game::gameHpMax_ * 0.25f) wallSprNo_ = WALL_3;
		else												wallSprNo_ = WALL_4;


		// wall collapse
		if (Game::gameHp() <= 0) wallPos_.y += 3;
		if (wallPos_.y >= 600)
		{
			Game::instance()->setTimer(0);
			Scene::setIrisInOut(false);
			Game::instance()->setState(3);
			break;
		}

		++timer_;
		break;
	}

}

void BG::render()
{
	texture::begin(GAME_BACK);

	// ‰æ–Ê—h‚ê‚ÌŽž‚É‰E‚ÉŒ©‚¦‚Ä‚µ‚Ü‚¤‰æ–ÊŠO‚ð‰B‚·
	texture::draw(
		GAME_BACK, VECTOR2(70, 0) + bgQuakePos_, { 1,1 },
		{ 0,0 }, { BG::WINDOW_W, BG::WINDOW_H },
		{ 0,0 }, ToRadian(0), { 1,1,1,1 }
	);
	texture::draw(
		GAME_BACK, bgQuakePos_, { 1,1 },
		{ 0,0 }, { BG::WINDOW_W, BG::WINDOW_H },
		{ 0,0 }, ToRadian(0), { 1,1,1,1 }
	);	
	texture::end(GAME_BACK);


	VECTOR4 wallColor = { 1,1,1,1 };
	if (Game::gameHp() > 0 )
	{
		if (Game::gameHp() == 1 && timer_ & 0x08) wallColor = { 1.f, 0.55f, 0.55f, 1 };
		if (Game::gameHp() == 2 && timer_ & 0x20) wallColor = { 1.f, 0.7f, 0.7f, 1 };

		if (isWallQuake_ && Game::gameHp() > 0) wallColor = { 1.f, 0.2f, 0.2f, 1 };
	}

	texture::begin(wallSprNo_);
	// ‰æ–Ê—h‚ê‚ÌŽž‚É‰E‚ÉŒ©‚¦‚Ä‚µ‚Ü‚¤‰æ–ÊŠO‚ð‰B‚·
	texture::draw(
		wallSprNo_, VECTOR2(1900, 0) +
		wallPos_ + bgQuakePos_ + wallQuakePos_,
		{ 1,1 },
		{ 0,0 }, { BG::WINDOW_W, BG::WINDOW_H },
		{ 0,0 }, ToRadian(0), wallColor
	);
	texture::draw(
		wallSprNo_, { wallPos_ + bgQuakePos_ + wallQuakePos_ },
		{ 1,1 },
		{ 0,0 }, { BG::WINDOW_W, BG::WINDOW_H },
		{ 0,0 }, ToRadian(0), wallColor
	);	
	texture::end(wallSprNo_);
}

void BG::wallQuake()
{
	static int loop = 0;
	static VECTOR2 addPos = {};

	switch (wallQuakeState_)
	{
	case 0:
		wallQuakeAct_ = 0;
		wallQuakePos_ = {};
		loop   = 0;
		addPos = { 0,20 };

		++wallQuakeState_;
		break;
	case 1:
		switch (wallQuakeAct_)
		{
		case 0:
			wallQuakePos_.y += addPos.y;
			++wallQuakeAct_;
			break;
		case 1:
			wallQuakePos_.y += addPos.y;
			++wallQuakeAct_;
			break;
		case 2:
			wallQuakePos_.y += addPos.y;
			++wallQuakeAct_;
			break;
		case 3:
			wallQuakePos_.y -= addPos.y;
			++wallQuakeAct_;
			break;
		case 4:
			wallQuakePos_.y -= addPos.y;
			++wallQuakeAct_;
			break;
		case 5:
			wallQuakePos_.y -= addPos.y;

			if (loop < 7)
			{
				++loop;
				addPos.y *= 0.675f;
				wallQuakeAct_ = 0;
				break;
			}
			else ++wallQuakeAct_;
			break;
		case 6:
			wallQuakeState_ = 0;
			isWallQuake_ = false;
			break;
		}
	}
}

void BG::bgQuake()
{
	static int loop = 0;

	switch (bgQuakeState_)
	{
	case 0:
		bgQuakeAct_ = 0;
		bgQuakePos_ = {};
		loop = 0;
		++bgQuakeState_;
		break;
	case 1:

		switch (bgQuakeNum_)
		{
		case SHOT_QUAKE:
			switch (bgQuakeAct_)
			{
			case 0:
				bgQuakePos_.x -= 30;
				bgQuakePos_.y -= 10;

				++bgQuakeAct_;
				break;
			case 1:
				bgQuakePos_.x += 30;
				++bgQuakeAct_;
				break;
			case 2:
				bgQuakePos_.y += 10;

				if (loop < 1)
				{
					++loop;
					bgQuakeAct_ = 0;
					break;
				}
				++bgQuakeAct_;
				break;
			case 3:
				bgQuakeState_ = 0;
				isBgQuake_ = false;
				bgQuakeNum_ = NONE_QUAKE;
				break;
			}
			break;
		case BOSS_QUAKE:
			switch (bgQuakeAct_)
			{
			case 0:
				bgQuakePos_.x -= 20;
				++bgQuakeAct_;
				break;
			case 1:
				bgQuakePos_.x -= 20;
				++bgQuakeAct_;
				break;			
			case 2:
				bgQuakePos_.x -= 20;
				++bgQuakeAct_;
				break;
			case 3:
				bgQuakePos_.x += 20;
				++bgQuakeAct_;
				break;			
			case 4:
				bgQuakePos_.x += 20;
				++bgQuakeAct_;
				break;
			case 5:
				bgQuakePos_.x += 20;

				if (loop < 23)
				{
					++loop;
					bgQuakeAct_ = 0;
					break;
				}
				else ++bgQuakeAct_;				
				break;
			case 6:
				bgQuakeState_ = 0;
				isBgQuake_ = false;
				bgQuakeNum_ = NONE_QUAKE;
				break;
			}
			break;

		case BOSS_DIED_QUAKE:
			switch (bgQuakeAct_)
			{
			case 0:
				bgQuakePos_.x -= 5;
				++bgQuakeAct_;
				break;
			case 1:
				bgQuakePos_.x -= 5;
				++bgQuakeAct_;
				break;
			case 2:
				bgQuakePos_.x -= 5;
				++bgQuakeAct_;
				break;
			case 3:
				bgQuakePos_.x += 5;
				++bgQuakeAct_;
				break;
			case 4:
				bgQuakePos_.x += 5;
				++bgQuakeAct_;
				break;
			case 5:
				bgQuakePos_.x += 5;

				if (loop < 110)
				{
					++loop;
					bgQuakeAct_ = 0;
					break;
				}
				else ++bgQuakeAct_;
				break;
			case 6:
				bgQuakeState_ = 0;
				isBgQuake_ = false;
				bgQuakeNum_ = NONE_QUAKE;
				break;
			}
			break;
		}

		break;
	}
}