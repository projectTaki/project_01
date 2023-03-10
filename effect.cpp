#include "all.h"

OBJ2D* Effect::effect_[efcMax] = {};

void Effect::init()
{
    state_ = 0;
	timer_ = 0;

	for (auto& efc : effect_)
	{
		efc = new OBJ2D(
			new Transform,
			new Renderer,
			new Collider,
			nullptr
		);
	}
}

void Effect::deinit()
{
	for (auto& efc : effect_) safe_delete(efc);
}

void Effect::update()
{
	switch (state_)
	{
	case 0:
		timer_ = 0;
		for (auto& efc : effect_) setEfc(efc);

		++state_;
		[[fallthrough]];
	case 1:
		// set
		for (auto& efc : effect_)
		{
			if (Game::gameHp() <= 0) break;
			if (efc->exist()) continue;

			//Transform* transform = efc->transform();
			Renderer*  renderer  = efc->renderer();
			//Collider*  collider  = efc->collider();

			int wallSmokeSetTimer = -1;

			     if (Game::gameHp() == 1) wallSmokeSetTimer = timer_ % 10;
			else if (Game::gameHp() == 2) wallSmokeSetTimer = timer_ % 30;
			else if (Game::gameHp() == 3) wallSmokeSetTimer = timer_ % 60;
			
			if (wallSmokeSetTimer == 0)
			{
				setEfc(efc);
				renderer->setSprNo(WALL_SMOKE_EFC);

				float randPosX = static_cast<float>((rand() % (1650 - 135)) + 135);
				float randPosY = static_cast<float>((rand() % 200 + 800));

				// 810, 250
				OBJ2D::searchSet(efc, EFFECT, { randPosX, randPosY }, false);
				
				break;
			}
		}

		// update
		for (auto& efc : effect_)
		{
			if (!efc->exist() && efc->state() > 0)
			{
				setEfc(efc);
				continue;
			}

			//Transform* transform = efc->transform();
			Renderer* renderer = efc->renderer();
			//Collider* collider = efc->collider();

			switch (renderer->sprNo())
			{
			case WALL_SMOKE_EFC: efc->mover_ = wallSmokeEfc; break;
			case EFFECT_NORMAL: [[fallthrough]];
			case EFFECT_FIRE:   [[fallthrough]];
			case EFFECT_WATER:  [[fallthrough]];
			case EFFECT_WIND:   [[fallthrough]];
			case EFFECT_RARE: efc->mover_ = elementEfc; break;

			}

			if (efc->mover_) efc->mover_(efc);

		}

		++timer_;
		break;
	}
}

void Effect::render()
{
	for (auto& efc : effect_)
	{
		if (!efc->exist()) continue;

		Transform* transform = efc->transform();
		Renderer*  renderer  = efc->renderer();
		Collider*  collider  = efc->collider();

		texture::begin(renderer->sprNo());
		texture::draw(
			renderer->sprNo(),
			transform->pos() + BG::bgQuakePos(),
			transform->scale(),
			transform->texPos(),
			collider->size(),
			transform->center(),
			transform->angle(),
			renderer->color()
		);
		texture::end(renderer->sprNo());
	}

}

void Effect::setEfc(OBJ2D* obj)
{
	Transform* transform = obj->transform();
	Renderer*  renderer  = obj->renderer();
	Collider*  collider  = obj->collider();

	obj->setExist(false);
	obj->setState(0);
	obj->mover_ = nullptr;

	renderer->setSprNo(0);
	transform->setPos(VECTOR2(0, 0));
	transform->setScale(VECTOR2(1, 1));
	transform->setTexPos(VECTOR2(0, 0));
	collider->setSize(VECTOR2(0, 0));
	transform->setCenter(VECTOR2(0, 0));
	transform->setAngle(ToRadian(0));
	renderer->setColor(VECTOR4(1, 1, 1, 1));
	transform->setVelocity(VECTOR2(0, 0));

	renderer->setAnimeTimer(0);
	renderer->setAnimeX(0);
	renderer->setAnimeY(0);

}



void wallSmokeEfc(OBJ2D* obj)
{
	if (!obj->exist()) return;

	Transform* transform = obj->transform();
	Renderer*  renderer  = obj->renderer();
	Collider*  collider  = obj->collider();

	switch (obj->state())
	{
	case 0:
		transform->setScale(VECTOR2(1, 1));
		transform->setTexPos(VECTOR2(0, 0));
		collider->setSize(VECTOR2(270, 250));
		transform->setCenter(collider->size() / 2);
		transform->setAngle(ToRadian(0));
		renderer->setColor(VECTOR4(0.75f, 0.75f, 0.75f, 0.85f));

		if (Game::gameHp() == 1)
		{
			transform->setVelocity(VECTOR2(0, -10));
		}
		else if (Game::gameHp() == 2)
		{
			transform->setVelocity(VECTOR2(0,-6));
		}		
		else if (Game::gameHp() == 3)
		{
			transform->setVelocity(VECTOR2(0, -4));
		}

		renderer->setAnimeTimer(0);
		renderer->setAnimeX(0);
		renderer->setAnimeY(0);

		obj->setState(1);
		[[fallthrough]];
	case 1:
		transform->addPos(transform->velocity());

		// update Anime
		renderer->addAnimeTimer(1);
		renderer->setAnimeX(renderer->animeTimer() / 8);
		transform->setTexPosX(collider->size().x * renderer->animeX());


		if (renderer->animeX() >= 3)
		{
			obj->setExist(false);
			break;
		}

		break;
	}
}


void elementEfc(OBJ2D* obj)
{
	if (!obj->exist()) return;

	Transform* transform = obj->transform();
	Renderer* renderer = obj->renderer();
	Collider* collider = obj->collider();

	switch (obj->state())
	{
	case 0:
		transform->setTexPos(VECTOR2(0, 0));
		collider->setSize(VECTOR2(200, 200));
		transform->setCenter(collider->size() / 2);
		renderer->setColor(VECTOR4(1, 1, 1, 1.f));

		renderer->setAnimeTimer(0);
		renderer->setAnimeX(0);
		renderer->setAnimeY(0);

		obj->setState(1);
		[[fallthrough]];
	case 1:
		// update Anime
		renderer->addAnimeTimer(1);
		renderer->setAnimeX(renderer->animeTimer() / 5);
		transform->setTexPosX(collider->size().x * renderer->animeX());


		if (renderer->animeX() >= 3)
		{
			obj->setExist(false);
			break;
		}

		break;
	}
}