#pragma once
#include <windows.h>

//Must be included before the other includes since these include glew.h and its picky
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Timer.h"

#define WINDOW_WIDTH			800
#define WINDOW_HEIGHT			600
#define FRAMES_PER_SECOND		60

class Game
{
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	/* Data Members */
	Timer *updateTimer	= nullptr;
	float TotalGameTime = 0.0f;

	Mesh Monkey;
	ShaderProgram PassThrough;
	Texture GrassTex;

	mat4 CameraTransform;
	mat4 CameraProjection;
	mat4 MonkeyTransform;

private:

};
