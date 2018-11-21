#pragma once
#include "SkinnedGameObject.h"

class BlendTree
{
public:
	glm::quat Slerp(glm::quat q1, glm::quat q2, double t)
	{
		//Quaternion to return
		glm::quat quat;

		//Calculate the angle between the quaternions
		double cosHalfTheta = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

		//If q1 == q2 or q1 == -q2 then theta = 0 and return q1
		if (abs(cosHalfTheta) >= 1.0)
		{
			quat.w = q1.w;
			quat.x = q1.x;
			quat.y = q1.y;
			quat.z = q1.z;

			return quat;
		}

		//Calculate temp values
		double halfTheta = acos(cosHalfTheta);
		double sinHalfTheta = sqrt(1.0 - cosHalfTheta * cosHalfTheta);
		//If theta = 180 then result is not fully defined
		//We could rotate around any axis normal to q1 or q2
		if (fabs(sinHalfTheta) < 0.001) //Fabs is floating point absolute
		{
			quat.w = (q1.w * 0.5 + q2.w * 0.5);
			quat.w = (q1.x * 0.5 + q2.x * 0.5);
			quat.w = (q1.y * 0.5 + q2.y * 0.5);
			quat.w = (q1.z * 0.5 + q2.z * 0.5);

			return quat;
		}

		double ratio1 = sin((1 - t) * halfTheta) / sinHalfTheta;
		double ratio2 = sin(t * halfTheta) / sinHalfTheta;

		//Calculate the Quaternion
		quat.w = (q1.w * ratio1 + q2.w * ratio2);
		quat.x = (q1.x * ratio1 + q2.x * ratio2);
		quat.y = (q1.y * ratio1 + q2.y * ratio2);
		quat.z = (q1.z * ratio1 + q2.z * ratio2);

		return quat;
	}

	glm::quat RotationToQuaternion(glm::mat4 rm)
	{
		glm::quat quat;

		quat.w = sqrt(1 + rm[0][0] + rm[1][1] + rm[2][2]) / 2;
		quat.x = (rm[2][1] - rm[1][2]) / (4 * quat.w);
		quat.y = (rm[0][2] - rm[2][0]) / (4 * quat.w);
		quat.z = (rm[1][0] - rm[0][1]) / (4 * quat.w);

		glm::normalize(quat);

		return quat;
	}

	glm::mat4 QuaternionToRotation(glm::quat quat)
	{
		glm::mat4 rm;

		rm[0][0] = 1 - (2 * (quat.y * quat.y)) - (2 * (quat.z * quat.z));
		rm[0][1] = (2 * quat.x * quat.y) - (2 * quat.z * quat.w);
		rm[0][2] = (2 * quat.x * quat.y) + (2 * quat.y * quat.w);
		rm[0][3] = 0;

		rm[1][0] = (2 * quat.x * quat.y) + (2 * quat.z * quat.w);
		rm[1][1] = 1 - (2 * (quat.x * quat.x)) - (2 * (quat.z * quat.z));
		rm[1][2] = (2 * quat.y * quat.z) - (2 * quat.z * quat.w);
		rm[1][3] = 0;

		rm[2][0] = (2 * quat.z * quat.y) - (2 * quat.y * quat.w);
		rm[2][1] = (2 * quat.y * quat.z) + (2 * quat.x * quat.w);
		rm[2][2] = 1 - (2 * (quat.x * quat.x)) - (2 * (quat.y * quat.y));
		rm[2][3] = 0;

		rm[3][0] = 0;
		rm[3][1] = 0;
		rm[3][2] = 0;
		rm[3][3] = 1;

		//glm::normalize(rm);

		return rm;
	}
};