#include "character.h"

#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"

#include "game.h"
#include "world.h"
#include "input.h"


Character::Character()
{
	id = 0;
	body = 0;
	controller = 0;
	visible = true;
	yaw = 0;
	pitch = 0;
	visibility = 1;
	height = 10;
	state = 0;
	//shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/flat_shaded.fs");
	shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	//shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
}

void Character::updateMatrix()
{
	model.setIdentity();
	model.translate(position.x, position.y, position.z);
	model.rotate( (yaw + 180 ) * DEG2RAD, Vector3(0, 1, 0));
}

void Character::updateCamera(Camera* cam, bool first_person )
{
	Matrix44 yaw_mat;
	yaw_mat.rotate(yaw * DEG2RAD, Vector3(0, 1, 0));

	Vector3 right = yaw_mat * Vector3(1, 0, 0);
	Matrix44 pitch_mat;
	pitch_mat.rotate(pitch * DEG2RAD, right);

	Matrix44 r = yaw_mat * pitch_mat;

	Vector3 front = r * Vector3(0, 0, -1);
	Vector3 eye = position + Vector3(0, height, 0);
	if (!first_person)
		eye = eye - front * 10 + Vector3(0,2,0);
	Vector3 center = eye + front;
	cam->lookAt(eye, center, Vector3(0, 1, 0));

	visible = true;// !first_person;
}

void Character::render(Camera* camera)
{
	if (!visible)
	{
		visibility = 0;
		return;
	}

	Vector3 center = position + Vector3(0, 5, 0);
	if (camera->testSphereInFrustum(center, 15) == CLIP_OUTSIDE)
		return;

	float distance = camera->eye.distance(center);
	float v = clamp(80.0 / distance, 0, 1);
	if (visibility < v)
		visibility = v;

	if (!bone_matrices.size())
		return;

	updateMatrix();

	mesh = Mesh::Get(body == 0 ? "data/characters/male.mesh" : "data/characters/female.mesh");
	texture = Texture::Get(body == 0 ? "data/characters/male.png" : "data/characters/female.png");

	//Animation* anim = Animation::Get("data/characters/walking.skanim");
	//anim->assignTime(Game::instance->time*1.2 + id*0.1, false);

	//Animation* anim2 = Animation::Get("data/characters/crouch walking.skanim");
	//anim2->assignTime(Game::instance->time*1.2 + id*0.1, false);

	Matrix44 m = model;
	float s = 0.5 * height / 10.0;
	m.scale(s, s, s);

	shader->enable();
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_model", m);
	shader->setUniform("u_texture", texture);
	shader->setUniform("u_color", World::instance->ambient_light);
	shader->setUniform("u_bones", bone_matrices );
	mesh->render(GL_TRIANGLES);
	shader->disable();

	if (0)
	{
		glDisable(GL_DEPTH_TEST);
		glLineWidth(3);
		sk.renderSkeleton(camera, m);
		glEnable(GL_DEPTH_TEST);
	}
}

void Character::update(float dt)
{
	Vector3 move;
	float yaw_delta = 0;
	float pitch_delta = 0;
	if (controller == 1)
	{
		if (Input::isKeyPressed(SDL_SCANCODE_W))
			move.z -= dt * 20;
		if (Input::isKeyPressed(SDL_SCANCODE_S))
			move.z += dt * 10;
		if (Input::isKeyPressed(SDL_SCANCODE_A))
			move.x -= dt * 10;
		if (Input::isKeyPressed(SDL_SCANCODE_D))
			move.x += dt * 10;
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT))
			move.z *= 3;
		if (Input::wasKeyPressed(SDL_SCANCODE_DELETE))
		{
			if (state != -1)
				kill();
			else
				revive();
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_B))
			World::instance->dance_mode = !World::instance->dance_mode;

		if (Game::instance->mouse_locked || Input::isMousePressed(SDL_BUTTON_LEFT))
		{
			yaw_delta = Input::mouse_delta.x * dt * -20;
			pitch_delta = Input::mouse_delta.y * dt * -20;
		}
	}
	if (controller == 2)
	{
		if (abs(Input::gamepads[0].axis[LEFT_ANALOG_Y]) > 0.25)
			move.z += dt * 20 * Input::gamepads[0].axis[LEFT_ANALOG_Y];
		if (abs(Input::gamepads[0].axis[LEFT_ANALOG_X]) > 0.25)
			move.x += dt * 10 * Input::gamepads[0].axis[LEFT_ANALOG_X];
		if (abs(Input::gamepads[0].axis[RIGHT_ANALOG_X]) > 0.25)
			yaw_delta = dt * 90 * Input::gamepads[0].axis[RIGHT_ANALOG_X];
		if (abs(Input::gamepads[0].axis[RIGHT_ANALOG_Y]) > 0.25)
			pitch_delta = dt * 90 * Input::gamepads[0].axis[RIGHT_ANALOG_Y];
		if (Input::gamepads[0].axis[TRIGGER_LEFT] > 0.5)
			move.z *= 3;
	}

	if (state != -1)
	{
		Matrix44 rot;
		rot.rotate(yaw*DEG2RAD, Vector3(0, 1, 0));
		move = rot * move;
		velocity = velocity + move * 3;
		position = position + velocity * dt;
		Matrix44 R;
		R.rotate(yaw * DEG2RAD, Vector3(0, 1, 0));
		front = R * Vector3(0, 0, -1);

		yaw += yaw_delta;
		pitch += pitch_delta;
		pitch = clamp(pitch, -89.0f, 89.0f);

		float friction = 1.0 / (1.0 + (dt * 4.5));
		velocity = velocity * friction;
	}

	if (visibility != 0)
		updateSkeleton(dt);

	visibility = -1;
}

