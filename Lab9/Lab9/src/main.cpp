// PUT YOUR NAME AND STUDENT NUMBER HERE //

#define GLEW_STATIC
#include "glew/glew.h"
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "FreeImage.lib")

#include "SkinnedGameObject.h"
#include "BlendTree.h"
#include "Transform.h"
#include "AnimationMath.h"
#include "ParticleEmitter.h"
#include "GUIFuncs.h"

// Core Libraries (std::)
#include <iostream>
#include <string>
#include <math.h>
#include <vector>

// 3rd Party Libraries
#include <GLUT/freeglut.h>
#include <TTK/GraphicsUtils.h>
#include <TTK/Camera.h>
#include "imgui/imgui.h"
#include <TTK/OBJMesh.h>

struct Input
{
	bool cameraLeft = false;
	bool cameraRight = false;
	bool cameraForward = false;
	bool cameraBackward = false;
	bool cameraUp = false;
	bool cameraDown = false;
}; Input input;

// Defines and Core variables
#define FRAMES_PER_SECOND 60
const int FRAME_DELAY = 1000 / FRAMES_PER_SECOND; // Milliseconds per frame

// Window size
int windowWidth = 800;
int windowHeight = 600;

// Angle conversions
const float degToRad = 3.14159f / 180.0f;
const float radToDeg = 180.0f / 3.14159f;

float deltaTime = 0.0f; // amount of time since last update (set every frame in timer callback)

// Mouse position in pixels
glm::vec3 mousePosition; // x, y, 0
glm::vec3 mousePositionFlipped; // x, windowHeight - y, 0
TTK::Camera camera;

BlendTree blend;
float interpVal = 0.0f;
//SkinnedGameObject skinnedGameObject;
std::string animationPath = "../../assets/animations/HTR/";

//Used for moving the player at the right position
glm::vec3 currentRootPosition;

SkinnedGameObject interpSkeleton;
SkinnedGameObject PrevAnim;
SkinnedGameObject Idle;
SkinnedGameObject WalkForward;
SkinnedGameObject WalkForLeft;
SkinnedGameObject WalkForRight;
SkinnedGameObject WalkBackward;
SkinnedGameObject WalkBackLeft;
SkinnedGameObject WalkBackRight;
SkinnedGameObject ShuffleLeft;
SkinnedGameObject ShuffleRight;
SkinnedGameObject Jump;
SkinnedGameObject Strong;
SkinnedGameObject Weak;
bool isWalkFor;
bool isWalkForLeft;
bool isWalkForRight;
bool isWalkBack;
bool isWalkBackLeft;
bool isWalkBackRight;
bool isShuffleLeft;
bool isShuffleRight;
bool isJump;
bool isStrong;
bool isWeak;
bool loopPlayed = false;

//Key pressed bools
bool pressUp = false;
bool pressDown = false;
bool pressLeft = false;
bool pressRight = false;

