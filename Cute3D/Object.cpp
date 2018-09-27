#include "Object.h"
#include <iostream>

Object::Object(glm::vec3 position, glm::vec3 scale, glm::vec3 eulerRotation){
	m_position = position;
	m_scale = scale;
	m_eulerRotation = eulerRotation;
	m_orientation = glm::quat(m_eulerRotation);
}

Object::~Object()
{
	delete m_pShape;
	delete m_pBody;
	delete m_pMotionState;
}

void Object::initializeRigidBody(glm::vec3 size, glm::vec3 pos) {
	collisionOffset = pos*m_scale;
	m_pMotionState = new btDefaultMotionState(btTransform(
		btQuaternion(m_orientation.x, m_orientation.y, m_orientation.z, m_orientation.w),
		btVector3(m_position.x + collisionOffset.x, m_position.y + collisionOffset.y, m_position.z + collisionOffset.z)
	));
	m_pShape = new btBoxShape(btVector3(size.x, size.y, size.z));
	btVector3 localInertia(0, 0, 0);
	btRigidBody::btRigidBodyConstructionInfo cInfo(0, m_pMotionState, m_pShape, localInertia);
	m_pBody = new btRigidBody(cInfo);
	m_pShape->setLocalScaling(btVector3(m_scale.x, m_scale.y, m_scale.z));
	m_pBody->setUserPointer(this);
}
void Object::setPosition(glm::highp_float x, glm::highp_float y, glm::highp_float z)
{
	m_position += glm::vec3(x, y, z);
	btTransform transform;
	m_pBody->getMotionState()->getWorldTransform(transform);
	transform.setOrigin(btVector3(m_position.x + collisionOffset.x, m_position.y + collisionOffset.y, m_position.z + collisionOffset.z));

	m_pBody->getMotionState()->setWorldTransform(transform);
	m_pBody->setCenterOfMassTransform(transform);
}

void Object::setScale(glm::highp_float x, glm::highp_float y, glm::highp_float z)
{
	m_scale = glm::vec3(x, y, z);
	m_pShape->setLocalScaling(btVector3(x,y,z));
}

void Object::setRotation(glm::highp_float x, glm::highp_float y, glm::highp_float z)
{
	m_eulerRotation = glm::vec3(x, y, z);
	m_orientation = glm::quat(m_eulerRotation);
	btTransform transform;
	m_pBody->getMotionState()->getWorldTransform(transform);
	transform.setRotation(btQuaternion(m_orientation.x, m_orientation.y, m_orientation.z, m_orientation.w));
	
	m_pBody->getMotionState()->setWorldTransform(transform);
	m_pBody->setCenterOfMassTransform(transform);
}
