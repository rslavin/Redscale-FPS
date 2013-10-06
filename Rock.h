#ifndef ROCK_H
#define ROCK_H

#include "vec3f.h"
#include "loader.h"

class Rock {
public: 
	Rock(float, float, float);
	~Rock(void);
	float getX(void);
	float getY(void);
	float getZ(void);
	float getAngle(void);
	void setAngle(float);
	void Rock::setPosition(float x, float y, float z);
	void Rock::setMins(Vec3f);
	void Rock::setMaxs(Vec3f);
	Vec3f bbMins;
	Vec3f bbMaxs;
	void Rock::drawRock(void);
	

private:
	float posX, posY, posZ, angle;
	
};
#endif // ROCK_H