#include "all.h"
using namespace input;
using namespace GameLib;

Clear Clear::instance_;

bool Clear::isRetry_;

Clear::ClearObj Clear::clearLogo_;
Clear::ClearObj Clear::clearTitle_;
Clear::ClearObj Clear::clearRetry_;

Clear::RabbitObj Clear::clearCarpet_[clearCarpetMax];
Clear::RabbitObj Clear::clearRabbit_;


void Clear::init()
{
	Scene::init();
}

void Clear::deinit()
{
	music::setVolume(SE_RABBIT_HOLD, 0.75f);

	texture::release(loadTextureClear);
}

void Clear::update()
{
	music::setVolume(SE_RABBIT_HOLD, 0.3f);

	////dispHitRect_
	//if (TRG(0) & PAD_RCLICK)
	//{
	//	if (!Scene::isDispHitRect_) Scene::isDispHitRect_ = true;
	//	else Scene::isDispHitRect_ = false;
	//}

	switch (state_)
	{
	case 0:
	{
		timer_ = 0;
		isRetry_ = false;

		// size{ 1050, 280 }
		clearLogo_ = { { 600, -140 + -200 }, { 0, 50 } };

		// size{ 450,160 }
		clearTitle_ = { { -225 + -880, 500}, {90,0}, {450, 160}, {}, {1,1} };
		clearTitle_.offset = { -clearTitle_.hitSize / 2 };

		// size{ 450,160 }
		clearRetry_ = { { -225 + -320, 500}, {70,0}, {450,160}, {}, {1,1} };
		clearRetry_.offset = { -clearRetry_.hitSize / 2 };

		{
			for (int i = 0; i < clearCarpetMax; ++i) clearCarpet_[i] = { CLEAR_FIRE, {}, {}, 0, 0 };

			clearCarpet_[0].sprNo = CLEAR_RARE;
			clearCarpet_[0].pos = { 1850, 420 };
			clearCarpet_[0].animeStartTime = 0;

			clearCarpet_[1].sprNo = CLEAR_RARE;
			clearCarpet_[1].pos = { 1600, 520 };
			clearCarpet_[2].pos = { 1850, 620 };
			clearCarpet_[3].pos = { 2100, 720 };
			clearCarpet_[1].animeStartTime = 15;
			clearCarpet_[2].animeStartTime = 15;
			clearCarpet_[3].animeStartTime = 15;

			clearCarpet_[4].sprNo = CLEAR_RARE;
			clearCarpet_[4].pos = { 1350, 620 };
			clearCarpet_[5].pos = { 1600, 720 };
			clearCarpet_[6].pos = { 1850, 820 };
			clearCarpet_[7].pos = { 2100, 920 };
			clearCarpet_[4].animeStartTime = 30;
			clearCarpet_[5].animeStartTime = 30;
			clearCarpet_[6].animeStartTime = 30;
			clearCarpet_[7].animeStartTime = 30;

			clearCarpet_[8].sprNo = CLEAR_RARE;
			clearCarpet_[8].pos   = { 1100, 720 };
			clearCarpet_[9].pos   = { 1350, 820 };
			clearCarpet_[10].pos  = { 1600, 920 };
			clearCarpet_[11].sprNo = CLEAR_RARE;
			clearCarpet_[11].pos  = { 1850, 1020 };
			clearCarpet_[8].animeStartTime  = 45;
			clearCarpet_[9].animeStartTime  = 45;
			clearCarpet_[10].animeStartTime = 45;
			clearCarpet_[11].animeStartTime = 45;

			clearCarpet_[12].sprNo = CLEAR_RARE;
			clearCarpet_[12].pos = { 850,  820 };
			clearCarpet_[13].pos = { 1100, 920 };
			clearCarpet_[14].pos = { 1350, 1020 };
			clearCarpet_[15].sprNo = CLEAR_RARE;
			clearCarpet_[15].pos = { 1600, 1120 };
			clearCarpet_[12].animeStartTime = 45;
			clearCarpet_[13].animeStartTime = 45;
			clearCarpet_[14].animeStartTime = 45;
			clearCarpet_[15].animeStartTime = 45;

			clearCarpet_[16].sprNo = CLEAR_RARE;
			clearCarpet_[16].pos = { 600,  920 };
			clearCarpet_[17].pos = { 850,  1020 };
			clearCarpet_[18].pos = { 1100, 1120 };
			clearCarpet_[16].animeStartTime = 60;
			clearCarpet_[17].animeStartTime = 60;
			clearCarpet_[18].animeStartTime = 60;

			clearCarpet_[19].sprNo = CLEAR_RARE;
			clearCarpet_[19].pos = { 350,  1020 };
			clearCarpet_[20].pos = { 600,  1120 };
			clearCarpet_[19].animeStartTime = 75;
			clearCarpet_[20].animeStartTime = 75;

			clearCarpet_[21].sprNo = CLEAR_RARE;
			clearCarpet_[21].pos = { 100,  1120 };
			clearCarpet_[22].pos = { 350,  1220 };
			clearCarpet_[21].animeStartTime = 90;
			clearCarpet_[22].animeStartTime = 90;
		}

		clearRabbit_.pos = { 1350, 860 };

	}

	Scene::isIrisEnd_ = false; // インが終わる前にstateが進むのを防ぐ
	Scene::setIrisInOut(true);

	GameLib::setBlendMode(Blender::BS_ALPHA);
	texture::load(loadTextureClear);


	++state_;
	[[fallthrough]];
	case 1: // IrisIn
		if (Scene::isIrisEnd_)
		{
			music::play(AUDIO::BGM_CLEAR, false);

			++state_;
			break;
		}

		break;
	case 2:	// moveIn
	{
		bool isMoved[3] = {};
		if (timer_ >= 0)  if (moveIn(&clearLogo_.pos, { 600, 200 },  &clearLogo_.velocity, { 0, -0.5f }))   isMoved[0] = true;
		if (timer_ >= 20) if (moveIn(&clearTitle_.pos, { 880, 500 }, &clearTitle_.velocity, { -2.08f, 0 })) isMoved[1] = true;
		if (timer_ >= 40) if (moveIn(&clearRetry_.pos, { 320, 500 }, &clearRetry_.velocity, { -2.8f, 0 }))   isMoved[2] = true;

		if (isMoved[0] && isMoved[1] && isMoved[2])
		{
			++state_;
			break;
		}
	}
		++timer_;
		break;
	case 3:	// 通常
		Judge::clearJudge();

		++timer_;
		break;
	case 4: // IrisOut
		music::fade(BGM_CLEAR, 0.5f, 0.0f);

		if (Scene::isIrisEnd_)
		{
			if (isRetry_)
			{
				++Title::skyColorNum_;
				if (Title::skyColorNum_ > 2) Title::skyColorNum_ = 0;

				Clear::instance()->changeScene(Game::instance());
				return;
			}
			else
			{
				Clear::instance()->changeScene(Title::instance());
				return;
			}
		}

		break;
	}

}

