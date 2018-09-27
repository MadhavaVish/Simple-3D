#pragma once

#include <string>

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "btBulletDynamicsCommon.h"

#include "GLShader.h"


class Object
{
public:
	Object(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3 eulerRotation = glm::vec3(0, 0, 0));
	~Object();

	void initializeRigidBody(glm::vec3 size = glm::vec3(1, 1, 1), glm::vec3 pos = glm::vec3(0,0,0));

	btRigidBody* GetRigidBody() { return m_pBody; }

	void setPosition(glm::highp_float x = 0, glm::highp_float y = 0, glm::highp_float z = 0);
	void setScale(glm::highp_float x = 0, glm::highp_float y = 0, glm::highp_float z = 0);
	void setRotation(glm::highp_float x = 0, glm::highp_float y = 0, glm::highp_float z = 0);
protected:

	btCollisionShape* m_pShape;
	btRigidBody* m_pBody;
	btDefaultMotionState* m_pMotionState;

	glm::vec3 m_position;
	glm::vec3 collisionOffset;
	glm::vec3 m_scale;
	glm::vec3 m_eulerRotation;
	glm::quat m_orientation;
};