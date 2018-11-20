#pragma once

#include "Transform.h"
#include "HTRLoader.h"
#include "TTK\OBJMesh.h"

// Forward declare the HTRLoader class
// This is necessary because SkinnedGameObject references HTRLoader and HTRLoader references SkinnedGameObject
class HTRLoader;

class SkinnedGameObject : public Transform
{
	// Let the HTRLoader class access private and protected member variables
	friend class HTRLoader;

protected:
	int m_pCurrentFrame;
	HTRLoader* m_pHTRAnimation;
	JointDescriptor* m_pJointAnimation; // The animation for this specific joint

	std::shared_ptr<TTK::OBJMesh> m_pBindMesh; // Mesh in t-pose
	std::shared_ptr<TTK::OBJMesh> m_pSkinnedMesh;

	glm::mat4 m_pJointToBindMat;
public:
	SkinnedGameObject();
	~SkinnedGameObject();

	bool playing;

	void initializeSkeletonFromHTR(std::string htrFilePath, std::string skinWeightFilePath, std::shared_ptr<TTK::OBJMesh> bindMesh);

	void loadSkeletonFromHTR(std::string htrFilePath);

	glm::mat4 getJointToBindMatrix();

	virtual void update(float dt);

	std::vector<std::string> animFiles;
};