void InitializeScene()
{
	//std::string animationPath = "../../assets/animations/HTR/";
	std::string modelsPath = "../../assets/models/";

	/*skinnedGameObject.animFiles = {
		animationPath + "Idle.htr",
		animationPath + "Jump.htr",
		animationPath + "ShuffleLeft.htr",
		animationPath + "ShuffleRight.htr",
		animationPath + "WalkBackLeft.htr",
		animationPath + "WalkBackRight.htr",
		animationPath + "WalkBackward.htr",
		animationPath + "WalkForward.htr",
		animationPath + "WalkForwardLeft.htr",
		animationPath + "WalkForwardRight.htr",
		animationPath + "StrongAttack.htr",
		animationPath + "WeakAttack.htr",
	};*/

	Idle.name = "root";
	//skinnedGameObject.initializeSkeletonFromHTR(animationPath + "simple_rig.htr", "", nullptr);
	/*for (int i = 0; i < skinnedGameObject.animFiles.size(); i++)
	{
		skinnedGameObject.initializeSkeletonFromHTR(skinnedGameObject.animFiles[i], "", nullptr);
	}*/
	Idle.initializeSkeletonFromHTR(animationPath + "Idle.htr", "", nullptr);
	Idle.setLocalRotationAngleX(-90);

	WalkForward.name = "walk";
	WalkForward.initializeSkeletonFromHTR(animationPath + "WalkForward.htr", "", nullptr);
	WalkForward.setLocalRotationAngleX(-90);

	WalkForLeft.initializeSkeletonFromHTR(animationPath + "WalkForwardLeft.htr", "", nullptr);
	WalkForLeft.setLocalRotationAngleX(-90);

	WalkForRight.initializeSkeletonFromHTR(animationPath + "WalkForwardRight.htr", "", nullptr);
	WalkForRight.setLocalRotationAngleX(-90);

	WalkBackward.initializeSkeletonFromHTR(animationPath + "WalkBackward.htr", "", nullptr);
	WalkBackward.setLocalRotationAngleX(-90);

	WalkBackLeft.initializeSkeletonFromHTR(animationPath + "WalkBackLeft.htr", "", nullptr);
	WalkBackLeft.setLocalRotationAngleX(-90);

	WalkBackRight.initializeSkeletonFromHTR(animationPath + "WalkBackRight.htr", "", nullptr);
	WalkBackRight.setLocalRotationAngleX(-90);

	ShuffleLeft.initializeSkeletonFromHTR(animationPath + "ShuffleLeft.htr", "", nullptr);
	ShuffleLeft.setLocalRotationAngleX(-90);

	ShuffleRight.initializeSkeletonFromHTR(animationPath + "ShuffleRight.htr", "", nullptr);
	ShuffleRight.setLocalRotationAngleX(-90);

	Jump.initializeSkeletonFromHTR(animationPath + "Jump.htr", "", nullptr);
	Jump.setLocalRotationAngleX(-90);

	Strong.initializeSkeletonFromHTR(animationPath + "StrongAttack.htr", "", nullptr);
	Strong.setLocalRotationAngleX(-90);

	Weak.initializeSkeletonFromHTR(animationPath + "WeakAttack.htr", "", nullptr);
	Weak.setLocalRotationAngleX(-90);

	PrevAnim = Idle;

	//skinnedGameObject.loadSkeletonFromHTR(skinnedGameObject.animFiles[0]);
	//skinnedGameObject.initializeSkeletonFromHTR(animationPath + "Idle.htr", "", nullptr);
	//skinnedGameObject.initializeSkeletonFromHTR(animationPath + "Jump1.htr", "", nullptr);
}

void drawHierarchyUI();

void ProcessInput()
{
	if (input.cameraUp)
		camera.moveUp();
	if (input.cameraDown)
		camera.moveDown();
	if (input.cameraForward)
		camera.moveForward();
	if (input.cameraBackward)
		camera.moveBackward();
	if (input.cameraRight)
		camera.moveRight();
	if (input.cameraLeft)
		camera.moveLeft();
}

void Interpolation()
{

}

// This is where we draw stuff
void DisplayCallbackFunction(void)
{
	ProcessInput();
	// Set up scene
	TTK::Graphics::SetBackgroundColour(0.5f, 0.5f, 0.5f);
	TTK::Graphics::ClearScreen();
	TTK::Graphics::SetCameraMode3D(windowWidth, windowHeight);
	TTK::Graphics::DrawGrid();
	camera.update();

	// Update the GameObjects as necessary
	if (isWalkFor)
	{
		for (float i = 0.5f; i >= interpVal; i -= 0.1f)
		{
			//std::cout << i << std::endl;
			glm::quat IdleQuat = blend.RotationToQuaternion(Idle.getWorldRotation());
			glm::quat WalkForQaut = blend.RotationToQuaternion(WalkForward.getWorldRotation());
			glm::quat SlerpQuat = blend.Slerp(IdleQuat, WalkForQaut, i);
			interpSkeleton = Idle;
			interpSkeleton.m_pLocalRotation = blend.QuaternionToRotation(SlerpQuat);
			//interpSkeleton.update(deltaTime);
			//interpSkeleton.draw();
		}

		WalkForward.update(deltaTime);

		WalkForward.draw();
	}
	else if (isWalkForLeft)
	{
		WalkForLeft.update(deltaTime);

		WalkForLeft.draw();
	}
	else if (isWalkForRight)
	{
		WalkForRight.update(deltaTime);

		WalkForRight.draw();
	}
	else if (isWalkBack)
	{
		WalkBackward.update(deltaTime);

		WalkBackward.draw();
	}
	else if (isWalkBackLeft)
	{
		WalkBackLeft.update(deltaTime);

		WalkBackLeft.draw();
	}
	else if (isWalkBackRight)
	{
		WalkBackRight.update(deltaTime);

		WalkBackRight.draw();
	}
	else if (isShuffleLeft)
	{
		ShuffleLeft.update(deltaTime);

		ShuffleLeft.draw();
	}
	else if (isShuffleRight)
	{
		ShuffleRight.update(deltaTime);

		ShuffleRight.draw();
	}
	else if (isJump)
	{
		Jump.update(deltaTime);

		Jump.draw();
	}
	else if (isStrong)
	{
		Strong.update(deltaTime);

		Strong.draw();
	}
	else if (isWeak)
	{
		Weak.update(deltaTime);

		Weak.draw();
	}
	else
	{
		Idle.update(deltaTime);

		Idle.draw();
	}

	// Draw UI
	// You must call this prior to using any imgui functions
	TTK::Graphics::StartUI(windowWidth, windowHeight);
	ImGui::Begin("Inspector");

	drawHierarchyUI();

	//ImGui::Checkbox("Play Animation", &skinnedGameObject.playing);

	// You must call this once you are done doing UI stuff
	// This is what actually draws the ui on screen
	ImGui::End();
	TTK::Graphics::EndUI();

	// Swap buffers
	// This is how we tell the program to put the things we just drew on the screen
	glutSwapBuffers();
}

