#include "all.h"
#include "imgui_manager.h"

#include "Font/directX11Render2D.h"
#include "Font/fontTexture.h"
#include "Font/dispString.h"
#include <tchar.h>

using namespace input;
using namespace GameLib;


Title Title::instance_;

static const float circle_centerPosY[] = {
		400,
		300
};

int Title::skyColorNum_ = 0;

int Title::changeSceneNum_ = 0;

float Title::titleCloudAlpha_ = 0.0f;
float Title::titleCloudAddAlpha_ = 0.0f;

Title::TitleObj Title::titleRabbitNormal_ = {};
Title::TitleObj Title::titleRabbitFire_ = {};
Title::TitleObj Title::titleRabbitWind_ = {};

Title::TitleObj Title::titleLogo_ = {};
Title::TitleObj Title::titleStart_ = {};
Title::TitleObj Title::titleRule_ = {};
Title::TitleObj Title::titleEnd_ = {};

VECTOR2 corePos = {BG::WINDOW_W_HALF, BG::WINDOW_H_HALF};
VECTOR2 subPos[5] = {
	{corePos.x + 100, corePos.y },
	{corePos.x + 200, corePos.y + 50 },
	{corePos.x - 50, corePos.y - 100 },
	{corePos.x - 100, corePos.y + 50 },
	{corePos.x - 200, corePos.y - 50 },
};
VECTOR2 size{ 100,100 };

bool miniJudge(VECTOR2 pos)
{
	for (auto& sPos : subPos)
	{
		if (pos == sPos) continue;
		if (Judge::rectHitCheck(
			pos  - VECTOR2(size * 0.5f), size,
			sPos - VECTOR2(size * 0.5f), size))
		{
			return true;
		}
	}

	if (Judge::rectHitCheck(
		corePos - VECTOR2(size * 0.5f), size,
		pos - VECTOR2(size * 0.5f), size))
	{
		return true;
	}


	return false;
}

void Title::init()
{
	Scene::init();
}

void Title::deinit()
{
	texture::release(loadTextureTitle);
}

