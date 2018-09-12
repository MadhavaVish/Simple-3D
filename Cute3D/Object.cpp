#include "Object.h"

Object::Object(Model* model, btCollisionShape* pShape, glm::vec3 position, glm::vec3 scale, glm::vec3 eulerRotation) : position(position), scale(scale), eulerRotation(eulerRotation)
{
}

Object::~Object()
{
}

void Object::setPosition(glm::highp_float x, glm::highp_float y, glm::highp_float z)
{
}

void Object::setScale(glm::highp_float x, glm::highp_float y, glm::highp_float z)
{
}

void Object::setRotation(glm::highp_float x, glm::highp_float y, glm::highp_float z)
{
}

void Object::draw(GLShader shader)
{
}
