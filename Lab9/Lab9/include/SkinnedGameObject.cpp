#include "SkinnedGameObject.h"
#include <GLM/glm.hpp>
#include <GLM\gtx\transform.hpp>
#include "HTRLoader.h"
#include <iostream>
#include <GLEW\glew.h>
#include <tinyxml2\tinyxml2.h>
#include "TTK/GraphicsUtils.h"
#include "GLM/gtx/string_cast.hpp"

SkinnedGameObject::SkinnedGameObject()
	: m_pCurrentFrame(0),
	m_pHTRAnimation(nullptr),
	m_pJointAnimation(nullptr),
	playing(true),
	m_pJointToBindMat(glm::mat4(1.0f))
{
}

SkinnedGameObject::~SkinnedGameObject()
{
	delete m_pHTRAnimation;
	delete m_pJointAnimation;
}

void SkinnedGameObject::initializeSkeletonFromHTR(std::string htrFilePath, std::string skinWeightFilePath, std::shared_ptr<TTK::OBJMesh> bindMesh)
{
	// Load the HTR data
	m_pHTRAnimation = new HTRLoader();
	m_pHTRAnimation->loadHTR(htrFilePath); // DONE

	// Create GameObjects from the data
	m_pHTRAnimation->createGameObjects();  // DONE

	// Add the root node from the HTR file as a child to
	// this GameObject. This allows us to control the locomotion of the hierarchy
	// my changing 'this' GameObject's scale, rotation and translation
	addChild(m_pHTRAnimation->getRootGameObject());
}

glm::mat4 SkinnedGameObject::getJointToBindMatrix()
{
	return m_pJointToBindMat;
}

void SkinnedGameObject::update(float dt)
{
	// This GameObject has no jointAnimation so we will construct our
	// transformation matrices in the traditional way
	if (m_pJointAnimation == nullptr)
	{
		if (!playing)
			dt = 0.0f;

		Transform::update(dt);
	}
	else
	{
		if ((m_pCurrentFrame) >= (int)m_pJointAnimation->numFrames)
			(m_pCurrentFrame) = 0;

		glm::mat4 baseRotation = glm::mat4_cast(m_pJointAnimation->jointBaseRotation);
		glm::mat4 baseTranslation = glm::translate(m_pJointAnimation->jointBasePosition);
		glm::mat4 bindSpaceTransform = baseTranslation * baseRotation;

		if (m_pCurrentFrame != -1)
		{
			// Todo: create localRotation, scale and translation matrices using HTR data

			m_pLocalRotation =
				glm::mat4_cast(m_pJointAnimation->jointBaseRotation *
					m_pJointAnimation->jointRotations[m_pCurrentFrame]);

			m_pLocalPosition =
				m_pJointAnimation->jointBasePosition +
				m_pJointAnimation->jointPositions[m_pCurrentFrame];

			m_pLocalScale = glm::vec3(m_pJointAnimation->jointScales[m_pCurrentFrame]);

			// Create translation matrix
			glm::mat4 tran = glm::translate(m_pLocalPosition);

			// Create scale matrix
			glm::mat4 scal = glm::scale(m_pLocalScale);

			m_pLocalTransformMatrix = tran * m_pLocalRotation * scal;
		}
		else
			m_pLocalTransformMatrix = m_pJointToBindMat;

		if (m_pParent)
		{
			m_pLocalToWorldMatrix = m_pParent->getLocalToWorldMatrix() * m_pLocalTransformMatrix;
			m_pJointToBindMat = static_cast<SkinnedGameObject*>(m_pParent)->getJointToBindMatrix() * bindSpaceTransform;
		}
		else
		{
			m_pLocalToWorldMatrix = m_pLocalTransformMatrix;
			m_pJointToBindMat = bindSpaceTransform;
		}

		// Update children
		for (int i = 0; i < m_pChildren.size(); i++)
			m_pChildren[i]->update(dt);

		// Increment frame to next frame
		// Note: Right now we are just playing the animation back frame by frame
		// Since we exported the animation at 60 fps, as long as the game runs at 60 fps this works fine
		// However, to make this more robust, you should blend between frames (i.e. lerp / slerp) to get
		// frame rate independent playback. This will also allow you to control the
		// playback speed of the animation (hint: this is what you need to do for assignment 3!)
		m_pCurrentFrame++;

		// Make sure we are within our animation frame count
		/*if ((m_pCurrentFrame) >= (int)m_pJointAnimation->numFrames)
			(m_pCurrentFrame) = 0;*/
		//Safer to check at the beginning rather than the end
	}
}