#pragma once

#include "framework.h"
#include "animation.h"

#include "game_classes.h"


class Character : public Entity {
public:

	int id;

	char body;
	float height;
	int type;
	int state;

	Vector3 position;
	Vector3 velocity;
	Vector3 front;
	float yaw;
	float pitch;

	char controller;
	float visibility; //0 out of frustum, 1 close to camera

	//anim
	float anim_time_start;
	Skeleton sk;
	std::vector<Matrix44> bone_matrices;

	Character();

	void render(Camera* camera);
	void update(float dt);
	void updateSkeleton(float dt);

	void kill();
	void revive();

	void updateMatrix();
	void updateCamera(Camera* cam, bool first_person = false);
};
