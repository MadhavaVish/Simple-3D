#include "GLMain.h"
#include <iostream>

GLMain::GLMain(QWidget *parent) : pauseGame(true)
{
	curs = cursor();
	setCursor(curs);
	curs.setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
	curs.setShape(Qt::BlankCursor);
	last = QPoint(width() / 2, height() / 2);

	
}

GLMain::~GLMain()
{
	delete m_pWorld;
	delete m_pCollisionConfiguration;
	delete m_pDispatcher;
	delete m_pBroadphase;
	delete m_pSolver;
	delete m_pDebugDrawer;
	for (auto object : objects) {
		delete object;
	}
	objects.clear();
}

void GLMain::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		if (pauseGame) {
			cursorPos = QCursor::pos();
			float x = ((float)event->x() / (float)width() - 0.5f) * 2.0f;
			float y = abs(event->y()-height());
			y = (y/(float)height() - 0.5f) * 2.0f;

			glm::vec4 lRayStart_NDC(x, y, -1.0, 1.0f);
			glm::vec4 lRayEnd_NDC(x, y, 0, 1.0f);

			glm::mat4 M = glm::inverse(projection * view);
			glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
			glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;

			glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
			lRayDir_world = glm::normalize(lRayDir_world);
			glm::vec3 out_end = glm::vec3(lRayStart_world) + lRayDir_world * 1000.0f;
			btCollisionWorld::ClosestRayResultCallback RayCallback(
				btVector3(lRayStart_world.x, lRayStart_world.y, lRayStart_world.z),
				btVector3(out_end.x, out_end.y, out_end.z)
			);
			m_pWorld->rayTest(
				btVector3(lRayStart_world.x, lRayStart_world.y, lRayStart_world.z),
				btVector3(out_end.x, out_end.y, out_end.z),
				RayCallback
			);
			int nice;
			std::cout << RayCallback.hasHit() << std::endl;
			if (RayCallback.hasHit()) {
				nice = (int)RayCallback.m_collisionObject->getUserPointer();
				std::cout << nice << std::endl;

				objects[0]->setScale(0.1f, 0.1f, 0.1f);
			}
			else {
				std::cout << "background" << std::endl;
				objects[0]->setScale(0.2f, 0.2f, 0.2f);
			}
		}
	}
}

void GLMain::initializeGL()
{
	initializeOpenGLFunctions();
	connect(this, SIGNAL(frameSwapped()), this, SLOT(repaintGL()));
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	m_pDispatcher = new	btCollisionDispatcher(m_pCollisionConfiguration);
	m_pBroadphase = new btDbvtBroadphase();
	m_pSolver = new btSequentialImpulseConstraintSolver();
	m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);
	glViewport(0, 0, width(), height());
	glClearColor(0.1,0.1,0.1 , 1.0f);
	glEnable(GL_DEPTH_TEST);

	shaders.push_back(GLShader("Basic.vert", "Basic.frag"));
	shaders[0].use();
	shaders[0].setInt("diffuse", 0);
	shaders[0].setInt("specular", 1);
	shaders.push_back(GLShader("Debug.vert", "Debug.frag"));
	objects.push_back(new Object(models.size()));
	models.push_back(Model("nanosuit/monki.obj"));
	if(m_pWorld)
		m_pWorld->addRigidBody(objects[0]->GetRigidBody());
	m_pDebugDrawer = new DebugDrawer();
	m_pDebugDrawer->setDebugMode(0);
	m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);
	m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawAabb);
	m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawContactPoints);

	m_pWorld->setDebugDrawer(m_pDebugDrawer);
	std::cout << glGetString(GL_VERSION) << std::endl;
}

void GLMain::repaintGL() {
	checkKeyState();
	if (!pauseGame) {
		checkMouseState();
	}
	update();
}

void GLMain::paintGL()
{
	currentFrame = high_resolution_clock::now();	
	deltaTime = (double)duration_cast<nanoseconds>(currentFrame - lastFrame).count()/1000000000;
	lastFrame = currentFrame;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	projection = glm::perspective(45.0f, ((float)width()) / height(), 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	shaders[0].use();
	shaders[0].setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shaders[0].setVec3("lightPos", 1.2f, 1.0f, 2.0f);
	shaders[0].setVec3("viewPos", camera.Position);
	shaders[0].setMat4("projection", projection);
	shaders[0].setMat4("view", view);
	
	objects[0]->draw(shaders[0]);
	models[0].draw(shaders[0]);

	shaders[1].use();
	shaders[1].setMat4("projection", projection);
	shaders[1].setMat4("view", view);

	m_pWorld->debugDrawWorld();
}

void GLMain::checkMouseState() {
	center = mapToGlobal(QPoint(width() / 2, height() / 2));
	cursorPos = QCursor::pos();
	offsetx = cursorPos.x() - last.x();
	offsety = last.y() - cursorPos.y();
	QCursor::setPos(center);
	last = center;
	camera.ProcessMouseMovement(offsetx / 2, offsety / 2);
}

void GLMain::checkKeyState()
{
	if (!pauseGame) {
		if (GetAsyncKeyState(0x57))
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (GetAsyncKeyState(0x53))
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (GetAsyncKeyState(0x41))
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (GetAsyncKeyState(0x44))
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (GetAsyncKeyState(VK_RBUTTON)) {
		if (pauseGame) {
			last = QCursor::pos();
		}
		pauseGame = false;
		curs.setShape(Qt::BlankCursor);
		setCursor(curs);
	}
	else {
		pauseGame = true;
		curs.setShape(Qt::ArrowCursor);
		setCursor(curs);
	}
}
void GLMain::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}