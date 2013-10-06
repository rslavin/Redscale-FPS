#include "glm.h"
#include <stdarg.h>
#include <iostream>
#include <memory.h>
#include <time.h>
#include <fstream>
#include "glut.h"
#include <stdlib.h>
#include <math.h>
#include "Environment.h"
#include "loader.h"
#include "Enemy.h"
#include "hud.h"
#include "avatar.h"
#include <sstream>
#include "vec3f.h"
#include <windows.h>
#include <mmsystem.h>


using namespace std;
//for light
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.20f, 15.0f, 1.0f };
// for colors
GLfloat blue[] = {0.f, 0.f, 1.0f, 1.f};
GLfloat red[] = {1.f, 0.f, .0f, 1.f};
GLfloat white[] = {1.f, 1.f, 1.f, 1.f};
GLfloat black[] = {0.f, 0.f, .0f, 1.f};
GLfloat grey[] = {.5f,.5f,.5f,1.f};
GLfloat yellow[] = {1.f, 1.f, .0f, 1.f};
GLfloat green[] = {0.f, 5.f, .0f, 1.f};

// fog
GLfloat density = 0.0002;
GLfloat fogColor[4] = {0.8, 0.3, 0.3, 1.0}; 

// Default Image dimens5ons

// for states
enum States {WELCOME, MAIN_MENU, MAP_SELECTION, IN_GAME, GAME_STATS, GAME_OVER, END_LEVEL};
States state;
States prevState;

// timer
const int initial = 1000;
int timer = initial;

// FPS
float frame;
int time1;
int time2;
float fps;
float displayFPS;


// night/day
int day = 1;
void lights();

bool showBoundingBoxes = false;
const float GRAVITY = 8.0f;  // 8ish
const float TIME_BETWEEN_UPDATES = 0.01f;
const int TIMER_MS = 25; //The number of milliseconds to which the timer is set
//The amount of time until performUpdate should be called
float _timeUntilUpdate = 0;
int mousePause = 0;

// set the bullet variables here
float bulletRadius = 0.25f;     // radius
float velocity = 300.0f;        // velocity scalar

struct Bullet {
	Vec3f v;         // Velocity
	Vec3f pos;       // Position
	float r;         // Radius
	bool inPlay;     // Is the bullet still in play
	Vec3f color;
};


vector<Bullet*> _bullets; // All of the bullets
vector<Enemy*> _enemies;  // All of the enemies 

void shaderSwitch();
void perspectiveSwitch();
void nightDay();

// hud
void drawHud();
void hudText(float tx, float ty, float tz, const char *message, GLfloat[]);
int score = 0;
int highScore = 250;

int view = 1;
int pview = 1;
int walking = 0;
float speed = 3;
float avatarX, avatarZ = 0;
float avatarY = 20;
float avatarAngle = 0;
float oldAvatarX, oldAvatarZ = 0;
float moveX = 5;
float turretAngle = 20;


// camera variables
float zpos =0;
float eyex,eyey,eyez, lookdirx,lookdiry =0;
float lookdirz =-1;
float sidediry, sidedirz = 0;
float sidedirx =1;
GLfloat viewMatrix[16];

float angle=0;

float cameraX, cameraY;
int mouseX, mouseY; 
int mouseLeft, mouseMiddle, mouseRight = 0;
float cameraLookX, cameraLookZ;
float zoom = 200;
float cameraAngleX, cameraAngleY = 0;
int perspective = 0;
int iWidth, iHeight;

int shaderModel = GL_SMOOTH;

// Default Image dimensions
int imageWidth = 1024;
int imageHeight = 768;

//OpenGL Calls
void setupGL();
void CheckGLError();

