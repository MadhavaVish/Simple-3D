#include "GLMain.h"
#include <iostream>

GLMain::GLMain(QWidget *parent) : pauseGame(true)
{
	moving = false;
	selected = nullptr;
	cursor = QWidget::cursor();
	setCursor(cursor);
	cursor.setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
	cursor.setShape(Qt::BlankCursor);
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
	for (auto model : models) delete model;
}

void GLMain::initializeGL()
{
	setupBullet();
	setupGL();
	std::cout << glGetString(GL_VERSION) << std::endl;
	setupShaders();
	addModel("objects/monki.obj");
	connect(this, SIGNAL(frameSwapped()), this, SLOT(repaintGL()));
}

void GLMain::repaintGL() {
	checkKeyState();
	checkMouseState();
	update();
}

void GLMain::paintGL()
{
	currentFrame = high_resolution_clock::now();
	deltaTime = duration<double>(currentFrame - lastFrame).count();
	lastFrame = currentFrame;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view = camera.GetViewMatrix();

	shaders[0].use();
	shaders[0].setMat4("projection", projection);
	shaders[0].setMat4("view", view);

	shaders[0].setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shaders[0].setVec3("lightPos", 1.2f, 1.0f, 2.0f);
	shaders[0].setVec3("viewPos", camera.Position);
	
	for (auto model : models) model->draw(shaders[0]);

	shaders[1].use();
	shaders[1].setMat4("projection", projection);
	shaders[1].setMat4("view", view);

	m_pWorld->debugDrawWorld();
}

void GLMain::checkMouseState() {
	center = mapToGlobal(QPoint(width() / 2, height() / 2));
	cursorPos = cursor.pos();
	offsetx = cursorPos.x() - last.x();
	
	if (moving&&selected!=nullptr) {
		selected->setPosition((float)offsetx*0.02, 0, 0);
	}

	offsety = last.y() - cursorPos.y();
	last = cursorPos;

	if (!pauseGame) {
		cursor.setPos(center);
		last = center;
		camera.ProcessMouseMovement(offsetx, offsety);
	}
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
	if (GetAsyncKeyState(VK_ESCAPE)) {
		selected = nullptr;
		moving = false;
	}
	if (GetAsyncKeyState(0x47)) {
		if(selected)
			moving = true;
	}
	if (GetAsyncKeyState(VK_RBUTTON)) {
		moving = false;
		if (pauseGame) {
			last = cursor.pos();
		}
		pauseGame = false;
		cursor.setShape(Qt::BlankCursor);
		setCursor(cursor);
	}
	else {
		pauseGame = true;
		cursor.setShape(Qt::ArrowCursor);
		setCursor(cursor);
	}
}

void GLMain::mousePressEvent(QMouseEvent * event)
{
	if (moving) {
		moving = false;
		m_pWorld->updateSingleAabb(selected->GetRigidBody());
	}
	else {
		
		if (event->button() == Qt::LeftButton) {
			if (pauseGame) {
				float x = ((float)event->x() / (float)width() - 0.5f) * 2.0f;
				float y = (abs(event->y() - height()) / (float)height() - 0.5) * 2.0f;

				glm::vec4 lRayStart_NDC(x, y, -1.0, 1.0f);
				glm::vec4 lRayEnd_NDC(x, y, 0, 1.0f);

				glm::mat4 M = glm::inverse(projection * view);
				glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world /= lRayStart_world.w;
				glm::vec4 lRayEnd_world = M * lRayEnd_NDC; lRayEnd_world /= lRayEnd_world.w;

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

				if (RayCallback.hasHit()) {
					selected = static_cast<Object*>(RayCallback.m_collisionObject->getUserPointer());
					selected->setRotation(0,0,3.14159/2);
					std::cout << "nut" << std::endl;
				}
			}
		}
	}
}

void GLMain::setupBullet() {
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	m_pDispatcher = new	btCollisionDispatcher(m_pCollisionConfiguration);
	m_pBroadphase = new btDbvtBroadphase();
	m_pSolver = new btSequentialImpulseConstraintSolver();
	m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);
	m_pDebugDrawer = new DebugDrawer();
	m_pDebugDrawer->setDebugMode(0);
	m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawAabb);
	m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);
	m_pWorld->setDebugDrawer(m_pDebugDrawer);
}

void GLMain::setupGL() {
	initializeOpenGLFunctions();
	glViewport(0, 0, width(), height());
	glClearColor(0.1, 0.1, 0.1, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void GLMain::setupShaders() {
	shaders.push_back(GLShader("Basic.vert", "Basic.frag"));
	shaders[0].use();
	shaders[0].setInt("diffuse", 0);
	shaders[0].setInt("specular", 1);

	shaders.push_back(GLShader("Debug.vert", "Debug.frag"));
}

void GLMain::addModel(std::string const& path) { 
	std::cout << path << std::endl;
	models.push_back(new Model(path));
	if (m_pWorld) m_pWorld->addRigidBody(models.back()->GetRigidBody());
	for (auto model : models) std::cout << model->GetRigidBody() << std::endl;
}

void GLMain::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	projection = glm::perspective(45.0f, ((float)w) / h, 0.1f, 100.0f);
}