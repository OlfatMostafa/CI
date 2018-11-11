#include <iostream>
#include <string>
using namespace std;
#include "stdafx.h"


void Anim();
void Display();
int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include "TextureBuilder.h"
#include <glut.h>
#include <ctime>

#define GLUT_KEY_ESCAPE 27
GLuint texID;

int rep = 1;
int keyC, speC, obsCount, p0[2], p1[2], p2[2], p3[2], lives = 1, tar = 4, randMotion, score, randPowerupTime, window_valid = 1,
powerupTimer, w = 1920, h = 1080, defenderToTheRescueT, rotationAngleP, rotationAngleA, randCurve;

float attacker[2], player[2], fire[2], obs[2], obsD[2], powerUp[2], defender[2], powerupTranslationX[2], powerupTranslationY[2], attackerHealthPoint, t,
r1, fireMov, value, max_value, progress, progressBarWidth, x, y, attackerOld[2], angle;

bool fire2, rotateLeftp, rotateRightp, rotateLefta, rotateRighta, defRev, powerupToggle, powerupActivate, defenderToTheRescue, rev, rm, destroy,
obsAttacker, obsDefender, fireToggle, lose = true, initf = true, inito = true;


void destroy_window() {

	// whatever

	window_valid = -1;
}
// draws a circle using OpenGL's gluDisk, given (x,y) of its center and tis radius
void drawCircle(int x, int y, float r) {
	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);//Green
	glTranslatef(x, y, 0);
	GLUquadric *quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
	glutPostRedisplay();

}

void shoot() {
	glPushMatrix();
	glTranslated(fire[0], fire[1], 0);
	//glPointSize(10);
	glColor3f(225, 215, 0);//gold
	glBegin(GL_TRIANGLES);
	glVertex2f(0, -50);
	glVertex2f(20, -50);
	glVertex2f(10, -30);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(10, -40);
	glVertex2f(0, -30);
	glVertex2f(0, -30);
	glVertex2f(20, -20);
	glVertex2f(20, -20);
	glVertex2f(0, -10);
	glEnd();
	glBegin(GL_QUADS);
	glVertex2f(0, -10);
	glVertex2f(20, -10);
	glVertex2f(20, 10);
	glVertex2f(0, 10);
	glEnd();
	glPopMatrix();
	glutPostRedisplay();
}

//this is the method used to print text in OpenGL
//there are three parameters,
//the first two are the coordinates where the text is display,
//the third coordinate is the string containing the text to display
void print(int x, int y, char *string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

int* bezier(float t, int* p0, int* p1, int* p2, int* p3)
{
	int res[2];
	res[0] = pow((1 - t), 3)*p0[0] + 3 * t*pow((1 - t), 2)*p1[0] + 3 * pow(t, 2)*(1 - t)*p2[0] + pow(t, 3)*p3[0];
	res[1] = pow((1 - t), 3)*p0[1] + 3 * t*pow((1 - t), 2)*p1[1] + 3 * pow(t, 2)*(1 - t)*p2[1] + pow(t, 3)*p3[1];
	return res;
}

void mo(int x, int y)
{
	y = 600 - y;
	if (x<0)
		x = 0;
	if (x>1000)
		x = 1000;
	if (y<0)
		y = 0;
	if (y>1000)
		y = 1000;
	if (tar == 0)
	{
		p0[0] = x;
		p0[1] = y;
	}
	else if (tar == 1)
	{
		p1[0] = x;
		p1[1] = y;
	}
	else if (tar == 2)
	{
		p2[0] = x;
		p2[1] = y;
	}
	if (tar == 3)
	{
		p3[0] = x;
		p3[1] = y;
	}
	glutPostRedisplay();
}

void mou(int b, int s, int x, int y)
{
	y = 600 - y;
	if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN)
	{
		if (p0[0]<x + 9 && p0[0]>x - 9 && p0[1]<y + 9 && p0[1]>y - 9)
		{
			tar = 0;
		}
		else if (p1[0]<x + 9 && p1[0]>x - 9 && p1[1]<y + 9 && p1[1]>y - 9)
		{
			tar = 1;
		}
		else if (p2[0]<x + 9 && p2[0]>x - 9 && p2[1]<y + 9 && p2[1]>y - 9)
		{
			tar = 2;
		}
		else if (p3[0]<x + 9 && p3[0]>x - 9 && p3[1]<y + 9 && p3[1]>y - 9)
		{
			tar = 3;
		}
	}
	if (b == GLUT_LEFT_BUTTON && s == GLUT_UP)
		tar = 4;
}

