// PUT YOUR NAME AND STUDENT NUMBER HERE //

#include "Transform.h"
#include <iostream>
#include "TTK\GraphicsUtils.h"

Transform::Transform()
	: m_pLocalScale(1.0f), colour(glm::vec4(1.0)), m_pParent(nullptr)
{
}

Transform::~Transform() {}

void Transform::addChild(Transform * newChild)
{
	if (newChild)
	{
		m_pChildren.push_back(newChild);
		newChild->m_pParent = this; // tell new child that this game object is its parent
	}
}

void Transform::removeChild(Transform * rip)
{
	for (int i = 0; i < m_pChildren.size(); ++i)
	{
		if (m_pChildren[i] == rip) // compare memory locations (pointers)
		{
			std::cout << "Removing child: " + rip->name << " from object: " << this->name;
			m_pChildren.erase(m_pChildren.begin() + i);
		}
	}
}

void Transform::setLocalPosition(glm::vec3 newPosition)
{
	m_pLocalPosition = newPosition;
}

void Transform::setLocalRotationAngleX(float newAngle)
{
	m_pLocalRotationAngles.x = newAngle;
}

void Transform::setLocalRotationAngleY(float newAngle)
{
	m_pLocalRotationAngles.y = newAngle;
}

void Transform::setLocalRotationAngleZ(float newAngle)
{
	m_pLocalRotationAngles.z = newAngle;
}

void Transform::setLocalScale(glm::vec3 newScale)
{
	m_pLocalScale = newScale;
}

glm::vec3 Transform::getLocalPosition()
{
	return m_pLocalPosition;
}

glm::vec3 Transform::getLocalRotationAngles()
{
	return m_pLocalRotationAngles;
}

glm::vec3 Transform::getLocalScale()
{
	return m_pLocalScale;
}

glm::mat4 Transform::getLocalToWorldMatrix()
{
	return m_pLocalToWorldMatrix;
}

glm::vec3 Transform::getWorldPosition()
{
	if (m_pParent)
		return m_pParent->getLocalToWorldMatrix() * glm::vec4(m_pLocalPosition, 1.0f);
	else
		return m_pLocalPosition;
}

glm::mat4 Transform::getWorldRotation()
{
	if (m_pParent)
		return m_pParent->getWorldRotation() * m_pLocalRotation;
	else
		return m_pLocalRotation;
}

std::vector<Transform*> Transform::getChildren()
{
	return m_pChildren;
}

Transform * Transform::getParent()
{
	return m_pParent;
}

void Transform::update(float dt)
{
	// Create 4x4 transformation matrix

	// Create rotation matrix
	glm::mat4 rx = glm::rotate(glm::radians(m_pLocalRotationAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 ry = glm::rotate(glm::radians(m_pLocalRotationAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rz = glm::rotate(glm::radians(m_pLocalRotationAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Note: pay attention to rotation order, ZYX is not the same as XYZ
	m_pLocalRotation = rz * ry * rx;

	// Create translation matrix
	glm::mat4 tran = glm::translate(m_pLocalPosition);

	// Create scale matrix
	glm::mat4 scal = glm::scale(m_pLocalScale);

	// Combine all above transforms into a single matrix
	// This is the local transformation matrix, ie. where is this game object relative to it's parent
	// If a game object has no parent (it is a root node) then its local transform is also it's global transform
	// If a game object has a parent, then we must apply the parent's transform
	m_pLocalTransformMatrix = tran * m_pLocalRotation * scal;

	if (m_pParent)
		m_pLocalToWorldMatrix = m_pParent->getLocalToWorldMatrix() * m_pLocalTransformMatrix;
	else
		m_pLocalToWorldMatrix = m_pLocalTransformMatrix;

	// Update children
	for (int i = 0; i < m_pChildren.size(); i++)
		m_pChildren[i]->update(dt);
}

void Transform::draw()
{
	drawKinematicLinkage();
}

void Transform::drawKinematicLinkage()
{
	// Draw a coordinate frame for the object
	glm::vec3 wPos = getWorldPosition();
	glm::mat4 wRot = getWorldRotation();

	glm::vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 blue(0.0f, 0.0f, 1.0f, 1.0f);

	TTK::Graphics::DrawVector(wPos, wRot[0] * 0.25f, 3.0f, red);
	TTK::Graphics::DrawVector(wPos, wRot[1] * 0.25f, 3.0f, green);
	TTK::Graphics::DrawVector(wPos, wRot[2] * 0.25f, 3.0f, blue);

	// Draw node
	//TTK::Graphics::DrawSphere(m_pLocalToWorldMatrix, 0.5f, colour);

	// Draw line to parent 
	if (m_pParent)
	{
		glm::vec3 parentWorldPosition = m_pParent->getWorldPosition();
		glm::vec3 myWorldPositon = getWorldPosition();
		TTK::Graphics::DrawLine(myWorldPositon, parentWorldPosition, 5.0f, colour);
	}

	// Draw children
	for (int i = 0; i < m_pChildren.size(); ++i)
		m_pChildren[i]->drawKinematicLinkage();
}