//Callbacks
void Display();
void Display_InGame();
void Display_Welcome();
void Idle();
void Reshape(int w, int h);
void Keyboard (unsigned char key, int , int );
void Keyboard_InGame(unsigned char key);
void Keyboard_Welcome(unsigned char key);
void MouseButton(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void PassiveMouseMotion(int x, int y);

// for the bullets
void DrawBullets();
void fireBullet();
void moveBullets(vector<Bullet*> &bullets, float dt);
void checkBullets(vector<Bullet*> &bullets);
void checkCollisions(vector<Bullet*> &bullets, vector<Enemy*> &enemies);
void applyGravity(vector<Bullet*> &bullets);
void performUpdate(vector<Bullet*> &bullets);
void advance(vector<Bullet*> &bullets, float t, float &timeUntilUpdate);
void update(int value);

// for the enemies
void GenerateEnemies(int num);
void RenderEnemies(vector<Enemy*> &enemies);

// a cleanup function
void cleanup(void);
void quit(int i = 0);

//font
GLvoid *font_style = GLUT_BITMAP_HELVETICA_12;

//Loader *model1, *model2, *model3;

void main(int argc, char **argv)
{
	
	// INITIALIZE THE GLUT WINDOW
	glutInit(&argc, argv);  
	glutInitWindowSize(imageWidth, imageHeight);
	glutInitDisplayString("rgb double");
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Red Scale");

	//SETUP GLUT CALLBACKS
	cout << "Setting up callbacks... ";
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	cout << "[completed]\n";
	glEnable(GL_DEPTH_TEST);
    //model1 = new Loader("data/", "duck.dae",.3);
	//model2 = new Loader("data/", "firtree3.lwo",.5);
	//model3 = new Loader("data/", "castle.obj",.5);
	state = WELCOME;
	prevState = WELCOME;
	setupGL();

	
loadSky();
loadWelcome();
	CheckGLError();
//loadModels();
	glutTimerFunc(TIMER_MS, update, 0);
	glutMainLoop();

}



// This function checks the state of openGL and prints 
// an error if an error has occurred
void CheckGLError()
{
	GLenum error;
	error = glGetError();
	if (error!=GL_NO_ERROR)
	{
		cout << "OpenGL reports an error: "<< gluErrorString(error) << endl;
		quit(1);
	}
}

//drawstr method from Nate Robbins tutorials, 1997
void drawstr(GLuint x, GLuint y, char* format, ...)
{

	va_list args;
	char buffer[255], *s;

	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	glRasterPos2i(x, y);
	for (s = buffer; *s; s++)
		glutBitmapCharacter(font_style, *s);
}






void Display(){
	if (view != pview){
		if (view == 1) {
			glutMotionFunc(NULL);
			glutPassiveMotionFunc(PassiveMouseMotion);
		} else {
			glutPassiveMotionFunc(NULL);
			glutMotionFunc(MouseMotion);
		}
		pview = view;
	}

	if (prevState != state){
		// do stuff here that should only be done once when the state is changed
		// reset the screen
		if(state == IN_GAME)
			GenerateEnemies(10);
		if (state == IN_GAME && view == 1)
			glutPassiveMotionFunc(PassiveMouseMotion);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		setupGL();
		prevState = state;
	}

	if (state == IN_GAME){
		Display_InGame();
	} else if (state == WELCOME){
		Display_Welcome();
	} else if (state == END_LEVEL){
		avatarX = avatarZ = 0;



		zpos =0;
		eyex,eyey,eyez, lookdirx,lookdiry =0;
		lookdirz =-1;
		sidediry, sidedirz = 0;
		sidedirx =1;





		state = IN_GAME;
		timer = initial;
		if(score > highScore)
			highScore = score;
		score = 0;
		cleanup();
	}

}


void Display_Welcome(){
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);	

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(0,0,1,0,0,0,0,1,0);
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
	
	// TEXTURE A QUAD
	buildWelcome();
	// swap the buffers
	glutSwapBuffers();
}