void spe(int k, int x, int y)// keyboard special key function takes 3 parameters
// int k: is the special key pressed such as the keyboard arrows the f1,2,3 and so on
{
	if (k == GLUT_KEY_RIGHT)//if the right arrow is pressed, then the object will be translated in the x axis by 10. (moving right)
	{
		if (!rotateRightp)
			player[1] += 10;
		rotationAngleP = -10;
		player[0] += 10;
		rotateRightp = true;
		if (player[0] >= (925 - 10))
		{
			player[0] = 925 - 10;
		}
	}

	if (k == GLUT_KEY_LEFT)//if the left arrow is pressed, then the object will be translated in the x axis by -10. (moving left)
	{
		if (!rotateLeftp)
			player[1] -= 10;
		rotationAngleP = 10;
		player[0] -= 10;
		rotateLeftp = true;
		if (player[0] <= -25 + 10)
		{
			player[0] = -25 + 10;
		}
	}
	//if (k == GLUT_KEY_UP)//if the up arrow is pressed, then the object will be translated in the y axis by 10. (moving upwords)
	//	player[1] += 10;
	//if (k == GLUT_KEY_DOWN)//if the down arrow is pressed, then the object will be translated in the y axis by -10. (moving downwords)
	//	player[1] -= 10;

	glutPostRedisplay();//redisplay to update the screen with the changes
}

void speUp(int k, int x, int y)// keyboard special key function is called whenever the special key is released.
{
	if (rotationAngleP == 10)
		player[1] += 10;
	else if (rotationAngleP == -10)
		player[1] -= 10;
	rotationAngleP = 0;
	rotateLeftp = false;
	rotateRightp = false;
	if (k == GLUT_KEY_F1)//if the F1 key is released, then the object will return back to it's original color.
		speC = 0;
	glutPostRedisplay();//redisplay to update the screen with the changes
}

void key(unsigned char k, int x, int y)//keyboard function, takes 3 parameters
// k is the key pressed from the keyboard
// x and y are mouse postion when the key was pressed.
{
	if (k == 'd')//if the letter d is pressed, then the object will be translated in the x axis by 10 (moving to the right).
		player[0] += 10;
	if (k == 'a')//if the letter a is pressed, then the object will be translated in the x axis by -10 (moving to the left).
		player[0] -= 10;
	if (k == 'w')//if the letter w is pressed, then the object will be translated in the y axis by 10. (moving upwords)
		player[1] += 10;
	if (k == 's')//if the letter w is pressed, then the object will be translated in the y axis by -10. (moving downwords)
		player[1] -= 10;
	if (k == ' ')//if the letter b is pressed, the player fires
	{
		PlaySound(L"E:\\SEM7\\Computer Graphics\\CI\\Bomb+2.wav", 0, SND_FILENAME | SND_ASYNC);
		fire2 = true;
		//PlaySound((LPCWSTR)"E:\SEM7\Computer Graphics\CI\gun-cocking-01.wav", 0, SND_ASYNC | SND_FILENAME |[B]SND_FILENAME[/B]);
	}
	glutPostRedisplay();//redisplay to update the screen with the changes
}

void keyUp(unsigned char k, int x, int y)//keyboard up function is called whenever the keyboard key is released
{
	if (k == 'q')//if the letter q is pressed, then the object will go back to it's original color.
		keyC = 0;
	glutPostRedisplay();//redisplay to update the screen with the changed
}

