#pragma once

#define randomInt() (rand()%10)
#define randomIntRange(min,max) (min + ( rand() % ( max - min + 1 ) ))
#define mapValue(x,minIn,maxIn,minOut,maxOut) ((x - minIn) * (maxOut - minOut) / (maxIn - minIn) + minOut);

enum class eDirection {
	FORWARD,
	BACKWARDS,
	LEFT,
	RIGHT,
};

enum class eStageType {
	BASE,
	PROCEDURAL_WORLD,
	DEPO,
};