void Display_InGame()
{

	
	//fog
	glEnable (GL_FOG);
	glFogi (GL_FOG_MODE, GL_EXP2);
	glFogfv (GL_FOG_COLOR, fogColor);
	glFogf (GL_FOG_DENSITY, density);
	glHint (GL_FOG_HINT, GL_NICEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);	

	glShadeModel(shaderModel);

	//fps 
	frame++;
	time1 = glutGet(GLUT_ELAPSED_TIME);
	if(time1 > 1000){
		fps = frame*1000/(time1-time2);
		time2 = time1;
		frame = 0;
	}

	if(angle>360)
		angle=0;
	angle+=2;
	mousePause++;
	timer--;
	if(timer/100 <= 0)
		timer = 10;


	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();				// Reset MV Matrix
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function


	glPushMatrix();
		glLoadIdentity();
		switch(view){
		/*	gluLookAt (0.0, 0.0, 5.0,  //position of eye 
					0.0, 0.0, 0.0,  //at, where pointing at 
					0.0, 1.0, 0.0);  //up vector of the camera /
		*/
			case 1:
				// first person
				eyex = avatarX;
				eyey = avatarY + 10;
				eyez = avatarZ;
				gluLookAt(eyex,eyey,eyez,eyex+lookdirx,eyey+lookdiry,eyez+lookdirz,0,1,0);
//				gluLookAt(avatarX*5,avatarY,avatarZ*5,
//					(avatarX + sin((avatarAngle - cameraAngleX) * (M_PI / 180)))* 5, avatarY - cameraAngleY/3,(avatarZ + cos((avatarAngle - cameraAngleX) * (M_PI / 180)))*5,
//					0,1,0);
				glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
				sidedirx=viewMatrix[0];
				sidediry=viewMatrix[4];
				sidedirz=viewMatrix[8];
				
				break;
			case 2:
				// third person
				
				gluLookAt((avatarX + zoom * sin((avatarAngle - (cameraAngleX - 180)) * (M_PI / 180)))* 5,avatarY - cameraAngleY/3,(avatarZ + zoom *cos((avatarAngle - (cameraAngleX - 180)) * (M_PI / 180)))*5,
					avatarX*5,avatarY,avatarZ*5,
					0,1,0);
				break;
			case 3:
				// birds eye
				gluLookAt(0,abs(zoom),0,0,0,0,0,0,1);
				glRotatef(cameraY, 1, 0, 0);
				glRotatef(cameraX, 0, 1, 0);
				break;
		}
    
		

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		

		glPushMatrix();	
			glPushMatrix();
                buildSky(eyex, eyey, eyez);	
				buildGround();
			glPopMatrix();
			DrawBullets();
			RenderEnemies(_enemies);
		glPopMatrix();
	glPopMatrix();

	drawHud();
	glutSwapBuffers();
	angle += .5;

	
}

void GenerateEnemies(int num){
	for (int c = 0; c < num; c++){
		Enemy* enemy = new Enemy((rand()%2000) - 1000, 10, (rand()%2000) - 1000, 100, 1, 'a');
		_enemies.push_back(enemy);
//enemy1->setPosition(enemy1->getX()+1, enemy1->getY(), enemy1->getZ());
	}
	for (int c = 0; c < num; c++){
		Enemy* enemy = new Enemy((rand()%2000) - 1000, 10, (rand()%2000) - 1000, 200, 5, 'a');
		_enemies.push_back(enemy);
//enemy1->setPosition(enemy1->getX()+1, enemy1->getY(), enemy1->getZ());
	}
}



