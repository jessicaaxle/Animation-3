//////////////////////////////////////////////////////////////////////////
//
// This header is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this header in your GDW games.
//
//
// Michael Gharbharan 2015
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "TTK/MeshBase.h"
#include <string>

namespace TTK
{
	typedef struct
	{
		int vertex1, texture1, normal1;
		int vertex2, texture2, normal2;
		int vertex3, texture3, normal3;
	}Face3;

	class OBJMesh : public MeshBase
	{
	public:
		void loadMesh(std::string filename);
		void updateMesh(bool updateVertices, bool updateNormals, bool updateUVs, bool calculateNormals);

		// Containers for OBJ data
		std::vector<glm::vec3> objVertices;
		std::vector<glm::vec3> objUVs;
		std::vector<glm::vec3> objNormals;
		std::vector<Face3> objFaces;

		std::vector<float> objVertWeights;
	};
} 