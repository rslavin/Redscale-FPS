#include <stdlib.h>
#include "glm.h"
#include "glut.h"
#include <stdarg.h>
#include <iostream>
#include <memory.h>
#include <time.h>
#include <fstream>
#include "loader.h"
#include "Environment.h"
GLuint skyleft;
GLuint skyright;
GLuint skyfront;
GLuint skyback;
GLuint skyfloor;
GLuint skyroof;
GLuint welcome;
Loader *mount, *grass, *rock, *tree;
#define SKYBackSIZE		4000.00 
GLfloat quad_mat[] =  {1,1,1,1};

float rocks[150][4];// x, y, angle, scale
float trees[150][4];// x, y, angle, scale

void loadSky(){
	mount = new Loader("data/", "mountains.obj",.3);
	grass = new Loader("data/", "treeGround.obj",.3);
	rock = new Loader("data/", "rock.obj",.3);
	tree = new Loader("data/", "tree.obj",.3);
CBmp back =  CBmp("./data/Back1.bmp");
for(int c = 0; c < 150; c++)	{	
	rocks[c][0] = rand()%2000-2000;
	rocks[c][1] = rand()%2000-2000;
	rocks[c][2] = rand()%360;
	rocks[c][3] = 700;
	
}
for(int c = 0; c < 150; c++)	{	
	trees[c][0] = rand()%2000-2000;
	trees[c][1] = rand()%2000-2000;
	trees[c][2] = rand()%360;
	trees[c][3] = 700;
	
}
		

if(back.init())
			{
				
				glGenTextures(1,&skyback);
				glBindTexture(GL_TEXTURE_2D, skyback);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,back.width,back.height,0,GL_RGB,GL_UNSIGNED_BYTE,back.m_pDibBuffer);
			}
CBmp front =  CBmp("./data/Front1.bmp");
if(front.init())
			{
				
				glGenTextures(1,&skyfront);
				glBindTexture(GL_TEXTURE_2D, skyfront);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,front.width,front.height,0,GL_RGB,GL_UNSIGNED_BYTE,front.m_pDibBuffer);
			}

CBmp roof =  CBmp("./data/Top1.bmp");
if(roof.init())
			{
				
				glGenTextures(1,&skyroof);
				glBindTexture(GL_TEXTURE_2D, skyroof);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,roof.width,roof.height,0,GL_RGB,GL_UNSIGNED_BYTE,roof.m_pDibBuffer);
			}

CBmp left =  CBmp("./data/Left1.bmp");
if(left.init())
			{
				
				glGenTextures(1,&skyleft);
				glBindTexture(GL_TEXTURE_2D, skyleft);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,left.width,left.height,0,GL_RGB,GL_UNSIGNED_BYTE,left.m_pDibBuffer);
			}
CBmp floor=  CBmp("./data/Ground.bmp");
if(floor.init())
			{
				
				glGenTextures(1,&skyfloor);
				glBindTexture(GL_TEXTURE_2D, skyfloor);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,floor.width,floor.height,0,GL_RGB,GL_UNSIGNED_BYTE,floor.m_pDibBuffer);
			}
CBmp right =  CBmp("./data/Right1.bmp");
if(right.init())
			{
				
				glGenTextures(1,&skyright);
				glBindTexture(GL_TEXTURE_2D, skyright);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,right.width,right.height,0,GL_RGB,GL_UNSIGNED_BYTE,right.m_pDibBuffer);
			}



}

void loadWelcome(){
	CBmp welcomeImg =  CBmp("./data/Welcome.bmp");
	if(welcomeImg.init())
	{
		glGenTextures(1,&welcome);
		glBindTexture(GL_TEXTURE_2D, welcome);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,welcomeImg.width,welcomeImg.height,0,GL_BGR_EXT,GL_UNSIGNED_BYTE,welcomeImg.m_pDibBuffer);
	}
}

void buildWelcome(){
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);

		//change the GL_MODULATE parameter for diffent effects. Look in the redbood for examples
		//glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, welcome);
		//glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, quad_mat);
		glBegin(GL_QUADS);
		//glColor4f(1, 1, 1, 1);
		glTexCoord2f(0,0);
		glVertex3f(-.6f,-.5f,0);

		glTexCoord2f(1,0);
		glVertex3f(.6,-.5f,0);

		glTexCoord2f(1,1);
		glVertex3f(.6f,.5f,0);

		glTexCoord2f(0,1);
		glVertex3f(-.6f,.5f,0);

		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
		


}