void DrawBullets(){
	//Draw the bullet(s)
	for(unsigned int i = 0; i < _bullets.size(); i++) {
		if(_bullets[i]->inPlay){
			cout << "bullets to draw: " << _bullets.size() << "\n";
			Bullet* bullet = _bullets[i];
			glPushMatrix();
			glTranslatef(bullet->pos[0], bullet->pos[1], bullet->pos[2]);
			cout << i << " = x: " << bullet->pos[0] << " y: " << bullet->pos[1] << " z: " << bullet->pos[2] << "\n";
			glColor3f(1.0f, 0.0f, 0.0f);
			glutSolidSphere(bullet->r, 12, 12); //Draw a sphere
			if (showBoundingBoxes){
				glColor3f(0.0f, 0.0f, 0.0f);
				glPushAttrib(GL_ENABLE_BIT);
				glEnable(GL_TEXTURE_2D);
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_LIGHTING);
				glDisable(GL_BLEND);
				glPushMatrix();
					glBegin(GL_LINES);
						glVertex3f(-bulletRadius, -bulletRadius, -bulletRadius);
						glVertex3f(bulletRadius, -bulletRadius, -bulletRadius);

						glVertex3f(bulletRadius, -bulletRadius, -bulletRadius);
						glVertex3f(bulletRadius, bulletRadius, -bulletRadius);

						glVertex3f(bulletRadius, bulletRadius, -bulletRadius);
						glVertex3f(-bulletRadius, bulletRadius, -bulletRadius);

						glVertex3f(-bulletRadius, bulletRadius, -bulletRadius);
						glVertex3f(-bulletRadius, -bulletRadius, -bulletRadius);

						glVertex3f(-bulletRadius, -bulletRadius, bulletRadius);
						glVertex3f(bulletRadius, -bulletRadius, bulletRadius);

						glVertex3f(bulletRadius, -bulletRadius, bulletRadius);
						glVertex3f(bulletRadius, bulletRadius, bulletRadius);

						glVertex3f(bulletRadius, bulletRadius, bulletRadius);
						glVertex3f(-bulletRadius, bulletRadius, bulletRadius);

						glVertex3f(-bulletRadius, bulletRadius, bulletRadius);
						glVertex3f(-bulletRadius, -bulletRadius, bulletRadius);

						
						glVertex3f(-bulletRadius, -bulletRadius, bulletRadius);
						glVertex3f(-bulletRadius, -bulletRadius, -bulletRadius);

						glVertex3f(bulletRadius, -bulletRadius, bulletRadius);
						glVertex3f(bulletRadius, -bulletRadius, -bulletRadius);

						glVertex3f(bulletRadius, bulletRadius, bulletRadius);
						glVertex3f(bulletRadius, bulletRadius, -bulletRadius);

						glVertex3f(-bulletRadius, bulletRadius, bulletRadius);
						glVertex3f(-bulletRadius, bulletRadius, -bulletRadius);

					glEnd();
				glPopMatrix();
				glPopAttrib();
			}
			glPopMatrix();
		}
	}
}

// fire
void fireBullet(){
	if(timer/100 > 0){
		PlaySound("data/Shotgun1[1]",NULL,SND_ASYNC);
		Bullet* bullet = new Bullet();
		bullet->pos = Vec3f(eyex, eyey, eyez);
		bullet->v = Vec3f(lookdirx*velocity,lookdiry*velocity,lookdirz*velocity);
		bullet->r = bulletRadius;
		bullet->inPlay = true;
		bullet->color = Vec3f(0.0f, 0.0f, 0.0f);
		_bullets.push_back(bullet);
	}
}


	


//Moves all of the bullets by their velocity times dt
void moveBullets(vector<Bullet*> &bullets, float dt) {
	for(unsigned int i = 0; i < bullets.size(); i++) {
		Bullet* bullet = bullets[i];
//		Vec3f oldPos = bullet->pos;
		bullet->pos += bullet->v * dt;
		
	}
}

//Decreases the y coordinate of the velocity of each bullet by GRAVITY *
//TIME_BETWEEN_UPDATES
void applyGravity(vector<Bullet*> &bullets) {
	for(unsigned int i = 0; i < bullets.size(); i++) {
		Bullet* bullet = bullets[i];
		bullet->v -= Vec3f(0, GRAVITY * TIME_BETWEEN_UPDATES, 0);
	}
}

//Applies gravity and handles all collisions.  Should be called every
//TIME_BETWEEN_UPDATES seconds.
void performUpdate(vector<Bullet*> &bullets) {
	applyGravity(bullets);
	checkBullets(bullets);
	checkCollisions(bullets, _enemies);
}

void checkBullets(vector<Bullet*> &bullets){
	for(unsigned int i = 0; i < bullets.size(); i++) {
		// check to see if it has dropped below the ground
		if (bullets[i]->pos[1] < 0){
			//bullets.erase (bullets.begin()+i);
			bullets[i]->inPlay = false;
		}
	}
	
}


