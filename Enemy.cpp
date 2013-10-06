#include "Enemy.h"



Enemy::Enemy(float x, float y, float z, int p, int s, char m){
	posX = x;
	posY = y;
	posZ = z;
	pointValue = p;
	model = m;
	angle = 0;
	speed = s;
	active = true;
	bbMaxs = Vec3f(5.0f+x, 15.0f+y, 5.0f+z);
	bbMins = Vec3f(-5.0f+x, -10.0f+y, -5.0f+z);
}

Enemy::~Enemy(void){

}

float Enemy::getX(void){ return posX; }
float Enemy::getY(void){ return posY; }
float Enemy::getZ(void){ return posZ; }
float Enemy::getAngle(void){ return angle; }
char Enemy::getModel(void){ return model; }
int Enemy::getPoints(void){ return pointValue; }
int Enemy::getSpeed(void){ return speed; }
bool Enemy::isActive(void){ return active; }

void Enemy::turnOff(void){
	active = false;
	bbMaxs = Vec3f(1.1f, 101.0f, 1.1f);
	bbMins = Vec3f(1.0f, 100.0f, 1.0f);
	
}

void Enemy::setSpeed(int s){
	speed = s;
}

void Enemy::setPoints(int p){
	pointValue = p;
}

void Enemy::setMins(Vec3f mins){
	bbMins = mins;
}

void Enemy::setMaxs(Vec3f maxs){
	bbMaxs = maxs;
}


void Enemy::setPosition(float x, float y, float z){
	posX = x;
	posY = y;
	posZ = z;
}

void Enemy::setModel(char m){
	model = m;
}

void Enemy::setAngle(float a){
	if(a < 360 && a > -360)
		angle = a;
}




