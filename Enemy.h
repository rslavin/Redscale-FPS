#ifndef ENEMY_H
#define ENEMY_H

#include "vec3f.h"

class Enemy {
public: 
	Enemy(float, float, float, int, int, char);
	~Enemy(void);
	float getX(void);
	float getY(void);
	float getZ(void);
	float getAngle(void);
	char getModel(void);
	void setPosition(float, float, float);
	void setModel(char);
	void setAngle(float);
	void setMaxHealth(int);
	void setHealth(int);
	void setPoints(int);
	void setSpeed(int);
	int getSpeed(void);
	int getPoints(void);
	void Enemy::setMins(Vec3f);
	void Enemy::setMaxs(Vec3f);
	Vec3f bbMins;
	Vec3f bbMaxs;
	void turnOff(void);
	bool isActive(void);
	

private:
	float posX, posY, posZ, angle;
	int pointValue;
	int speed;
	char model;
	bool active;
	
};
#endif // OBJECTS_H