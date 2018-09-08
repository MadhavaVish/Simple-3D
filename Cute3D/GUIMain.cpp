#include "GUIMain.h"

#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QSurfaceFormat>

GUIMain::GUIMain(QWidget *parent) : QMainWindow(parent)
{
	QMenu *file;
	file = menuBar()->addMenu("File");
	QAction *quit = new QAction("Quit", this);
	file->addAction(quit);

	view3D = new GLMain(this);

	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(4, 6);
	format.setSwapInterval(0);
	QSurfaceFormat::setDefaultFormat(format);
	
	setCentralWidget(view3D);

	
	connect(quit, &QAction::triggered, qApp, QApplication::quit);
}