void checkCollisions(vector<Bullet*> &bullets, vector<Enemy*> &enemies){
	for(unsigned int i = 0; i < bullets.size(); i++) {
		for (unsigned int j = 0; j < enemies.size(); j++){
			
			if (bullets[i]->pos[0] > enemies[j]->bbMins[0] &&
				bullets[i]->pos[0] < enemies[j]->bbMaxs[0] &&
				bullets[i]->pos[1] > enemies[j]->bbMins[1] &&
				bullets[i]->pos[1] < enemies[j]->bbMaxs[1] &&
				bullets[i]->pos[2] > enemies[j]->bbMins[2] &&
				bullets[i]->pos[2] < enemies[j]->bbMaxs[2]){
					// treating the bullet as a point
					score += enemies[j]->getPoints();
					//bullets.erase(bullets.begin() + i);	
					//enemies.erase(enemies.begin() + j);
					bullets[i]->inPlay = false;
					enemies[j]->turnOff();
					PlaySound("data/diescream[1].WAV",NULL,SND_ASYNC);
					
			}
			
		}
	}
}
	


//Advances the state of the bullets by t.  timeUntilUpdate is the amount of time
//until the next call to performUpdate.
void advance(vector<Bullet*> &bullets,
			 float t,
			 float &timeUntilUpdate) {
	while (t > 0) {
		if (timeUntilUpdate <= t) {
			moveBullets(bullets, timeUntilUpdate);
			performUpdate(bullets);
			t -= timeUntilUpdate;
			timeUntilUpdate = TIME_BETWEEN_UPDATES;
		}
		else {
			moveBullets(bullets, t);
			timeUntilUpdate -= t;
			t = 0;
		}
	}
}

//Called every TIMER_MS milliseconds
void update(int value) {
	advance(_bullets, (float)TIMER_MS / 1000.0f, _timeUntilUpdate);

	glutPostRedisplay();
	glutTimerFunc(TIMER_MS, update, 0);
}

// This function draws the scene

// This function is continuously called when events are not being received
// by the window.  This is a good place to update the state of your objects 
// after every frame.
void Idle() {
		moveX += 1;
	glutPostRedisplay();

}

// keyboard handler

void Keyboard (unsigned char key, int , int ){	
	switch(key){
		case 27:
		case 'Q':
		case 'q':
			quit(1);
			break;
		default:
			if (state == IN_GAME){
				Keyboard_InGame(key);
			} else if (state == WELCOME){
				Keyboard_Welcome(key);
			}
	}
}


void MouseButton(int button, int state, int x, int y)
{
	switch (button) 
	{
      case GLUT_LEFT_BUTTON:
		  if(mousePause > 30){
			mouseRight = 0;
			mouseMiddle = 0;
			mouseLeft = 1;	
			mouseX = x;
			mouseY = y;
			fireBullet();
			mousePause = 0;
		  }
		    break;
		case GLUT_MIDDLE_BUTTON:
			mouseMiddle = 1;
			mouseLeft = 0;
			mouseRight = 0;
			break;
		case GLUT_RIGHT_BUTTON:
			mouseLeft = 0;
			mouseMiddle = 0;
			mouseRight = 1;
		    break;
	}
}
float turnSpeed = .01;
void MouseMotion(int x, int y){

	if (state == IN_GAME && ((view == 2) || (view == 3))){
		if(mouseRight){
			cameraY -= (y - mouseY);
			cameraX += (x - mouseX);		

			if(cameraAngleY <= 90 && cameraAngleY >= -90)
				cameraAngleY += (y - mouseY);
			if(cameraAngleY > 90)
				cameraAngleY = 90;
			if(cameraAngleY < -90)
				cameraAngleY = -90;
			if(cameraAngleX <= 90 && cameraAngleX >= -90)
				cameraAngleX += (x - mouseX);
			if(cameraAngleX > 90)
				cameraAngleX = 90;
			if(cameraAngleX < -90)
				cameraAngleX = -90;
			mouseY = y;
			mouseX = x;
		}
		if(mouseMiddle){
				zoom += y;
			if(zoom > 500)
				zoom = 500;
			if(zoom < 0)
				zoom = 0;
			zoom = y;
		}

	}	
}


