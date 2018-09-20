#pragma once

#include <QMainWindow>
#include <QDockWidget>
#include <QFileDialog>
#include <QLineEdit>

#include <string>

#include "GLMain.h"

class GUIMain : public QMainWindow
{
	Q_OBJECT
public:

	GUIMain(QWidget *parent = nullptr);
private slots:
	void browse();
signals:
	void fileSelected(std::string path);
private:
	QDockWidget* dock;
	GLMain* view3D;
	std::string filename;
};