void Title::update() 
{
	using namespace input;

	switch (state_)
	{
	case 0:
	{
		timer_ = 0;

		changeSceneNum_ = 0;

		titleCloudAlpha_ = 0.95f;
		titleCloudAddAlpha_ = 0.005f;

		titleRabbitNormal_ = { {550,750}, {0, 0}, {670,400}, {}, {1,1} };
		titleRabbitNormal_.offset = { -360, -170 };

		titleRabbitFire_ = { {1700,400}, {0, -0.4f} };
		titleRabbitWind_ = { {1200,300}, {0,  1.5f} };

		titleLogo_ = { {   0 - 450, 250}, { 60,0} };

		titleStart_ = { {1920 + 325, 550}, {-40,0}, {340,120}, {}, {1,1} };
		titleStart_.offset = { -325, -titleStart_.hitSize.y / 2 };

		titleRule_ = { {1920 + 325, 750}, {-40,0}, {320,120}, {}, {1,1} };
		titleRule_.offset = { -330, -titleRule_.hitSize.y / 2 };

		titleEnd_ = { {1920 + 325, 950}, {-40,0}, {300,120}, {}, {1,1} };
		titleEnd_.offset = { -320,-titleEnd_.hitSize.y / 2 };
	}


		Scene::isIrisEnd_ = false; // インが終わる前にstateが進むのを防ぐ
		Scene::setIrisInOut(true);

		GameLib::setBlendMode(Blender::BS_ALPHA);
		texture::load(loadTextureTitle);

		//++state_;
		state_ = 3;
		[[fallthrough]];
	//case 1: // IrisIn
	//	if (Scene::isIrisEnd_)
	//	{
	//		music::play(AUDIO::BGM_TITLE, true);

	//		++state_;
	//		break;
	//	}

	//	break;
	//case 2:	// moveIn
	//{
	//	bool isMoved[4] = {};
	//	if (timer_ >= 0)  if (moveIn(&titleLogo_.pos,  {  525, 250 }, &titleLogo_.velocity,  { -1.911f, 0 })) isMoved[0] = true;
	//	if (timer_ >= 20) if (moveIn(&titleStart_.pos, { 1700, 550 }, &titleStart_.velocity, {  1.54f, 0 })) isMoved[1] = true;
	//	if (timer_ >= 30) if (moveIn(&titleRule_.pos,  { 1700, 750 }, &titleRule_.velocity,  {  1.53f, 0 })) isMoved[2] = true;
	//	if (timer_ >= 40) if (moveIn(&titleEnd_.pos,   { 1700, 950 }, &titleEnd_.velocity,   {  1.54f, 0 })) isMoved[3] = true;

	//	if (isMoved[0] && isMoved[1] && isMoved[2] && isMoved[3])
	//	{
	//		timer_ = 0;
	//		++state_;
	//		break;
	//	}
	//}
	//	++timer_;
	//	break;
	case 3:	// 通常

		for (auto& sPos : subPos)
		{
			float dx = corePos.x - sPos.x;
			float dy = corePos.y - sPos.y;
			float dist = sqrtf(dx * dx + dy * dy);

			if (STATE(0) & PAD_START)
			{
				const VECTOR2 minSize = { 25,25 };

				if (size.x > minSize.x)
				{
					size *= 0.995f;
					if (size.x < minSize.x) size = minSize;
				}

				if (miniJudge(sPos) == false)
				{
					float value = (dist > 100) ? 4.0f : 3.0f;
					sPos += VECTOR2(dx / dist * value, dy / dist * value);
				}

			}
			debug::setString("dist:%f", dist);

			if (STATE(0) & PAD_UP)    sPos.y -= 10;
			if (STATE(0) & PAD_DOWN)  sPos.y += 10;
			if (STATE(0) & PAD_LEFT)  sPos.x -= 10;
			if (STATE(0) & PAD_RIGHT) sPos.x += 10;
		}

		if (STATE(0) & PAD_UP)    corePos.y -= 10;
		if (STATE(0) & PAD_DOWN)  corePos.y += 10;
		if (STATE(0) & PAD_LEFT)  corePos.x -= 10;
		if (STATE(0) & PAD_RIGHT) corePos.x += 10;

		Judge::titleJudge();



		++timer_;
		break;
	case 4: // IrisOut
		music::fade(BGM_TITLE, 0.5f, 0.0f);

		if (Scene::isIrisEnd_)
		{
			if (changeSceneNum_ == 0) Title::instance()->changeScene(Game::instance());
			if (changeSceneNum_ == 1) Title::instance()->changeScene(Tutorial::instance());
			return;
		}	

		break;
	case 5: // RabbitNormalFall 1
		Judge::titleJudge();
		titleRabbitNormal_.pos.y += titleRabbitNormal_.velocity.y;
		titleRabbitNormal_.velocity.y += 0.1f;

		if (titleRabbitNormal_.pos.y > 1920 + 300)
		{
			titleRabbitNormal_.pos.y = -300;
			++state_;
			break;
		}

		break;
	case 6: // RabbitNormallFall 2
		Judge::titleJudge();
		titleRabbitNormal_.pos.y += titleRabbitNormal_.velocity.y;
		titleRabbitNormal_.velocity.y -= 0.142f;

		if (titleRabbitNormal_.pos.y > 750)
		{
			titleRabbitNormal_.pos.y = 750;
			titleRabbitNormal_.velocity.y = 0;
			state_ = 2;
			break;
		}

		break;
	}


	// update cloud
	if (titleCloudAlpha_ < 0.3f || titleCloudAlpha_ > 0.95f) titleCloudAddAlpha_ = -titleCloudAddAlpha_;
	titleCloudAlpha_ += titleCloudAddAlpha_;

	// update rabbit
	titleRabbitFire_.velocity.y += (circle_centerPosY[0] - titleRabbitFire_.pos.y) / 256;
	titleRabbitFire_.pos += titleRabbitFire_.velocity;
	titleRabbitWind_.velocity.y += (circle_centerPosY[1] - titleRabbitWind_.pos.y) / 256;
	titleRabbitWind_.pos += titleRabbitWind_.velocity;
}


void Title::draw()
{
	GameLib::clear(VECTOR4(0.1f, 0.1f, 0.1f, 1));

	primitive::rect(
		corePos, size, 
		{ size * 0.5f }, 0,
		{ 0,0,1,1 }
	);

	for (auto& sPos : subPos)
	{
		primitive::rect(
			sPos, size,
			{ size * 0.5f }, 0,
			{ 1,0,0,1 }
		);
	}



		//primitive::rect(
		//	titleEnd_.pos + titleEnd_.offset,
		//	titleEnd_.hitSize,
		//	{ 0,0 }, ToRadian(0),
		//	{ 1,0,0,0.2f }
		//);
 
	// "Push Start Button" 点滅
	//if (timer_ / 40 % 2)
}