void PassiveMouseMotion(int x, int y)
{
	//did we move the mouse at all?
	int deltaX = x - imageWidth/2;
	int deltaY = y - imageHeight/2;

	if( deltaX == 0 && deltaY == 0 ) return; //if we didnt move the mouse, dont do anything
	
	// update the look direction based on the side direction and which way we moved the mouse
	lookdirx+= sidedirx*deltaX*turnSpeed; 
	lookdirz+= sidedirz*deltaX*turnSpeed;
	float lookup = lookdiry- deltaY*turnSpeed;
	//if( lookup <1.9 && lookup >-1.9)// What happens if we get rid of this check? Why? Go look at gluLookat in display...
		lookdiry=lookup;		
	glutWarpPointer(imageWidth/2,imageHeight/2); // put the cursor back to the center of the screen
	
}

void setupGL(){

		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glClearDepth(1.0f);				// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

		glEnable(GL_LIGHTING);

		glEnable(GL_LIGHT0);    // Uses default lighting parameters
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glEnable(GL_NORMALIZE);

		glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
     	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
		glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
		glEnable(GL_LIGHT1);

}

void perspectiveSwitch(){
	if(perspective == 0){
		perspective = 1;
		Reshape(iWidth, iHeight);
	}else{
		perspective = 0;
		Reshape(iWidth, iHeight);
	}
}

void shaderSwitch(){
	if(shaderModel == GL_FLAT)
		shaderModel = GL_SMOOTH;
	else
		shaderModel = GL_FLAT;
}

void nightDay(){
	if(day == 1){
		
		day = 0;
	}else{
		
		day = 1;
	}
}
// This functions handles what happens when the window is reshaped
void Reshape(int w, int h) 
{
	imageWidth = iWidth = w;
	imageHeight = iHeight = h;

    glViewport(0,0,w,h);
	double aspect = ((double)w) / ((double)h);
	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
	if(perspective == 1)
		glOrtho(-w, w, -h, h, -1000, 10000);
	else
		gluPerspective(60,aspect,1, 100000);
		 
}
/*

void Reshape(int w, int h) 
{
	imageWidth = w;
	imageHeight = h;

	glViewport(0,0,w,h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,1.0,1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
*/
void Keyboard_Welcome(unsigned char key){
	switch(key){
	
		default:
			state = IN_GAME;
	}
}
void Keyboard_InGame(unsigned char key){
	switch(key){
		case 'L':
		case 'l':
			if (timer/100 <= 0){
				state = END_LEVEL;
			}
		case 'A':
		case 'a': 
			avatarAngle-=5;
			lookdirx-= sidedirx*5*turnSpeed; 
			lookdirz-= sidedirz*5*turnSpeed;
			break;
		case 'W':
		case 'w': 
			avatarX+=lookdirx*speed;
			avatarZ+=lookdirz*speed;
			walking = 1;
			break;
		case 'D':
		case 'd':
			avatarAngle+=5;
			lookdirx+= sidedirx*5*turnSpeed; 
			lookdirz+= sidedirz*5*turnSpeed;
			break;
		case 'S':
		case 's':
			avatarX-=lookdirx*speed;
			avatarZ-=lookdirz*speed;
			walking = 1;
			break;
		case 'P':
		case 'p':
			perspectiveSwitch();
			break;
		case 'M':
		case 'm':
			shaderSwitch();
			break;
		case 'N':
		case 'n':
			nightDay();
			break;		
		case '1':
			view = 1;
			break;
		case '2':
			view = 2;
			break;
		case '3':
			view = 3;
			break;
		case 'F':
		case 'f':
			fireBullet();
			break;
		case 'E':
		case 'e':
			GenerateEnemies(1);
			break;
		case 'B':
		case 'b':
			if (showBoundingBoxes)
				showBoundingBoxes = false;
			else
				showBoundingBoxes = true;
			break;
	}
}
void MenuEvents(int option) {

}

