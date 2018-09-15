#include "DebugDrawer.h"
#include <iostream>
DebugDrawer::DebugDrawer() {
	initializeOpenGLFunctions();
}
void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{

	GLfloat points[12];
	
	points[0] = from.x();
	points[1] = from.y();
	points[2] = from.z();
	points[3] = color.x();
	points[4] = color.y();
	points[5] = color.z();
	
	points[6] = to.x();
	points[7] = to.y();
	points[8] = to.z();
	points[9] = color.x();
	points[10] = color.y();
	points[11] = color.z();
	//std::cout << points[0] << points[2] << points[3] << std::endl;
	//std::cout << points[6] << points[7] << points[8] << std::endl;
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);

}

void DebugDrawer::drawContactPoint(const btVector3 &pointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)
{
	btVector3 const startPoint = pointOnB;
	btVector3 const endPoint = pointOnB + normalOnB * distance;
	drawLine( startPoint, endPoint, color );
}

void DebugDrawer::ToggleDebugFlag(int flag) {
	if (m_debugMode & flag)
		m_debugMode = m_debugMode & (~flag);
	else
		m_debugMode |= flag;
}