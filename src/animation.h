#pragma once

#include "mesh.h"

class Camera;

#define ANIM_BIN_VERSION 1

enum BODY_LAYERS {
	BODY = 1,
	UPPER_BODY = 2,
	LOWER_BODY = 4,
	LEFT_ARM = 8,
	RIGHT_ARM = 16,
	LEFT_LEG = 32,
	RIGHT_LEG = 64,
	HIPS = 128,
};

//This class contains the bone structure hierarchy
class Skeleton {
public:
	//fixed size to help serializing
	struct Bone {
		int8 parent;
		char name[32];
		Matrix44 model;
		uint8 layer;
		uint8 num_children;
		int8 children[16]; //max 16 children 
	};
	Bone bones[128]; //max 128 bones
	int num_bones;
	Matrix44 global_bone_matrices[128];
	std::map<std::string, int> bones_by_name;

	Skeleton();

	Bone* getBone(const char* name);
	Matrix44& getBoneMatrix(const char* name, bool local = false);
	void applyTransformToBones(const char* root, Matrix44 transform);
	void updateGlobalMatrices();

	void blend(Skeleton* sk, float w, uint8 layer = 0xFF);
	void renderSkeleton(Camera* camera, Matrix44 model, Vector4 color = Vector4(0.5, 0, 0.5, 1), bool render_points = false);
	void computeFinalBoneMatrices(std::vector<Matrix44>& bones, Mesh* mesh);
	void assignLayer(Bone* bone, uint8 layer);
};

//This class contains one animation loaded from a file (it also uses a skeleton to store the current snapshot)
class Animation {
public:

	Skeleton skeleton;

	float duration;
	float samples_per_second;
	int num_animated_bones;
	int num_keyframes;
	int8 bones_map[128]; //maps from keyframe data index to bone

	Matrix44* keyframes;

	Animation();
	~Animation();

	void assignTime(float time, bool loop = true, bool interpolate = true, uint8 layers = 0xFF);

	bool load(const char* filename);

	bool loadSKANIM(const char* filename);
	bool loadABIN(const char* filename);
	bool writeABIN(const char* filename);


	static std::map<std::string, Animation*> sAnimationsLoaded;
	static Animation* Get(const char* filename);

	void operator = (Animation* anim);
};

