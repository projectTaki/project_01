#include "all.h"

Tutorial Tutorial::instance_;

int Tutorial::pageNum_;
int Tutorial::ruleSprNo_;

void Tutorial::init()
{
	Scene::init();
}

void Tutorial::deinit()
{
	texture::release(loadTextureTutorial);
}

void Tutorial::update()
{
	using namespace GameLib;
	using namespace input;

	switch (state_)
	{
	case TUTORIAL_STATE::INIT:
		//////// 初期設定 ////////
		timer_ = 0;

		pageNum_ = 0;
		ruleSprNo_ = RULE_1;

		Scene::isIrisEnd_ = false; // インが終わる前にstateが進むのを防ぐ
		Scene::setIrisInOut(true);


		GameLib::setBlendMode(Blender::BS_ALPHA);   // 通常のアルファ処理
		texture::load(loadTextureTutorial);

		music::play(BGM_TUTORIAL, true);

		++state_;
		[[fallthrough]];
	case TUTORIAL_STATE::IRIS_IN: // IrisIn

		if (Scene::isIrisEnd_)
		{
			state_ = TUTORIAL_STATE::UPDATE; // wave moveInOut
			break;
		}

		break;
	case TUTORIAL_STATE::UPDATE:

		// ソフトリセット
		if (GetAsyncKeyState(VK_ESCAPE) < 0 && Scene::isIrisEnd_ && Scene::irisInOut() == true)
		{
			music::play(SE_DECIDE, false);

			Scene::isIrisEnd_ = false;
			Scene::setIrisInOut(false);
			timer_ = 0;
			state_ = TUTORIAL_STATE::IRIS_OUT_TITLE; // Title
			break;
		}

		if (TRG(0) & PAD_LCLICK) // NextPage
		{
			music::play(SE_DECIDE, false);
			if (pageNum_ < pageMax_) ++pageNum_;
			else 
			{ 
				Scene::isIrisEnd_ = false;
				Scene::setIrisInOut(false);
				state_ = TUTORIAL_STATE::IRIS_OUT_TITLE; 
				break; 
			}
		}
		else if (TRG(0) & PAD_RCLICK && pageNum_ > 0) // BackPage
		{
			music::play(SE_RABBIT_HOLD, false);
			--pageNum_;
		}

		// Set ruleSprNo
		{
			const int ruleSprTbl[] = {
				RULE_1,
				RULE_2,
				RULE_3,
				RULE_4,
				RULE_5,
				RULE_6,
				RULE_7,
				RULE_8,
				RULE_9,
			};
			ruleSprNo_ = ruleSprTbl[pageNum_];
		}

		break;
	case TUTORIAL_STATE::IRIS_OUT_TITLE: // title
		music::fade(BGM_TUTORIAL, 0.5f, 0.0f);

		if (Scene::isIrisEnd_)
		{
			++Title::skyColorNum_;
			if (Title::skyColorNum_ > 2) Title::skyColorNum_ = 0;

			Scene::changeScene(Title::instance());
			return;
		}

		break;
	}

}

void Tutorial::draw()
{
	GameLib::clear(VECTOR4(0.1f, 0.1f, 0.1f, 1));

	texture::begin(ruleSprNo_);
	texture::draw(
		ruleSprNo_, { 0,0 }, { 1,1 },
		{ 0,0 }, { 1920, 1080 }, { 0,0 },
		ToRadian(0), { 1,1,1,1 }
	);
	texture::end(ruleSprNo_);

	if (pageNum_ == 6)
	{
		texture::begin(ELEMENT);
		texture::draw(
			ELEMENT,
			VECTOR2(960, 400), { 2,2 },
			{ 0,0 }, { 200,200 }, { 100,100 },
			ToRadian(0), { 1.1f,1.1f,1.1f,1 }
		);
		texture::end(ELEMENT);
	}
	if (pageNum_ == 7)
	{
		texture::begin(ELEMENT);
		texture::draw(
			ELEMENT,
			VECTOR2(1500, 270), { 1.5f,1.5f },
			{ 0,0 }, { 200,200 }, { 100,100 },
			ToRadian(0), { 1,1,1,0.8f }
		);
		texture::end(ELEMENT);
	}
}

void Tutorial::reset()
{
	state_ = 0;
}