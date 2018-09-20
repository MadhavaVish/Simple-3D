#include "GUIMain.h"

#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QSurfaceFormat>
#include <QLayout>
#include <QPushButton>

#include <iostream>

GUIMain::GUIMain(QWidget *parent) : QMainWindow(parent)
{
	QMenu *file;
	file = menuBar()->addMenu("File");
	QAction *quit = new QAction("Quit", this);
	file->addAction(quit);

	view3D = new GLMain(this);

	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CompatibilityProfile);
	format.setVersion(4, 6);
	format.setSwapInterval(0);
	QSurfaceFormat::setDefaultFormat(format);
	
	setCentralWidget(view3D);

	dock = new QDockWidget(this);

	QPushButton* button = new QPushButton("Browse");
	QLineEdit* lineEdit = new QLineEdit();
	QWidget* modelSelector = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(lineEdit);
	layout->addWidget(button);
	
	modelSelector->setLayout(layout);
	dock->setWidget(modelSelector);
	addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, dock);
	connect(button, SIGNAL(clicked()), SLOT(browse()));
	connect(quit, &QAction::triggered, qApp, QApplication::quit);
	connect(this, SIGNAL(fileSelected(std::string)), view3D, SLOT(addModel(std::string)));
}

void GUIMain::browse() {
	filename = QFileDialog::getOpenFileName(this,
		tr("Open obj file"), "", tr("Model Files (*.obj)")).toStdString();
	std::cout << filename << std::endl;
	if (filename != "")	emit fileSelected(filename);

}