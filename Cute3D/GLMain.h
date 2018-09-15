#pragma once

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QPoint>
#include <QMouseEvent>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include <chrono>
#include <vector>

#include "Camera.h"
#include "GLShader.h"
#include "Model.h"
#include "Object.h"
#include "DebugDrawer.h"
using namespace std::chrono;

class GLMain : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
	Q_OBJECT
public:

	GLMain(QWidget *parent = NULL);
	~GLMain();
protected:
	void mousePressEvent(QMouseEvent *event) override;
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();
private slots:
	void repaintGL();
private:

	void checkMouseState();
	void checkKeyState();

	int offsetx, offsety;
	bool pauseGame;

	double deltaTime;
	high_resolution_clock::time_point currentFrame, lastFrame;
	
	QCursor curs;
	QPoint cursorPos, center, last;

	Camera camera;
	glm::mat4 projection;
	glm::mat4 view;
	std::vector<GLShader> shaders;
	std::vector<Model> models;
	std::vector<Object*> objects;

	btBroadphaseInterface* m_pBroadphase;
	btCollisionConfiguration* m_pCollisionConfiguration;
	btCollisionDispatcher* m_pDispatcher;
	btConstraintSolver* m_pSolver;
	btDynamicsWorld* m_pWorld;
	DebugDrawer* m_pDebugDrawer;
};