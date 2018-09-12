#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "Model.h"
#include "GLShader.h"

class Object
{
public:
	Object(Model* model, btCollisionShape* pShape, glm::vec3 position, glm::vec3 scale, glm::vec3 eulerRotation);
	~Object();
	
	void setPosition(glm::highp_float x = 0, glm::highp_float y = 0, glm::highp_float z = 0);
	void setScale(glm::highp_float x = 0, glm::highp_float y = 0, glm::highp_float z = 0);
	void setRotation(glm::highp_float x = 0, glm::highp_float y = 0, glm::highp_float z = 0);
	void draw(GLShader shader);

private:
	Model* model;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 eulerRotation;
	glm::quat orientation;
};