void main(int argc, char** argr) {
	glutInit(&argc, argr);

	p0[0] = 5;
	p0[1] = 560;

	p1[0] = 995;
	p1[1] = 569;

	p2[0] = 5;
	p2[1] = 225;

	p3[0] = 995;
	p3[1] = 225;

	attacker[0] = 400;
	attacker[1] = 400;

	obs[0] = attacker[0];
	obs[1] = attacker[1];

	player[0] = 500;
	player[1] = 0;
	fire[0] = player[0];
	defender[0] = 0;
	defender[1] = 0;
	powerupTranslationY[0] = 1000;
	powerupTranslationY[1] = 1000;

	value = 100.0;
	max_value = 100.0;
	randPowerupTime = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (100)));
	powerupTimer = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (100)));
	glutInitWindowSize(1000, 600);
	//	glutInitWindowPosition(150, 150);
	glutCreateWindow("CI");

	glEnable(GL_TEXTURE_2D);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutDisplayFunc(Display);
	//glClearColor(0, 0, 0, 0);
	gluOrtho2D(0.0, 1000, 0.0, 600);
	glLineWidth(5.0);
	glutIdleFunc(Anim);
	glutKeyboardFunc(key);			//call the keyboard function
	glutKeyboardUpFunc(keyUp);		//call the keyboard up function
	glutSpecialFunc(spe);			//call the keyboard special keys function
	glutSpecialUpFunc(speUp);		//call the keyboard special keys up function
	glPushMatrix();
	loadBMP(&texID, "E:/SEM7/Computer Graphics/textures/cc.bmp", true);
	loadBMP(&texID + 1, "E:/SEM7/Computer Graphics/textures/cc.bmp", true);
	loadBMP(&texID + 2, "E:/SEM7/Computer Graphics/textures/metal.bmp", true);
	glPopMatrix();
	glutMainLoop();

}