void drawHierarchyUI()
{
	ImGui::Text("Hierarchy");
	ImGui::Separator();

	Transform* selectedObject;
	static Transform* previousSelectedObject;

	if (isWalkFor)
	{
		selectedObject = drawHierarchyUI(&WalkForward);
		previousSelectedObject = selectedObject;
	}
	/*else if (isWalkFor)
	{
		selectedObject = drawHierarchyUI(&WalkForward);
		previousSelectedObject = selectedObject;
	}*/
	else
	{
		selectedObject = drawHierarchyUI(&Idle);
		previousSelectedObject = selectedObject;
	}

	if (selectedObject != nullptr)
	{
		ImGui::Text("Selected Object Properties");
		ImGui::Separator();

		ImGui::Text("Selected Object: %s", selectedObject->name.c_str());
		ImGui::Text("Local Position: %s", glm::to_string(selectedObject->getLocalPosition()).c_str());
		ImGui::Text("World Position: %s", glm::to_string(selectedObject->getWorldPosition()).c_str());
		
		selectedObject->colour = glm::vec4(0, 1, 1, 1);
		glm::vec3 pos = selectedObject->getLocalPosition();
		glm::vec3 rot = selectedObject->getLocalRotationAngles();
		glm::vec3 scale = selectedObject->getLocalScale();
		ImGui::DragFloat3("Position", &pos[0]);
		ImGui::DragFloat3("Rotation", &rot[0]);
		ImGui::DragFloat3("Scale", &scale[0]);

		selectedObject->setLocalPosition(pos);
		selectedObject->setLocalScale(scale);
		selectedObject->setLocalRotationAngleX(rot.x);
		selectedObject->setLocalRotationAngleY(rot.y);
		selectedObject->setLocalRotationAngleZ(rot.z);
	}

	if (previousSelectedObject != nullptr)
	{
		if (previousSelectedObject != selectedObject)
			previousSelectedObject->colour = glm::vec4(1);
	}

	previousSelectedObject = selectedObject;
}

/* function void KeyboardCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is pressed
*/
void KeyboardCallbackFunction(unsigned char key, int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[(int)key] = true;
	io.AddInputCharacter((int)key); // this is what makes keyboard input work in imgui
	
	switch (key)
	{
	case 27: // the escape key
		glutExit();
		break;
	case 'q': // the 'q' key
	case 'Q':
		input.cameraUp = true;
		break;
	case 'e':
	case 'E':
		input.cameraDown = true;
		break;
	case 'W':
	case 'w':
		input.cameraForward = true;
		break;
	case 'S':
	case 's':
		input.cameraBackward = true;
		break;
	case 'A':
	case 'a':
		input.cameraRight = true;
		break;
	case 'D':
	case 'd':
		input.cameraLeft = true;
		break;
	case 32:
		//WalkForward.initializeSkeletonFromHTR(skinnedGameObject.animFiles[1], "", nullptr);
		isJump = true;
		break;

		//skinnedGameObject.loadSkeletonFromHTR(skinnedGameObject.animFiles[1]);
	}

	// This is what makes the backspace button work
	int keyModifier = glutGetModifiers();
	switch (keyModifier)
	{
	case GLUT_ACTIVE_SHIFT:
		io.KeyShift = true;
		break;

	case GLUT_ACTIVE_CTRL:
		io.KeyCtrl = true;
		break;

	case GLUT_ACTIVE_ALT:
		io.KeyAlt = true;
		break;
	}
}

