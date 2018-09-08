#pragma once

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
//#include <QCursor>
#include <QPoint>

#include <chrono>
#include <vector>

#include "Camera.h"
#include "GLShader.h"
#include "Model.h"

using namespace std::chrono;

class GLMain : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
	Q_OBJECT
public:

	GLMain(QWidget *parent = NULL);
	~GLMain();
protected:

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
	std::vector<GLShader> shaders;
	std::vector<Model> objects;
};