void createMenu() {

	int submenu;

	submenu = glutCreateMenu(MenuEvents);
	/*
	glutAddMenuEntry(const char*,int);
	*/
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


//Deletes everything.  This should be called when exiting the program.
void cleanup() {
	_bullets.clear();
	_enemies.clear();
}

// a cleanup function.  call this when you want to exit.
void quit(int i) 
{
	cleanup();
	exit(i);
}



void lights(){
	float dayHeight = 700;
	float nightHeight = 500;
	float spread = 500;

	if(day){
		GLfloat lightColor[] = {.2f, .2f, .2f, 1.f};
		GLfloat light_position0[] = { spread, dayHeight, spread, 1.0 };
		GLfloat light_ambient0[] = { 0.02, 0.02, 0.02, 1.0 };
		GLfloat light_specular0[] = { 0.01, 0.01, 0.01, 1.0 };
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);

		
		GLfloat light_position1[] = { -spread, dayHeight, -spread, 1.0 };
		GLfloat light_ambient1[] = { 0.02, 0.02, 0.02, 1.0 };
		GLfloat light_specular1[] = { 0.01, 0.01, 0.01, 1.0 };
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);

		GLfloat light_position2[] = { -spread, dayHeight, spread, 1.0 };
		GLfloat light_ambient2[] = { 0.02, 0.02, 0.02, 1.0 };
		GLfloat light_specular2[] = { 0.01, 0.01, 0.01, 1.0 };
		glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor);
		glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
		glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
		glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);

		
		GLfloat light_position3[] = { spread, dayHeight, -spread, 1.0 };
		GLfloat light_ambient3[] = { 0.02, 0.02, 0.02, 1.0 };
		GLfloat light_specular3[] = { 0.01, 0.01, 0.01, 1.0 };
		glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor);
		glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
		glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient3);
		glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular3);	
			

	}else{
		
		GLfloat lightColor[] = {.02f, .02f, .02f, 1.f};
		GLfloat light_position0[] = { spread, nightHeight, spread, 1.0 };
		GLfloat light_ambient0[] = { 0.02, 0.02, 0.02, 1.0 };
		GLfloat light_specular0[] = { 0.01, 0.01, 0.01, 1.0 };
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);

		
		GLfloat light_position1[] = { -spread, nightHeight, -spread, 1.0 };
		GLfloat light_ambient1[] = { 0.02, 0.02, 0.02, 1.0 };
		GLfloat light_specular1[] = { 0.01, 0.01, 0.01, 1.0 };
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);

		GLfloat light_position2[] = { -spread, nightHeight, spread, 1.0 };
		GLfloat light_ambient2[] = { 0.02, 0.02, 0.02, 1.0 };
		GLfloat light_specular2[] = { 0.01, 0.01, 0.01, 1.0 };
		glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor);
		glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
		glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
		glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);

		
		GLfloat light_position3[] = { spread, nightHeight, -spread, 1.0 };
		GLfloat light_ambient3[] = { 0.02, 0.02, 0.02, 1.0 };
		GLfloat light_specular3[] = { 0.01, 0.01, 0.01, 1.0 };
		glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor);
		glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
		glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient3);
		glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular3);	
	}
}