/* function void KeyboardUpCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is lifted
*/
void KeyboardUpCallbackFunction(unsigned char key, int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = false;

	switch (key)
	{
	case 'q': // the 'q' key
	case 'Q':
		input.cameraUp = false;
		break;
	case 'e':
	case 'E':
		input.cameraDown = false;
		break;
	case 'W':
	case 'w':
		input.cameraForward = false;
		break;
	case 'S':
	case 's':
		input.cameraBackward = false;
		break;
	case 'A':
	case 'a':
		input.cameraRight = false;
		break;
	case 'D':
	case 'd':
		input.cameraLeft = false;
		break;
	case 32:
		isJump = false;
		break;
	default:
		break;
	}

	int keyModifier = glutGetModifiers();
	io.KeyShift = false;
	io.KeyCtrl = false;
	io.KeyAlt = false;
	switch (keyModifier)
	{
	case GLUT_ACTIVE_SHIFT:
		io.KeyShift = true;
		break;

	case GLUT_ACTIVE_CTRL:
		io.KeyCtrl = true;
		break;

	case GLUT_ACTIVE_ALT:
		io.KeyAlt = true;
		break;
	}
}

/* function TimerCallbackFunction(int value)
* Description:
*  - this is called many times per second
*  - this enables you to animate things
*  - no drawing, just changing the state
*  - changes the frame number and calls for a redisplay
*  - FRAME_DELAY is the number of milliseconds to wait before calling the timer again
*/
void TimerCallbackFunction(int value)
{
	// Calculate the amount of time since the last frame
	static int elapsedTimeAtLastTick = 0;
	int totalElapsedTime = glutGet(GLUT_ELAPSED_TIME);

	deltaTime = (float)totalElapsedTime - elapsedTimeAtLastTick;
	deltaTime /= 1000.0f;
	elapsedTimeAtLastTick = totalElapsedTime;

	// Re-trigger the display event
	glutPostRedisplay();

	/* this call gives it a proper frame delay to hit our target FPS */
	glutTimerFunc(FRAME_DELAY, TimerCallbackFunction, 0);
}

/* function WindowReshapeCallbackFunction()
* Description:
*  - this is called whenever the window is resized
*  - and sets up the projection matrix properly
*/
void WindowReshapeCallbackFunction(int w, int h)
{
	/* Update our Window Properties */
	windowWidth = w;
	windowHeight = h;
}

// This is called when a mouse button is clicked
void MouseClickCallbackFunction(int button, int state, int x, int y)
{
	ImGui::GetIO().MouseDown[0] = state;

	mousePosition.x = x;
	mousePosition.y = y;

	mousePositionFlipped.x = x;
	mousePositionFlipped.y = windowHeight - y;
}

