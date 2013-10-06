#include <stdlib.h>
#include "glm.h"
#include "glut.h"
#include <stdarg.h>
#include <iostream>
#include <memory.h>
#include <time.h>
#include <fstream>
#include "avatar.h"


void drawCube(){	
        glPushMatrix();
     
		glBegin(GL_QUADS);   
				glNormal3f(0, 1, 0);				
                glVertex3f(0, 0, 0);
                glVertex3f(0, 0, -1);
                glVertex3f(-1, 0, -1);
                glVertex3f(-1, 0, 0); 
		glEnd();
		glBegin(GL_QUADS); 
				glNormal3f(0, 0, 1);
                glVertex3f(0, 0, 0);
                glVertex3f(-1, 0, 0);
                glVertex3f(-1, -1, 0);
                glVertex3f(0, -1, 0); 
		glEnd();
		glBegin(GL_QUADS); 
                glNormal3f(1, 0, 0);
				glVertex3f(0, 0, 0);
                glVertex3f(0, -1, 0);
                glVertex3f(0, -1, -1);
                glVertex3f(0, 0, -1);  
		glEnd();
		glBegin(GL_QUADS); 
                glNormal3f(-1, 0, 0);
				glVertex3f(-1, 0, 0);
                glVertex3f(-1, 0, -1);
                glVertex3f(-1, -1, -1);
                glVertex3f(-1, -1, 0);
		glEnd();
		glBegin(GL_QUADS); 
				glNormal3f(0, -1, 0);
                glVertex3f(0, 0, 0);
                glVertex3f(0, -1, -1);
                glVertex3f(-1, -1, -1);
                glVertex3f(-1, -1, 0);
		glEnd();
		glBegin(GL_QUADS); 
                glNormal3f(0, 0, -1);
				glVertex3f(0, 0, -1);
                glVertex3f(-1, 0, -1);
                glVertex3f(-1, -1, -1);
                glVertex3f(0, -1, -1);
        glEnd();
        glPopMatrix();
}


void drawTorso(float x, float y, float z){
	glPushMatrix();
		glTranslatef((x/2), (y/2), (z/2));
		glScalef(x, y, z);
		drawCube();
	glPopMatrix();
}

void drawHead(float radius, float x, float y, float z){
	glPushMatrix();
		glTranslatef(x, y, z);
		glutSolidSphere(radius, 100, 100);
	glPopMatrix();
}

void drawArmRight(float angle){
	glPushMatrix();
		glTranslatef(1.2, 1.5, 0);
		glRotatef(angle, 1, 0, 0);
		glScalef(.4, 3, .4);
		drawCube();
	glPopMatrix();
}

void drawArmLeft(float angle){
	glPushMatrix();
		glTranslatef(-.75, 1.5, 0);
		glRotatef(angle, 1, 0, 0);
		glScalef(.4, 3, .4);
		drawCube();
	glPopMatrix();
}

void drawLegRight(float angle){
	glPushMatrix();
		glTranslatef(.85, -1.5, 0);
		glRotatef(angle, 1, 0, 0);
		glScalef(.4, 1.6, .4);
		drawCube();
		drawFoot(0, -1, 0);
	glPopMatrix();
}

void drawLegLeft(float angle){
	glPushMatrix();		
		glTranslatef(-.5, -1.5, 0);
		glRotatef(angle, 1, 0, 0);
		glScalef(.4, 1.6, .4);
		drawCube();
		drawFoot( 0, -1, 0);
		
	glPopMatrix();
}

void drawFoot(float x, float y, float z){	
	glPushMatrix();
		glTranslatef(x, y, z);		
		glScalef(1.25, .2, 2);
		drawCube();
	glPopMatrix();
}

void drawAvatar(float angle, int walk){
	if(walk){
		if(angle >= 315){
			angle = angle - 360;
		}else if(angle >= 225){
			angle = 270 - angle;
		}else if(angle >= 135){
			angle = angle-180;
		}else if(angle >= 45){
			angle = 90-angle;
		}
	}else{
		angle = 0;
	}
	
	glPushMatrix();
		glRotatef(150, 0, 1, 0);
		glRotatef(30, 0, 1, 0);
		drawHead(.75, 0, 2.25, 0);
		drawTorso(1.5, 3, 1);
		drawArmLeft(-angle);
		drawArmRight(angle);
		drawLegLeft(-angle);
		drawLegRight(angle);
	glPopMatrix();
	
}