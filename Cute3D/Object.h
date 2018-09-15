#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "btBulletDynamicsCommon.h"

#include "Model.h"
#include "GLShader.h"


class Object
{
public:
	Object(size_t index, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3 eulerRotation = glm::vec3(0, 0, 0));
	~Object();
	
	btRigidBody* GetRigidBody() { return m_pBody; }

	void setPosition(glm::highp_float x = 0, glm::highp_float y = 0, glm::highp_float z = 0);
	void setScale(glm::highp_float x = 0, glm::highp_float y = 0, glm::highp_float z = 0);
	void setRotation(glm::highp_float x = 0, glm::highp_float y = 0, glm::highp_float z = 0);
	void draw(GLShader shader);

private:
	size_t m_model;
	btCollisionShape* m_pShape;
	btRigidBody* m_pBody;
	btDefaultMotionState* m_pMotionState;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_eulerRotation;
	glm::quat m_orientation;
};