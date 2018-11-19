// PUT YOUR NAME AND STUDENT NUMBER HERE //
#pragma once

#include <GLM/glm.hpp>
#include <GLM\gtx\transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <string>

#include "TTK\QuadMesh.h"
#include <memory>

#include "JointTypes.h"

class Transform
{
protected:
	glm::vec3 m_pLocalScale;

	glm::vec3 m_pLocalRotationAngles; // local rotation angles (degrees)

	glm::vec3 m_pLocalPosition;
	glm::mat4 m_pLocalRotation;
	glm::mat4 m_pLocalTransformMatrix;
	glm::mat4 m_pLocalToWorldMatrix; // parentLocalToWorld * m_pLocalTransformation 

	// Forward kinematics
	Transform* m_pParent; // Pointer to parent
	std::vector<Transform*> m_pChildren; // Pointers to children

public:
	Transform();
	~Transform();

	// Forward Kinematics
	void addChild(Transform* newChild);
	void removeChild(Transform* rip);

	// Note: These now set LOCAL transformation properties
	// Local means: relative to the PARENT
	// Example:
	// if this gameobject's parent located at (0, 2, 0) in WORLD SPACE
	// and this gameobject's LOCAL position is (0,0,0) then that means it is (0,0,0) units
	// from its parent. The WORLD SPACE position of this gameobject would be (0,2,0).
	// If this gameobject's LOCAL position was (0,2,0) then it would be (0,2,0) units from
	// it's parent. Which would mean the gameobject's WORLD position would be (0,4,0).
	// (0,4,0) = world position of parent + gameobject's local offset from parent
	// This is a very important concept to understand!
	void setLocalPosition(glm::vec3 newPosition);
	void setLocalRotationAngleX(float newAngle);
	void setLocalRotationAngleY(float newAngle);
	void setLocalRotationAngleZ(float newAngle);
	void setLocalScale(glm::vec3 newScale);

	glm::vec3 getLocalPosition();
	glm::vec3 getLocalRotationAngles();
	glm::vec3 getLocalScale();

	// parentLocalToWorld * localTransformation
	glm::mat4 getLocalToWorldMatrix();

	glm::vec3 getWorldPosition();

	glm::mat4 getWorldRotation();

	std::vector<Transform*> getChildren();

	Transform* getParent();

	virtual void update(float dt);	
	virtual void draw();

	void drawKinematicLinkage();

	// Other Properties
	std::string name;
	glm::vec4 colour; 
	// Material* ...
};