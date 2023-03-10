#include "all.h"
using namespace input;
using namespace GameLib;

Over Over::instance_;

bool Over::isRetry_;

Over::OverObj Over::overLogo_;
Over::OverObj Over::overTitle_;
Over::OverObj Over::overRetry_;


void Over::init()
{
    Scene::init();
}

void Over::deinit()
{
	texture::release(loadTextureOver);
}

void Over::update()
{
	// //dispHitRect_
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
		overLogo_ = { { 600, -140 + -200 }, { 0, 50 } };

		// size{ 450,160 }
		overTitle_ = { { -225 + -880, 500}, {90,0}, {450, 160}, {}, {1,1} };
		overTitle_.offset = { -overTitle_.hitSize / 2 };

		// size{ 450,160 }
		overRetry_ = { { -225 + -320, 500}, {70,0}, {450,160}, {}, {1,1} };
		overRetry_.offset = { -overRetry_.hitSize / 2 };
	}

	Scene::isIrisEnd_ = false; // インが終わる前にstateが進むのを防ぐ
	Scene::setIrisInOut(true);

	GameLib::setBlendMode(Blender::BS_ALPHA);
	texture::load(loadTextureOver);

	++state_;
	[[fallthrough]];
	case 1: // IrisIn
		if (Scene::isIrisEnd_)
		{
			music::play(AUDIO::BGM_OVER, false);
			++state_;
			break;
		}

		break;
	case 2:	// moveIn
	{
		bool isMoved[3] = {};
		if (timer_ >= 0)  if (moveIn(&overLogo_.pos,  { 600, 200 }, &overLogo_.velocity,  { 0, -0.5f }))   isMoved[0] = true;
		if (timer_ >= 20) if (moveIn(&overTitle_.pos, { 880, 500 }, &overTitle_.velocity, { -2.08f, 0 })) isMoved[1] = true;
		if (timer_ >= 40) if (moveIn(&overRetry_.pos, { 320, 500 }, &overRetry_.velocity, { -2.8f, 0 }))   isMoved[2] = true;

		if (isMoved[0] && isMoved[1] && isMoved[2])
		{
			timer_ = 0;
			++state_;
			break;
		}
	}
		++timer_;
		break;
	case 3:	// 通常
		Judge::overJudge();

		++timer_;
		break;
	case 4: // IrisOut
		music::fade(BGM_OVER, 0.5f, 0.0f);

		if (Scene::isIrisEnd_)
		{
			if (isRetry_)
			{
				++Title::skyColorNum_;
				if (Title::skyColorNum_ > 2) Title::skyColorNum_ = 0;

				Over::instance()->changeScene(Game::instance());
				return;
			}
			else
			{
				Over::instance()->changeScene(Title::instance());
				return;
			}
		}

		break;
	}
}

void Over::draw()
{
	GameLib::clear(VECTOR4(0, 0, 0, 0));

	// Back
	texture::begin(OVER_BACK);
	texture::draw(
		OVER_BACK, { 0,0 }, { 1,1 },
		{ 0,0 }, { 1920,1080 }, { 0,0 },
		ToRadian(0), { 1,1,1,1 }
	);
	texture::end(OVER_BACK);


	// Logo
	texture::begin(OVER_LOGO);
	texture::draw(
		OVER_LOGO, overLogo_.pos, { 1,1 },
		{ 0,0 }, { 1050,280 }, { 525,140 },
		ToRadian(0), { 1,1,1,1 }
	);
	texture::end(OVER_LOGO);


	// Title
	texture::begin(OVER_TITLE);
	texture::draw(
		OVER_TITLE, overTitle_.pos, overTitle_.scale ,
		{ 0,0 }, { 450,160 }, { 225,80 },
		ToRadian(0), { 1,1,1,1 }
	);
	texture::end(OVER_TITLE);

	// Retry
	texture::begin(OVER_RETRY);
	texture::draw(
		OVER_RETRY, overRetry_.pos, overRetry_.scale,
		{ 0,0 }, { 450,160 }, { 225,80 },
		ToRadian(0), { 1,1,1,1 }
	);
	texture::end(OVER_RETRY);


	// rect
	if (Scene::isDispHitRect_)
	{
		primitive::rect(
			overTitle_.pos + overTitle_.offset,
			overTitle_.hitSize,
			{ 0,0 }, ToRadian(0),
			{ 1,0,0,0.2f }
		);		
		primitive::rect(
			overRetry_.pos + overRetry_.offset,
			overRetry_.hitSize,
			{ 0,0 }, ToRadian(0),
			{ 1,0,0,0.2f }
		);
	}

}