void SpecialInputCallbackFunction(int key, int x, int y)
{
	float movementSpeed = 2.5f; // how fast should the object move
	//glm::vec3 currentRootPosition;// = PrevAnim.getLocalPosition();
	if(isWalkFor)
		currentRootPosition = WalkForward.getLocalPosition();
	else if (isWalkForLeft)
		currentRootPosition = WalkForLeft.getLocalPosition();
	else if (isWalkForRight)
		currentRootPosition = WalkForRight.getLocalPosition();
	else if (isWalkBack)
		currentRootPosition = WalkBackward.getLocalPosition();
	else if (isWalkBackLeft)
		currentRootPosition = WalkBackLeft.getLocalPosition();
	else if (isWalkBackRight)
		currentRootPosition = WalkBackRight.getLocalPosition();
	else if (isShuffleLeft)
		currentRootPosition = ShuffleLeft.getLocalPosition();
	else if (isShuffleRight)
		currentRootPosition = ShuffleRight.getLocalPosition();
	/*else if (isJump)
		currentRootPosition = Jump.getLocalPosition();
	else if (isStrong)
		currentRootPosition = Strong.getLocalPosition();
	else if (isWeak)
		currentRootPosition = Weak.getLocalPosition();
	else
		currentRootPosition = Idle.getLocalPosition();*/

	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = true;

	switch (key)
	{
	case GLUT_KEY_UP:
		//pressUp = true;
		isWalkFor = true;
		//Idle.addChild(&WalkForward);
		currentRootPosition.x -= movementSpeed * deltaTime;
		break;
	case GLUT_KEY_DOWN:
		isWalkBack = true;
		currentRootPosition.x += movementSpeed * deltaTime;
		break;
	case GLUT_KEY_LEFT:
		isShuffleLeft = true;
		currentRootPosition.z += movementSpeed * deltaTime;
		break;
	case GLUT_KEY_RIGHT:
		isShuffleRight = true;
		currentRootPosition.z -= movementSpeed * deltaTime;
		break;
	case GLUT_KEY_SHIFT_L:
		isWeak = true;
		break;
	case GLUT_KEY_CTRL_L:
		isStrong = true;
		break;
	}

	if (isWalkFor == true && isShuffleRight == true)
	{
		isWalkForRight = true;
		currentRootPosition.x -= movementSpeed * deltaTime;
		currentRootPosition.z -= movementSpeed * deltaTime;
	}
	if (isWalkFor == true && isShuffleLeft == true)
	{
		isWalkForLeft = true;
		currentRootPosition.x -= movementSpeed * deltaTime;
		currentRootPosition.z += movementSpeed * deltaTime;
	}
	if (isWalkBack == true && isShuffleRight == true)
	{
		isWalkBackRight = true;
		currentRootPosition.x += movementSpeed * deltaTime;
		currentRootPosition.z -= movementSpeed * deltaTime;
	}
	if (isWalkBack == true && isShuffleLeft == true)
	{
		isWalkBackLeft = true;
		currentRootPosition.x += movementSpeed * deltaTime;
		currentRootPosition.z += movementSpeed * deltaTime;
	}

	//if (isWalkFor)
		WalkForward.setLocalPosition(currentRootPosition);
	//else if (isWalkForLeft)
		WalkForLeft.setLocalPosition(currentRootPosition);
	//else if (isWalkForRight)
		WalkForRight.setLocalPosition(currentRootPosition);
	//else if (isWalkBack)
		WalkBackward.setLocalPosition(currentRootPosition);
	//else if (isWalkBackLeft)
		WalkBackLeft.setLocalPosition(currentRootPosition);
	//else if (isWalkBackRight)
		WalkBackRight.setLocalPosition(currentRootPosition);
	//else if (isShuffleLeft)
		ShuffleLeft.setLocalPosition(currentRootPosition);
	//else if (isShuffleRight)
		ShuffleRight.setLocalPosition(currentRootPosition);
	//else if (isJump)
		Jump.setLocalPosition(currentRootPosition);
	//else if (isStrong)
		Strong.setLocalPosition(currentRootPosition);
	//else if (isWeak)
		Weak.setLocalPosition(currentRootPosition);
	//else
		Idle.setLocalPosition(currentRootPosition);
}
//glm::vec3 currentRootPosition;
void SpecialInputUpCallbackFunction(int key, int x, int y)
{
	float movementSpeed = 2.5f; // how fast should the object move
	// = PrevAnim.getLocalPosition();
	if (isWalkFor)
		currentRootPosition = WalkForward.getLocalPosition();
	else if (isWalkForLeft)
		currentRootPosition = WalkForLeft.getLocalPosition();
	else if (isWalkForRight)
		currentRootPosition = WalkForRight.getLocalPosition();
	else if (isWalkBack)
		currentRootPosition = WalkBackward.getLocalPosition();
	else if (isWalkBackLeft)
		currentRootPosition = WalkBackLeft.getLocalPosition();
	else if (isWalkBackRight)
		currentRootPosition = WalkBackRight.getLocalPosition();
	else if (isShuffleLeft)
		currentRootPosition = ShuffleLeft.getLocalPosition();
	else if (isShuffleRight)
		currentRootPosition = ShuffleRight.getLocalPosition();
	/*else if (isJump)
		currentRootPosition = Jump.getLocalPosition();
	else if (isStrong)
		currentRootPosition = Strong.getLocalPosition();
	else if (isWeak)
		currentRootPosition = Weak.getLocalPosition();
	else
		currentRootPosition = Idle.getLocalPosition();*/

	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = false;
	std::cout << "hi";
	switch (key)
	{
	case GLUT_KEY_UP:
		isWalkFor = false;
		PrevAnim = WalkForward;
		//Idle.removeChild(&WalkForward);
		currentRootPosition.x -= movementSpeed * deltaTime;
		break;
	case GLUT_KEY_DOWN:
		isWalkBack = false;
		PrevAnim = WalkBackward;
		currentRootPosition.x += movementSpeed * deltaTime;
		break;
	case GLUT_KEY_LEFT:
		isShuffleLeft = false;
		PrevAnim = ShuffleLeft;
		currentRootPosition.z += movementSpeed * deltaTime;
		break;
	case GLUT_KEY_RIGHT:
		isShuffleRight = false;
		PrevAnim = ShuffleRight;
		currentRootPosition.z -= movementSpeed * deltaTime;
		break;
	case GLUT_KEY_SHIFT_L:
		isWeak = false;
		break;
	case GLUT_KEY_CTRL_L:
		isStrong = false;
		break;
	}

	if (isWalkFor == false && isShuffleRight == false)
		isWalkForRight = false;
	if (isWalkFor == false && isShuffleLeft == false)
		isWalkForLeft = false;
	if (isWalkBack == false && isShuffleRight == false)
		isWalkBackRight = false;
	if (isWalkBack == false && isShuffleLeft == false)
		isWalkBackLeft = false;

	//if (isWalkFor)
		WalkForward.setLocalPosition(currentRootPosition);
	//else if (isWalkForLeft)
		WalkForLeft.setLocalPosition(currentRootPosition);
	//else if (isWalkForRight)
		WalkForRight.setLocalPosition(currentRootPosition);
	//else if (isWalkBack)
		WalkBackward.setLocalPosition(currentRootPosition);
	//else if (isWalkBackLeft)
		 WalkBackLeft.setLocalPosition(currentRootPosition);
	//else if (isWalkBackRight)
		WalkBackRight.setLocalPosition(currentRootPosition);
	//else if (isShuffleLeft)
		ShuffleLeft.setLocalPosition(currentRootPosition);
	//else if (isShuffleRight)
		ShuffleRight.setLocalPosition(currentRootPosition);
	//else if (isJump)
		Jump.setLocalPosition(currentRootPosition);
	//else if (isStrong)
		Strong.setLocalPosition(currentRootPosition);
	//else if (isWeak)
		Weak.setLocalPosition(currentRootPosition);
	//else
		Idle.setLocalPosition(currentRootPosition);
}

