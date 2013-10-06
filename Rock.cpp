#include "Rock.h"



Rock::Rock(float x, float y, float z){
	posX = x;
	posY = y;
	posZ = z;	
	angle = 0;
	bbMaxs = Vec3f(5.0f+x, 15.0f+y, 5.0f+z);
	bbMins = Vec3f(-5.0f+x, -10.0f+y, -5.0f+z);
}

Rock::~Rock(void){

}

float Rock::getX(void){ return posX; }
float Rock::getY(void){ return posY; }
float Rock::getZ(void){ return posZ; }
float Rock::getAngle(void){ return angle; }

void Rock::setMins(Vec3f mins){
	bbMins = mins;
}

void Rock::setMaxs(Vec3f maxs){
	bbMaxs = maxs;
}

void Rock::setAngle(float a){
	angle = a;
}


void Rock::setPosition(float x, float y, float z){
	posX = x;
	posY = y;
	posZ = z;
}

void Rock::drawRock(void){
	Loader *rock1;
	rock1 = new Loader("data/", "rock.obj",.3);

	glPushMatrix();
	glScalef(1000,1000,1000);
		glTranslatef(0.0f, -.01f, 0.0f);
		rock1->drawModel();
	glPopMatrix();
}






