#include "TTK/OBJMesh.h"
#include "glm/glm.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>
#include <GLM\gtx\transform.hpp>

void TTK::OBJMesh::loadMesh(std::string filename)
{
	std::ifstream file;

	//open file
	file.open(filename.c_str());

	//check if file opened
	if (file.fail() == true)
	{
		std::cout << "Error - OBJMesh::loadMesh file: " << filename << " not found.\n";
		return;
	}

	char currentChar;

	glm::vec3 temp;
	Face3 temp2;

	file.get(currentChar);

	while (!file.eof())
	{
		if (currentChar == 'v')
		{
			file.get(currentChar);
			if (currentChar == ' ')
			{
				file >> temp.x >> temp.y >> temp.z;
				temp = glm::rotate(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(temp, 1.0f);

				objVertices.push_back(temp);
			}
			if (currentChar == 't')
			{
				file >> temp.x >> temp.y;
				temp.z = 0.0f;
				objUVs.push_back(temp);
			}
			if (currentChar == 'n')
			{
				file >> temp.x >> temp.y >> temp.z;

				temp = glm::rotate(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(temp, 1.0f);

				objNormals.push_back(temp);
			}
		}
		else
		{
			if (currentChar == 'f')
			{
				file.get(currentChar);
				if (currentChar == ' ')
				{
					file >> temp2.vertex1 >> currentChar >> temp2.texture1 >> currentChar >> temp2.normal1;
					file >> temp2.vertex2 >> currentChar >> temp2.texture2 >> currentChar >> temp2.normal2;
					file >> temp2.vertex3 >> currentChar >> temp2.texture3 >> currentChar >> temp2.normal3;
					objFaces.push_back(temp2);
				}
			}
		}
		file.get(currentChar);
	}

	file.close();

	updateMesh(true, true, true, false);

	//createVBO();
}

void TTK::OBJMesh::updateMesh(bool updateVertices, bool updateNormals, bool updateUVs, bool calculateNormals)
{
	// Unpack data
	if (updateVertices)
	{
		vertices.clear();
		vertices.reserve(objVertices.size());
	}

	if (updateNormals || calculateNormals)
	{
		normals.clear();
		normals.reserve(objNormals.size());
	}

	if (updateUVs)
	{
		textureCoordinates.clear();
		textureCoordinates.reserve(objUVs.size());
	}

	for (int i = 0; i < objFaces.size(); i++)
	{
		Face3* face = &objFaces[i];

		if (updateVertices)
		{
			vertices.push_back(objVertices[face->vertex1 - 1]);
			vertices.push_back(objVertices[face->vertex2 - 1]);
			vertices.push_back(objVertices[face->vertex3 - 1]);
		}

		if (updateNormals)
		{
			normals.push_back(objNormals[face->normal1 - 1]);
			normals.push_back(objNormals[face->normal2 - 1]);
			normals.push_back(objNormals[face->normal3 - 1]);
		}

		// Per-face normal
		if (calculateNormals)
		{
			glm::vec3 a = objVertices[face->vertex1 - 1];
			glm::vec3 b = objVertices[face->vertex2 - 1];
			glm::vec3 c = objVertices[face->vertex3 - 1];

			glm::vec3 u = a - b;
			glm::vec3 v = a - c;
			glm::vec3 n = glm::cross(u, v);
			normals.push_back(n);
			normals.push_back(n);
			normals.push_back(n);
		}

		if (updateUVs)
		{
			textureCoordinates.push_back(objUVs[face->texture1 - 1]);
			textureCoordinates.push_back(objUVs[face->texture2 - 1]);
			textureCoordinates.push_back(objUVs[face->texture3 - 1]);
		}
	}
}