void drawHud(){
    
    glDisable( GL_LIGHTING );    
    
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
        glLoadIdentity();
        glOrtho( -100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f );

        glMatrixMode( GL_MODELVIEW );
		
		if(view == 1 && timer/100 > 0){
			glPushMatrix();
			glutSetCursor (GLUT_CURSOR_NONE);
			glLoadIdentity();
			glColor3fv(red);
            glBegin( GL_QUADS );
				glVertex2f(-3.0f, .2f);
				glVertex2f(3.0f, .2f);
				glVertex2f(3.0f, -.2f);
				glVertex2f(-3.0f, -.2f);
            glEnd();
			glBegin( GL_QUADS );
				glVertex2f(.2f, -4.0f);
				glVertex2f(.2f, 4.0f);
				glVertex2f(-.2f, 4.0f);
				glVertex2f(-.2f, -4.0f);
            glEnd();
        glPopMatrix();
		}else if(view == 1)
			glutSetCursor (GLUT_CURSOR_NONE);
		else
			glutSetCursor (GLUT_CURSOR_LEFT_ARROW);

		// gui box		
        glPushMatrix();
            glLoadIdentity();
			glColor3fv(black);
            glBegin( GL_QUADS );
				glVertex2f(-100.0f, -100.0f);
				glVertex2f(100.0f, -100.0f);
				glVertex2f(100.0f, -60.0f);
				glVertex2f(-100.0f, -60.0f);
            glEnd();
        glPopMatrix();
		glPushMatrix();
			glLoadIdentity();
			glBegin( GL_QUADS );
				glColor3fv(red);
				glVertex2f(-100.0f, -65.0f);
				glVertex2f(100.0f, -65.0f);
				glColor3fv(black);
				glVertex2f(100.0f, -60.0f);
				glVertex2f(-100.0f, -60.0f);
            glEnd();
        glPopMatrix();

	glDisable(GL_TEXTURE_2D);


	// text  
	stringstream myString;
	myString << "Location: " << avatarX << ", "<<avatarY<<", "<<avatarZ;	
	hudText(-90.0f, -75.0f, 0.0f, myString.str().c_str(), blue);
	myString.str("");

	if(timer/100 <= 0){
		glPushMatrix();
            glLoadIdentity();
			glColor3fv(black);
            glBegin( GL_QUADS );
				glVertex2f(-40.0f, -30.0f);
				glVertex2f(40.0f, -30.0f);
				glVertex2f(40.0f, 10.0f);
				glVertex2f(-40.0f, 10.0f);
            glEnd();
        glPopMatrix();
		glColor3fv(red);
		myString << "GAME OVER" ;
		hudText(-20.0f, 0.0f, 0.0f, myString.str().c_str(), red);
		myString.str("");
		myString << "Score: " << score;
		hudText(-20.0f, -20.0f, 0.0f, myString.str().c_str(), red);
		myString.str("");
		if(score > highScore){
			myString << "NEW HIGH SCORE";
			hudText(-30.0f, -10.0f, 0.0f, myString.str().c_str(), red);
			myString.str("");
		}
	}

	if(time1%10 == 0)
			displayFPS = fps;
	myString << "FPS: " << displayFPS;
	hudText(-90.0f, -85.0f, 0.0f, myString.str().c_str(), green);
	myString.str("");

	myString << "Score: " << score;
	hudText(-90.0f, -95.0f, 0.0f, myString.str().c_str(), red);
	myString.str("");

	myString << "TIME: " << timer/100;
	hudText(0.0f, -85.0f, 0.0f, myString.str().c_str(), green);
	myString.str("");

	myString << "High Score: " << highScore;
	hudText(0.0f, -95.0f, 0.0f, myString.str().c_str(), red);

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    // Reenable lighting
    glEnable( GL_LIGHTING );
}


/**
tx, ty, tz = translate
message = message to display
color = color array
**/
void hudText(float tx, float ty, float tz, const char *message, GLfloat color[]){

	 glPushMatrix();
        glLoadIdentity();
        glTranslatef( tx, ty, tz );
        glScalef( 0.05f, 0.05f, 0.05f );
		glColor3fv(color);
		while(*message != '\0')
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *message++);
    glPopMatrix();
}

void RenderEnemies(vector<Enemy*> &enemies){
	for (int c = 0; c < enemies.size(); c++){		
		Enemy* e = enemies[c];
		if(e->isActive()){
			int angleDirection = rand()%2;
			float curX = e->getX();
			float curY = e->getY();
			float curZ = e->getZ();
			float newX = curX + e->getSpeed()*cos(e->getAngle());
			float newY = curY;
			float newZ = curZ + e->getSpeed()*sin(e->getAngle());

			e->setPosition(newX, newY, newZ);
			if(angleDirection)
				e->setAngle(e->getAngle()+.1);
			else
				e->setAngle(e->getAngle()-.1);

			e->setMaxs(Vec3f(5.0f+e->getX(), 15.0f+e->getY(), 5.0f+e->getZ()));
			e->setMins(Vec3f(-5.0f+e->getX(), -10.0f+e->getY(), -5.0f+e->getZ()));

			glPushMatrix();				
				
				glTranslatef(e->getX(), e->getY(), e->getZ());			
				glRotatef(atan((newX -curX)/(newZ - curZ))* (180 / M_PI), 0, 1, 0);	
				

				switch(e->getModel()){
					case 'a':
						glScalef(3.0f, 3.0f, 3.0f);
						drawAvatar(angle, 1);
						break;
					case 'b':
						// render here
						break;
					default:
						break;					
				}
			glPopMatrix();
		}
		
	}
}