void buildSky(float x, float y, float z){

	glPushMatrix();
	// Move the render space to the correct position (Translate)
	glTranslatef(x, y, z);

	// Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
 
    // Just in case we set all vertices to white.
    glColor4f(1,1,1,1);
 
  glBindTexture(GL_TEXTURE_2D, skyback);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex3f(SKYBackSIZE, -SKYBackSIZE+2000, -SKYBackSIZE*2);
  glTexCoord2f(1,0);
  glVertex3f(-SKYBackSIZE, -SKYBackSIZE+2000, -SKYBackSIZE*2);
  glTexCoord2f(1,1);
  glVertex3f(-SKYBackSIZE, SKYBackSIZE+2000, -SKYBackSIZE*2);
  glTexCoord2f(0,1);
  glVertex3f(SKYBackSIZE, SKYBackSIZE+2000, -SKYBackSIZE*2);
  glEnd();

glBindTexture(GL_TEXTURE_2D, skyfront);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex3f(-SKYBackSIZE, -SKYBackSIZE+2000, SKYBackSIZE*2);
  glTexCoord2f(1,0);
  glVertex3f(SKYBackSIZE, -SKYBackSIZE+2000, SKYBackSIZE*2);
  glTexCoord2f(1,1);
  glVertex3f(SKYBackSIZE, SKYBackSIZE+2000, SKYBackSIZE*2);
  glTexCoord2f(0,1);
  glVertex3f(-SKYBackSIZE, SKYBackSIZE+2000, SKYBackSIZE*2);
  glEnd();

glBindTexture(GL_TEXTURE_2D, skyleft);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex3f(SKYBackSIZE, -SKYBackSIZE+2000, SKYBackSIZE*2);
  glTexCoord2f(1,0);
  glVertex3f(SKYBackSIZE, -SKYBackSIZE+2000, -SKYBackSIZE*2);
  glTexCoord2f(1,1);
  glVertex3f(SKYBackSIZE, SKYBackSIZE+2000, -SKYBackSIZE*2);
  glTexCoord2f(0,1);
  glVertex3f(SKYBackSIZE, SKYBackSIZE+2000, SKYBackSIZE*2);
  glEnd();

glBindTexture(GL_TEXTURE_2D, skyright);
    glBegin(GL_QUADS);
	glTexCoord2f(0,0);
  glVertex3f(-SKYBackSIZE, -SKYBackSIZE+2000, -SKYBackSIZE*2);
  glTexCoord2f(1,0);
  glVertex3f(-SKYBackSIZE, -SKYBackSIZE+2000, SKYBackSIZE*2);
  glTexCoord2f(1,1);
  glVertex3f(-SKYBackSIZE, SKYBackSIZE+2000, SKYBackSIZE*2);
  glTexCoord2f(0,1);
  glVertex3f(-SKYBackSIZE, SKYBackSIZE+2000, -SKYBackSIZE*2);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, skyroof);
    glBegin(GL_QUADS);
	glTexCoord2f(0,1);
  glVertex3f(-SKYBackSIZE, SKYBackSIZE+2000, -SKYBackSIZE*2);
  glTexCoord2f(0,0);
  glVertex3f(-SKYBackSIZE, SKYBackSIZE+2000, SKYBackSIZE*2);
  glTexCoord2f(1,0);
  glVertex3f(SKYBackSIZE, SKYBackSIZE+2000, SKYBackSIZE*2);
  glTexCoord2f(1,1);
  glVertex3f(SKYBackSIZE, SKYBackSIZE+2000, -SKYBackSIZE*2);
  glEnd();

      // Restore enable bits and matrix
    glPopAttrib();
	glPopMatrix();


}
void buildGround(){

	glPushMatrix();
glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, skyfloor);
    glBegin(GL_QUADS);
	glTexCoord2f(0,0);
  glVertex3f(-4000, 0, -4000);
  glTexCoord2f(0,120);
  glVertex3f(-4000,0 , 4000);
  glTexCoord2f(120,120);
  glVertex3f(4000, 0, 4000);
  glTexCoord2f(120,0);
  glVertex3f(4000,0, -4000);
  glEnd();



 glPushMatrix();
 glScalef(5000,5000,5000);
	glTranslatef(0.0f, -0.005f, 0.0f);
	mount->drawModel();
	
 glPopMatrix();
 // rocks
	//for(int count = 1; count < 15; count++)
	//	drawRock(rand()%20-10, rand()%20-10, rand()%360, rand()%1000);
for(int c = 0; c < 15; c++)	{
	 drawRock(rocks[c][0], rocks[c][1], rocks[c][2], rocks[c][3]);
 }
for(int c = 0; c < 15; c++)	{
	 drawTree(trees[c][0], trees[c][1], trees[c][2], trees[c][3]);
 }
/*
	glPushMatrix();
		glScalef(1000,1000,1000);
		glRotatef(40, 0, 1, 0);
		glTranslatef(0, -.01f, 0);
		rock->drawModel();
	glPopMatrix();
	*/
  glPopMatrix();
glDisable(GL_TEXTURE_2D);

}

void drawRock(float x, float z, float angle, float size){
	glPushMatrix();
		
		glRotatef(angle, 0, 1, 0);
		glTranslatef(x, -10.0f, z);
		glScalef(size,size,size);
		rock->drawModel();
	glPopMatrix();
}

void drawTree(float x, float z, float angle, float size){
	glPushMatrix();		
		glRotatef(angle, 0, 1, 0);
		glTranslatef(x, -10.0f, z);
		glScalef(size,size,size);
		tree->drawModel();
	glPopMatrix();
}