void Clear::draw()
{
	GameLib::clear(VECTOR4(0, 0, 0, 0));

	// Back
	texture::begin(CLEAR_BACK);
	texture::draw(
		CLEAR_BACK, { 0,0 }, { 1,1 },
		{ 0,0 }, { 1920,1080 }, { 0,0 },
		ToRadian(0), { 1,1,1,1 }
	);
	texture::end(CLEAR_BACK);


	for (int i = 0; i < clearCarpetMax; ++i)
	{
		if( state_ > 1 && timer_ >= clearCarpet_[i].animeStartTime) ++clearCarpet_[i].animeTimer;
		clearCarpet_[i].anime.x = clearCarpet_[i].animeTimer / 10;

		if (clearCarpet_[i].animeStartTime == 45)
		{
			if (clearCarpet_[i].anime.x == 1) clearRabbit_.pos.y = 600;
			if (clearCarpet_[i].anime.x == 2) clearRabbit_.pos.y = 550;

			clearRabbit_.anime.x = (clearCarpet_[i].anime.x - 1 >= 0)
								 ? clearCarpet_[i].anime.x - 1
								 : 0;
		}

		if (clearCarpet_[i].anime.x >= 4)
		{
			clearCarpet_[i].anime.x = 4;
			if (clearCarpet_[i].animeTimer >= 2 * 60)
			{
				music::play(SE_RABBIT_HOLD, false);
				clearCarpet_[i].animeTimer = 0;
				clearCarpet_[i].anime.x    = 0;
			}
		}

		texture::begin(clearCarpet_[i].sprNo);
		texture::draw(
			clearCarpet_[i].sprNo,
			clearCarpet_[i].pos, { 0.75f,0.75f },
			{ 640 * clearCarpet_[i].anime.x, 0 }, { 640,880 }, { 320, 880 - 220 },
			ToRadian(0), { 1,1,1,1 }
		);
		texture::end(clearCarpet_[i].sprNo);
	}


	// Logo
	texture::begin(CLEAR_LOGO);
	texture::draw(
		CLEAR_LOGO, clearLogo_.pos, { 1,1 },
		{ 0,0 }, { 1140,380 }, { 570,190 },
		ToRadian(0), { 1,1,1,1 }
	);
	texture::end(CLEAR_LOGO);


	// Title
	texture::begin(CLEAR_TITLE);
	texture::draw(
		CLEAR_TITLE, clearTitle_.pos, clearTitle_.scale,
		{ 0,0 }, { 450,160 }, { 225,80 },
		ToRadian(0), { 1,1,1,1 }
	);
	texture::end(CLEAR_TITLE);

	// Retry
	texture::begin(CLEAR_RETRY);
	texture::draw(
		CLEAR_RETRY, clearRetry_.pos, clearRetry_.scale,
		{ 0,0 }, { 450,160 }, { 225,80 },
		ToRadian(0), { 1,1,1,1 }
	);
	texture::end(CLEAR_RETRY);


	// update rabbit animation


	const float orgVeloY = 5;
	static float velocityY = orgVeloY;

	if (clearRabbit_.anime.x < 3) velocityY = orgVeloY;
	if (clearRabbit_.anime.x >= 3)
	{
		if (clearRabbit_.pos.y < 860)
		{
			velocityY += 1.5f;
			clearRabbit_.pos.y += velocityY;
			if (clearRabbit_.pos.y > 860) clearRabbit_.pos.y = 860;
		}
	}
	if (clearRabbit_.anime.x > 4) clearRabbit_.anime.x = 0;

	// rabbit
	texture::begin(CLEAR_RABBIT);
	texture::draw(
		CLEAR_RABBIT, clearRabbit_.pos, { 0.75f,0.75f },
		{ 960 * clearRabbit_.anime.x, 0 }, { 960,1120 }, { 480, 1120 - 280 },
		ToRadian(0), { 1,1,1,1 }
	);
	texture::end(CLEAR_RABBIT);


	// rect
	if (Scene::isDispHitRect_)
	{
		primitive::rect(
			clearTitle_.pos + clearTitle_.offset,
			clearTitle_.hitSize,
			{ 0,0 }, ToRadian(0),
			{ 1,0,0,0.2f }
		);
		primitive::rect(
			clearRetry_.pos + clearRetry_.offset,
			clearRetry_.hitSize,
			{ 0,0 }, ToRadian(0),
			{ 1,0,0,0.2f }
		);
	}

}
