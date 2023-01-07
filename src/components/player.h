#pragma once 

#include <core/types.h>
#include <core/ecs.h>
#include "enemy.h"
#include <iostream>


class Player : public Component
{
public:
	// Custom variables
	float health, speed;
	int money;
	float damage_melee;
	Collider* collider;
	Vec2 pos_sprite_center;
	bool god_mode;
	float health_decay;

	Player();

	// Maintenance
	void update() override;
	void render(Renderer* ren) override;
};