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
}

void GLMain::initializeGL()
{
	initializeOpenGLFunctions();
	connect(this, SIGNAL(frameSwapped()), this, SLOT(repaintGL()));

	glViewport(0, 0, width(), height());
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	shaders.push_back(GLShader("Basic.vert", "Basic.frag"));
	shaders[0].use();
	shaders[0].setInt("diffuse", 0);
	shaders[0].setInt("specular", 1);
	objects.push_back(Model("nanosuit/monki.obj"));
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

	glm::mat4 projection = glm::perspective(45.0f, ((float)width()) / height(), 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	shaders[0].use();
	shaders[0].setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shaders[0].setVec3("lightPos", 1.2f, 1.0f, 2.0f);
	shaders[0].setVec3("viewPos", camera.Position);
	shaders[0].setMat4("projection", projection);
	shaders[0].setMat4("view", view);

	objects[0].draw(shaders[0]);
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