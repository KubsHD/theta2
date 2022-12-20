#include "movement.h"

#include <SDL.h>
#include <components/animator.h>
#include <components/sprite.h>
#include <components/collider.h>
#include <components/enemy.h>
#include <core/input.h>


enum PLAYER_STATES
{
	IDLE, RUN, ATTACK
};


String player_anim[3] =
{
	"witch_idle",
	"witch_run",
	"anm_witch_atk_R"
};


PlayerMovement::PlayerMovement(float speed) : speed(speed), is_attacking(false)
{
}

void PlayerMovement::init()
{
	entity->get<Animator>()->play_anim(player_anim[IDLE]);
}


void PlayerMovement::update()
{
	if (Input::key_down(SDL_SCANCODE_SPACE))
	{
		is_attacking = true;

		speed = speed_when_attacking;

		Entity e;
		if (entity->get<Collider>()->check_sphere(entity->position + Vec2(entity->flip ? -10 : 65, 30.0f), 25.0f, CollisionTag::Enemy, e))
		{
			e.get<Enemy>()->health = -1;
		}

		this->entity->get<Animator>()->play_one_shot(player_anim[ATTACK], [this]() {
			is_attacking = false;
			speed = speed_base;

			
		});
	}


	is_running = false;
	
	if (Input::key_held(SDL_SCANCODE_D)) {
		entity->position.x += speed;
		

		if (!is_attacking)
		{
			this->entity->get<Animator>()->flip = false;
			is_running = true;
			entity->flip = false;
		}
	}

	if (Input::key_held(SDL_SCANCODE_S)) {
		entity->position.y += speed;
		is_running = true;
	}

	if (Input::key_held(SDL_SCANCODE_A)) {
		entity->position.x -= speed;


		if (!is_attacking)
		{
			this->entity->get<Animator>()->flip = true;
			is_running = true;
			entity->flip = true;
		}
	}

	if (Input::key_held(SDL_SCANCODE_W)) {
		entity->position.y -= speed;
		is_running = true;
	}


	if (is_running == true )
	{
		this->entity->get<Animator>()->play_anim(player_anim[RUN]);
	}
	else if (is_attacking == false)
	{
		this->entity->get<Animator>()->play_anim(player_anim[IDLE]);
	}


};


void PlayerMovement::render(Renderer* ren)
{

}