void Display() {
	glEnable(GL_BLEND);
	glPushMatrix();
	glTranslated(x, y, 0);
	glBindTexture(GL_TEXTURE_2D, texID + 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(0, 10, 0);
	glTexCoord2f(rep, 0.0f);			glVertex3f(1000, 10, 0);
	glTexCoord2f(rep, rep);				 glVertex3f(1000, -1000, 0);
	glTexCoord2f(0.0f, rep);			 glVertex3f(0, -1000, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glEnable(GL_BLEND);
	glPushMatrix();
	glTranslated(x, y, 0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(0, 0, 0);
	glTexCoord2f(rep, 0.0f);			glVertex3f(1000, 0, 0);
	glTexCoord2f(rep, rep);				 glVertex3f(1000, 1000, 0);
	glTexCoord2f(0.0f, rep);			 glVertex3f(0, 1000, 0);
	glEnd();
	glDisable(GL_BLEND);

	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texID + 2);
	glPopMatrix();
	if (lives <= 0)
	{
		glPushMatrix();
		glColor4f(1.0f, 0.0f, 0.0f, 0.0f);//red
		char* p1s[20];
		sprintf_s((char *)p1s, 20, "YOU LOST !!!!!!!!");
		if (lose)
		{
			PlaySound(L"E:\\SEM7\\Computer Graphics\\CI\\maybe-next-time.wav", 0, SND_FILENAME | SND_SYNC);
			lose = false;
		}
		print(415, 540, (char *)p1s);
		glPopMatrix();
		destroy_window();
	}
	if (window_valid == -1 && destroy){
		return;
	}
	if (window_valid == -1){
		destroy = true;
	}
	//lives
	glPushMatrix();

	char* p0s[20];
	sprintf_s((char *)p0s, 20, "X %d", lives);
	print(45, 560, (char *)p0s);
	glBegin(GL_POLYGON);
	glVertex3f(20, 580, 0.0f);
	glVertex3f(5, 590, 0.0f);
	glVertex3f(20, 560, 0.0f);
	glVertex3f(35, 590, 0.0f);
	glEnd();
	glColor4f(1.0f, 0.0f, 0.0f, 0.0f);//red
	glPopMatrix();

	//score
	char* p1s[20];
	sprintf_s((char *)p1s, 20, "Score:  %d", score);
	print(745, 560, (char *)p1s);

	//attacker
	glPushMatrix();
	glTranslated(attacker[0], attacker[1], 0);
	glRotated(rotationAngleA, 0, 0, 1);
	glColor3f(215.0f, 50.0f, 0.0f);//Blue
	//head
	glBegin(GL_POLYGON);
	glVertex3f(35, 20, 0.0f);
	glVertex3f(35, 0, 0.0f);
	glVertex3f(50, -15, 0.0f);
	glVertex3f(65, 0, 0.0f);
	glVertex3f(65, 20, 0.0f);
	glEnd();
	//body
	glBegin(GL_QUADS);
	glVertex3f(20, -65, 0.0f);
	glVertex3f(80, -65, 0.0f);
	glVertex3f(80, -15, 0.0f);
	glVertex3f(20, -15, 0.0f);
	glEnd();
	glBegin(GL_TRIANGLES);
	//wing1
	glVertex3f(20, -65, 0.0f);
	glVertex3f(20, -15, 0.0f);
	glVertex3f(-50, 20, 0.0f);
	//wing2
	glVertex3f(80, -65, 0.0f);
	glVertex3f(80, -15, 0.0f);
	glVertex3f(150, 20, 0.0f);
	//leg1
	glVertex3f(30, -65, 0.0f);
	glVertex3f(35, -85, 0.0f);
	glVertex3f(40, -65, 0.0f);
	//leg2
	glVertex3f(60, -65, 0.0f);
	glVertex3f(65, -85, 0.0f);
	glVertex3f(70, -65, 0.0f);
	//horn1
	glVertex3f(35, 20, 0.0f);
	glVertex3f(40, 30, 0.0f);
	glVertex3f(45, 20, 0.0f);
	//horn2
	glVertex3f(65, 20, 0.0f);
	glVertex3f(60, 30, 0.0f);
	glVertex3f(55, 20, 0.0f);
	glEnd();
	glPopMatrix();

	//obstacles
	//drawCircle(obs[0], obs[1], 10);
	glPushMatrix();
	glColor3f(1.0f, 0.5f, 0.0f);//Orange
	glTranslated(obs[0], obs[1], 0);

	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(-10, 0);
	glVertex2f(-10, 10);
	glVertex2f(0, 10);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(-10, 0);
	glVertex2f(-20, -10);
	glVertex2f(-10, -20);
	glVertex2f(0, -20);
	glVertex2f(10, -10);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(0, -20);
	glVertex2f(-5, -30);
	glVertex2f(-10, -20);
	glEnd();
	glPopMatrix();

	//obstaclesD
	//drawCircle(obs[0], obs[1], 10);
	glPushMatrix();
	glColor3f(1.0f, 0.5f, 0.0f);//Orange
	glTranslated(obsD[0], obsD[1], 0);

	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(-10, 0);
	glVertex2f(-10, 10);
	glVertex2f(0, 10);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(-10, 0);
	glVertex2f(-20, -10);
	glVertex2f(-10, -20);
	glVertex2f(0, -20);
	glVertex2f(10, -10);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(0, -20);
	glVertex2f(-5, -30);
	glVertex2f(-10, -20);
	glEnd();
	glPopMatrix();

	//defender
	if (defenderToTheRescue)
	{
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);//Blue
		glTranslated(defender[0], defender[1], 0);
		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(100, 0);
		glVertex2f(100, 10);
		glVertex2f(0, 10);
		glEnd();
		glPopMatrix();
	}

	//progress bar
	glPushMatrix();
	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);//light blue
	glBegin(GL_QUADS);
	glVertex2f(300, 570);
	glVertex2f(300 + 400, 570);
	glVertex2f(300 + 400, 570 + 10);
	glVertex2f(300, 570 + 10);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor4f(1.0f, 0.0f, 0.0f, 0.0f);//red
	glBegin(GL_QUADS);
	glVertex2f(300, 570);
	glVertex2f(300 + progressBarWidth, 570);
	glVertex2f(300 + progressBarWidth, 570 + 10);
	glVertex2f(300, 570 + 10);
	glEnd();
	glPopMatrix();

	//fire

	if (fire2) {
		glPushMatrix();
		shoot();
		glPopMatrix();
	}
	//powerup1
	glPushMatrix();
	glTranslated(powerupTranslationX[0], powerupTranslationY[0], 0);
	glColor4f(1.0f, 0.0f, 0.0f, 0.0f);//red
	glBegin(GL_TRIANGLES);
	glVertex3f(50, 70, 0.0f);
	glVertex3f(80, 70, 0.0f);
	glVertex3f(65, 40, 0.0f);
	glVertex3f(65, 80, 0.0f);
	glVertex3f(50, 50, 0.0f);
	glVertex3f(80, 50.0f, 0.0f);
	glEnd();
	glPopMatrix();
	//powerup2
	glPushMatrix();
	glTranslated(powerupTranslationX[1], powerupTranslationY[1], 0);
	glColor4f(1.0f, 0.0f, 0.0f, 0.0f);//red
	glBegin(GL_POLYGON);
	glVertex3f(620, 80, 0.0f);
	glVertex3f(605, 90, 0.0f);
	glVertex3f(620, 60, 0.0f);
	glVertex3f(635, 90, 0.0f);
	glEnd();
	glPopMatrix();

	//player
	glColor4f(1, 1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslated(player[0], player[1], 0);
	glRotated(rotationAngleP, 0, 0, 1);
	//body
	glBegin(GL_POLYGON);
	glVertex3f(50.0f, 0.0f, 0.0f);
	glVertex3f(75.0f, 25.0f, 0.0f);
	glVertex3f(50.0f, 50.0f, 0.0f);
	glVertex3f(25, 25.0f, 0.0f);
	glEnd();
	//head
	glBegin(GL_TRIANGLES);
	glVertex3f(40, 50, 0.0f);
	glVertex3f(60, 50, 0.0f);
	glVertex3f(50, 60, 0.0f);
	glEnd();
	//antenna
	glBegin(GL_LINES);
	glVertex3f(25, 90, 0.0f);
	glVertex3f(30, 75, 0.0f);
	glVertex3f(30, 75, 0.0f);
	glVertex3f(50, 60, 0.0f);
	glVertex3f(50, 60, 0.0f);
	glVertex3f(75, 75, 0.0f);
	glVertex3f(75, 75, 0.0f);
	glVertex3f(80, 90, 0.0f);
	glEnd();
	//nosils
	glBegin(GL_QUADS);
	glVertex3f(20, 100.0f, 0.0f);
	glVertex3f(20, 90, 0.0f);
	glVertex3f(30, 90, 0.0f);
	glVertex3f(30, 100.0f, 0.0f);
	glVertex3f(75, 100.0f, 0.0f);
	glVertex3f(75, 90, 0.0f);
	glVertex3f(85, 90, 0.0f);
	glVertex3f(85, 100.0f, 0.0f);
	glEnd();
	glPopMatrix();
	glFlush();
}

void Anim(){
	randCurve++;
	//powerups dropping
	if (!powerupToggle)
	{
		if ((powerupTimer % 3 == 0))
		{
			powerupActivate = true;
			powerupTranslationY[0] = 1000;
			powerupTranslationY[1] --;
			if (powerupTranslationY[1] <= -100)
			{
				powerupTranslationY[1] = 1000;
				powerupToggle = true;
				randPowerupTime = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (100)));
				powerupActivate = false;
			}
		}
		else
		{
			randPowerupTime = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (100)));
			powerupTranslationX[1] = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (500)));
			powerupToggle = true;
		}
	}
	else if ((randPowerupTime % 5 == 0))
	{
		powerupActivate = true;
		powerupTranslationY[1] = 1000;
		powerupTranslationY[0] --;
		if (powerupTranslationY[0] <= -100)
		{
			powerupTranslationY[0] = 1000;
			powerupToggle = false;
			powerupTimer = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (100)));
			powerupActivate = false;
		}
	}
	else
	{
		powerupTimer = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (100)));
		powerupTranslationX[0] = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (500)));
		powerupToggle = false;
	}

	//obstacles dropping from attacker
	if (obsCount % 1000 == 0 && !obsAttacker)
	{
		obs[0] = attacker[0];
		obs[1] = attacker[1];
		obsAttacker = true;
	}
	if (obsAttacker){
		if (obs[1] <= -10)
			obsAttacker = false;
		obs[1] --;
	}
	//obstacles dropping from defender
	if (obsCount % 2000 == 0 && !obsDefender)
	{
		obsD[0] = defender[0];
		obsD[1] = defender[1];
		obsDefender = true;
	}
	if (obsDefender){
		if (obsD[1] <= -10)
			obsDefender = false;
		obsD[1] --;
	}
	attackerOld[0] = attacker[0];
	attackerOld[1] = attacker[1];
	//attacker's new location
	attacker[0] = bezier(t, p0, p1, p2, p3)[0];
	attacker[1] = bezier(t, p0, p1, p2, p3)[1];

	float slope = (attacker[1] - attackerOld[1]) / (attacker[0] - attackerOld[0]);
	rotationAngleA = atan(slope);
	//defender's movements
	if (defenderToTheRescue)
	{
		defender[1] = 205;
		if (!defRev)
			defender[0] += 1;
		else
			defender[0] -= 1;

		if (defender[0] >= 1000 - 100)
			defRev = true;
		else if (defender[0] <= 0)
			defRev = false;
	}
	//randomization for new attacker's location
	//if (randMotion % 200 == 0)
	//{
	//	//srand(static_cast <unsigned> (time(0)));
	//	/*srand(time(NULL));*/
	//	r1 = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1)));
	//	rm = true;
	//}
	//if (rm)
	//{
	//	if (t > r1)
	//		rev = true;
	//	else
	//		rev = false;
	//	rm = false;
	//}
	if (t >= 1)
	{
		rev = true;
		rotationAngleA = 0;
	}
	if (rev)
	{
		rotationAngleA = 10;
		t -= 0.001;
		if (t <= 0)
		{
			rotationAngleA = 0;
			rev = false;
			t = 0;
			p2[0] = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1000)));
			p3[0] = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1000)));
		}
	}
	else
	{
		rotationAngleA = -10;
		t += 0.001;
	}
	randMotion++;

	//fire
	if (fire2) {
		if (initf)
		{
			if (!fireToggle)
			{
				fire[0] = player[0] + 25;
				fire[1] = 100;
				fireToggle = true;

			}
			else
			{
				fire[0] = player[0] + 80;
				fire[1] = 100;
				fireToggle = false;
			}
			initf = false;
		}
		if ((fire[0] <= defender[0] + 100 && fire[0] >= defender[0]) && (fire[1] <= defender[1] + 10 && fire[1] >= defender[1]) && defenderToTheRescue){
			fire[1] = player[1];
			fire2 = false;
			initf = true;
		}
		else if ((fire[0] <= attacker[0] + 100 && fire[0] >= attacker[0]) && (fire[1] <= attacker[1] + 50 && fire[1] >= attacker[1] - 50)){
			PlaySound(L"E:\\SEM7\\Computer Graphics\\CI\\pain.wav", 0, SND_FILENAME | SND_ASYNC);
			value -= 10;
			if (max_value / value >= 2)
				defenderToTheRescue = true;
			fire[1] = player[1];
			fire2 = false;
			initf = true;
			if (value <= 0){
				max_value /= 0.2;
				value = max_value;
				score++;
				defender[0] = 0;
				defender[1] = 0;
				defenderToTheRescue = false;
				PlaySound(L"E:\\SEM7\\Computer Graphics\\CI\\kill.wav", 0, SND_FILENAME | SND_ASYNC);
			}

		}
		else if (fire[1] >= 1000)
		{
			fire[1] = player[1];
			fire2 = false;
			initf = true;
			fire[0] = player[0];
		}
		else
			fire[1] += 2;

	}
	//obstacles hitting player
	if (obsAttacker)
		if ((obs[0] <= player[0] + 75 && obs[0] >= player[0] + 25) && (obs[1] <= player[1] + 25 && obs[1] >= player[1]))
		{
			lives--;
			if (lives <= 0)
				obs[0] = 0;
			obs[1] = 0;
		}
	if (obsDefender)
		if ((obsD[0] <= player[0] + 75 && obsD[0] >= player[0] + 25) && (obsD[1] <= player[1] + 25 && obsD[1] >= player[1]))
		{
			lives--;
			if (lives <= 0)
				obsD[0] = 0;
			obsD[1] = 0;
		}
	//powerups acquiring
	if (powerupActivate)
	{
		if ((powerupTranslationX[0] + 65 <= player[0] + 75 && powerupTranslationX[0] + 65 >= player[0] + 25) && (powerupTranslationY[0] + 40 <= player[1] + 25 && powerupTranslationY[0] + 40 >= player[1]))
		{
			PlaySound(L"E:\\SEM7\\Computer Graphics\\CI\\pain.wav", 0, SND_FILENAME | SND_ASYNC);
			value -= 5;
			if (max_value / value >= 2)
				defenderToTheRescue = true;
			if (value <= 0)
			{
				max_value /= 0.2;
				value = max_value;
				score++;
				defenderToTheRescue = false;
				PlaySound(L"E:\\SEM7\\Computer Graphics\\CI\\kill.wav", 0, SND_FILENAME | SND_ASYNC);
			}
			powerupTranslationY[0] = 1000;
		}
		if ((powerupTranslationX[1] + 620 <= player[0] + 75 && powerupTranslationX[1] + 620 >= player[0] + 25) && (powerupTranslationY[1] + 60 <= player[1] + 25 && powerupTranslationY[1] + 60 >= player[1]))
		{
			lives++;
			powerupTranslationY[1] = 1000;
		}
		powerupActivate = false;
	}
	progress = value / max_value;
	progressBarWidth = progress * 400;

	obsCount++;
	defenderToTheRescueT++;
	y += .1;
	if (y >= 1000)
		y = 0;
	glutPostRedisplay();
}