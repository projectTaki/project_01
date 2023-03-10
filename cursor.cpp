#include "all.h"

OBJ2D* Cursor::cursor_;

void Cursor::init()
{
	if (!cursor_)
	{
		cursor_ = new OBJ2D(
			new Transform,
			new Renderer,
			new Collider,
			nullptr
		);
	}
}

void Cursor::deinit()
{
	safe_delete(cursor_);
}

void Cursor::update()
{
	using namespace input;

	switch (state_)
	{
	case 0:

		++state_;
		/*fallthrough*/
		[[fallthrough]];
	case 1:
		setCursor();

		++state_;
		/*fallthrough*/
		[[fallthrough]];
	case 2:
		Transform * transform = cursor_->transform();
		Collider* collider = cursor_->collider();
		Renderer* renderer = cursor_->renderer();

		if (!(STATE(0) & PAD_LCLICK) && !cursor_->isTouchCmd())
		{
			renderer->setAnimeY(0);
			cursor_->setIsHold(false);
		}

		GetCursorPos(&csPoint_);                     
		ScreenToClient(window::getHwnd(), &csPoint_);

		if (renderer->animeY() == 2) transform->setPos(VECTOR2((float)csPoint_.x, (float)csPoint_.y - 50));
		else transform->setPos(VECTOR2((float)csPoint_.x, (float)csPoint_.y));


		renderer->setAnimeX(renderer->animeTimer() / 13 % 2);
		transform->setTexPos(
			VECTOR2(collider->size().x * renderer->animeX(),
					collider->size().y * renderer->animeY()));
		renderer->addAnimeTimer(1);
		break;
	}

}

void Cursor::render()
{
	Transform* transform = cursor_->transform();
	Collider* collider   = cursor_->collider();
	Renderer* renderer   = cursor_->renderer();

	texture::begin(CURSOR);
	texture::draw(
		CURSOR,
		transform->pos(),
		transform->scale(),
		transform->texPos(),
		collider->size(),
		transform->center(),
		transform->angle(),
		renderer->color()
	);
	texture::end(CURSOR);

	if (Scene::isDispHitRect_)
	{
		primitive::rect(
			{ transform->pos() + transform->offset() },
			{ collider->hitSize() },
			{ 0,0 },
			0,
			{ 0,1,0,0.5f }
		);
	}

}


// カーソルのパラメータ設定
void Cursor::setCursor()
{
	GetCursorPos(&csPoint_);                      // スクリーン座標を取得する
	ScreenToClient(window::getHwnd(), &csPoint_); // クライアント座標に変換する

	cursor_->collider()->setSize(VECTOR2(150,150));	
	cursor_->transform()->setOffset(-(cursor_->collider()->size() / 2));
	cursor_->transform()->setPos(VECTOR2((float)csPoint_.x, (float)csPoint_.y));
	cursor_->transform()->setScale(VECTOR2(1, 1));
	cursor_->transform()->setTexPos(VECTOR2(0, 0));
	cursor_->transform()->setCenter(VECTOR2(75,75));
	cursor_->transform()->setAngle(ToRadian(0));
	cursor_->renderer()->setColor(VECTOR4(1,1,1,1));

	cursor_->collider()->setHitSize(VECTOR2(69.5f, 70));
	cursor_->transform()->setOffset(-cursor_->collider()->hitSize() / 2);

	cursor_->renderer()->setAnimeTimer(0);
	cursor_->renderer()->setAnimeX(0);
	cursor_->renderer()->setAnimeY(0);

	cursor_->setIsHold(false);
	cursor_->setIsTouchCmd(false);
}