// Called when the mouse is clicked and moves
void MouseMotionCallbackFunction(int x, int y)
{

	if (mousePosition.length() > 0 && !ImGui::GetIO().WantCaptureMouse) // wantCaptureMouse is true if the mouse is within an imgui element
		camera.processMouseMotion(x, y, mousePosition.x, mousePosition.y, deltaTime);

	ImGui::GetIO().MousePos = ImVec2((float)x, (float)y);

	mousePosition.x = x;
	mousePosition.y = y;

	mousePositionFlipped.x = x;
	mousePositionFlipped.y = windowHeight - y;
}

// Called when the mouse is moved inside the window
void MousePassiveMotionCallbackFunction(int x, int y)
{
	ImGui::GetIO().MousePos = ImVec2((float)x, (float)y);

	mousePositionFlipped.x = x;
	mousePositionFlipped.y = windowHeight - y;
}

/* function main()
* Description:
*  - this is the main function
*  - does initialization and then calls glutMainLoop() to start the event handler
*/
int main(int argc, char **argv)
{
	/* initialize the window and OpenGL properly */

	// Request an OpenGL 4.4 compatibility
	// A compatibility context is needed to use the provided rendering utilities 
	// which are written in OpenGL 1.1
	glutInitContextVersion(4, 4); 
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Tutorial");

	//Init GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "TTK::InitializeTTK Error: GLEW failed to init" << std::endl;
	}
	printf("OpenGL version: %s, GLSL version: %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	/* set up our function callbacks */
	glutDisplayFunc(DisplayCallbackFunction);
	glutKeyboardFunc(KeyboardCallbackFunction);
	glutKeyboardUpFunc(KeyboardUpCallbackFunction);
	glutReshapeFunc(WindowReshapeCallbackFunction);
	glutMouseFunc(MouseClickCallbackFunction);
	glutMotionFunc(MouseMotionCallbackFunction);
	glutPassiveMotionFunc(MousePassiveMotionCallbackFunction);
	glutTimerFunc(1, TimerCallbackFunction, 0);
	glutSpecialFunc(SpecialInputCallbackFunction);
	glutSpecialUpFunc(SpecialInputUpCallbackFunction);

	// Init GL
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	/* Start Game Loop */
	deltaTime = (float)glutGet(GLUT_ELAPSED_TIME);
	deltaTime /= 1000.0f;

	InitializeScene();

	// Init IMGUI
	TTK::Graphics::InitImGUI();

	glutMainLoop();

	return 0;
}