#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
	delete updateTimer;

	PassThrough.UnLoad();
	Monkey.Unload();
	GrassTex.Unload();
}

void Game::initializeGame()
{
	updateTimer = new Timer();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	if (!PassThrough.Load("./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/PassThrough.frag"))
	{
		std::cout << "Shaders failed to initialize.\n";
		system("pause");
		exit(0);
	}

	if (!Monkey.LoadFromFile("./Assets/Models/Monkey.obj"))
	{
		std::cout << "Model failed ot load.\n";
		system("pause");
		exit(0);
	}

	if (!GrassTex.LoadFromFile("./Assets/Textures/Grass.png"))
	{
		std::cout << "Model failed ot load.\n";
		system("pause");
		exit(0);

	}
	CameraTransform.Translate(vec3(0.0f, 0.0f, 5.0f));
	CameraProjection = mat4::PerspectiveProjection(60.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 10000.0f);
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	MonkeyTransform.RotateY(deltaTime * 45.0f);
}

void Game::draw()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	PassThrough.Bind();

	PassThrough.SendUniformMat4("uModel", MonkeyTransform.data, true);
	PassThrough.SendUniformMat4("uView", CameraTransform.GetInverse().data, true);
	PassThrough.SendUniformMat4("uProj", CameraProjection.data, true);

	PassThrough.SendUniform("uTex", 0);
	PassThrough.SendUniform("LightPosition", CameraTransform.GetInverse()* vec4(4.0f, 0.0f, 0.0f, 1.0f));
	PassThrough.SendUniform("LightAmbient", vec3(0.15f, 0.15f, 0.15f));
	PassThrough.SendUniform("LightDiffuse", vec3(0.7f, 0.1f, 0.2f));
	PassThrough.SendUniform("LightSpecular", vec3(0.8f, 0.1f, 0.1f));
	PassThrough.SendUniform("LightSpecularExponent", 50.0f);
	PassThrough.SendUniform("Attenuation_Constant", 1.0f);
	PassThrough.SendUniform("Attenuation_Linear", 0.1f);
	PassThrough.SendUniform("Attenuation_Quadratic", 0.01f);


	GrassTex.Bind();

	glBindVertexArray(Monkey.VAO);
	glDrawArrays(GL_TRIANGLES, 0, Monkey.GetNumVertices());
	glBindVertexArray(0);

	GrassTex.UnBind();

	PassThrough.UnBind();

	glutSwapBuffers();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN) 
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:

			break;
		case GLUT_RIGHT_BUTTON:
		
			break;
		case GLUT_MIDDLE_BUTTON:

			break;
		}
	}
	else
	{

	}
}

/*
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus 
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Game::mouseMoved(int x, int y)
{
}