void Character::updateSkeleton(float dt)
{
	//if (visibility < 0.8 && (Game::instance->frame + id) % 5)
	//	return;

	//update anim
	Mesh* mesh = Mesh::Get(body == 0 ? "data/characters/male.mesh" : "data/characters/female.mesh");

	float t = World::instance->time + id;
	float speed = velocity.length() * 0.1;
	float w = 0.0;
	Vector3 vel = velocity;
		
	if (velocity.length() > 0.01)
	{
		normalize(velocity); //vel in local space
		Matrix44 R;
		R.rotate(-yaw*DEG2RAD, Vector3(0, 1, 0));
		vel = R * vel;
	}

	Animation* anim = NULL;
	Animation* animB = NULL;
	const char* idle_name = "data/characters/idle.skanim";

	if(World::instance->dance_mode)
		idle_name = "data/characters/rumba.skanim";

	if (state == -1) //die
	{
		anim = Animation::Get("data/characters/stunned.skanim");
		anim->assignTime( getTime()*0.001 - anim_time_start, false );
		sk = anim->skeleton;
	}
	else if (state == -2) //revive
	{
		anim = Animation::Get("data/characters/stunned.skanim");
		float f = (getTime()*0.001 - anim_time_start) / anim->duration;
		anim->assignTime((1.0 - f) * anim->duration, false);

		animB = Animation::Get(idle_name);
		animB->assignTime(t);
		blendSkeleton( &anim->skeleton, &animB->skeleton, f, &sk );

		if (f >= 1.0)
			state = 0;
	}
	else if (speed < 0.01) //idle
	{
		anim = Animation::Get(idle_name);
		anim->assignTime(t);
		sk = anim->skeleton;
	}
	else if (speed < 1.0) //walk
	{
		anim = Animation::Get(idle_name);
		anim->assignTime(t);
		w = clamp(speed, 0, 1);
		animB = Animation::Get("data/characters/walking.skanim");
		if (vel.z < 0)
			animB->assignTime(t);
		else
			animB->assignTime(-t);
		blendSkeleton( &anim->skeleton, &animB->skeleton, w, &sk );
	}
	else //run
	{
		w = clamp(speed - 1.0, 0, 1);
		anim = Animation::Get("data/characters/walking.skanim");
		if (vel.z < 0)
			anim->assignTime(t);
		else
			anim->assignTime(-t);
		animB = Animation::Get("data/characters/running.skanim");
		if (vel.z < 0)
			animB->assignTime((t / anim->duration) * animB->duration);
		else
			animB->assignTime(-(t / anim->duration) * animB->duration);
		blendSkeleton(&anim->skeleton, &animB->skeleton, w, &sk);
	}

	/*
	anim = Animation::Get("data/characters/macarena.skanim");
	anim->assignTime(t, true);
	sk.blend(&anim->skeleton, 0.8, UPPER_BODY );
	*/

	sk.computeFinalBoneMatrices( bone_matrices, mesh );
}


void Character::kill()
{
	state = -1;
	anim_time_start = getTime() * 0.001;
	velocity.set(0, 0, 0);
}

void Character::revive()
{
	state = -2;
	anim_time_start = getTime() * 0.001;
	velocity.set(0, 0, 0);
}